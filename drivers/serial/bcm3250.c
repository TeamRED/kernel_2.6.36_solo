/*  vi:noet:sw=8 ts=8
 *
 *  linux/drivers/serial/bcm97xxx_serial.c
 *
 *  Driver for UPG DUART found in some Broadcom 97xxx devices.
 *
 *  Copyright (C) 2007 Pace Microtechnology plc
 *
 *  Based on drivers/serial/clps711x.c, by ARM Ltd.
 *  Based on drivers/serial/at91_serial.c, by Rick Bronson.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <linux/clk.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/tty_flip.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/brcmstb/brcmstb.h>

#if defined(CONFIG_SERIAL_BCM3250_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/serial_core.h>

#define UART_NR 3

#if ! defined(CONFIG_SERIAL_BCM3250_TTYS)

/* Use device name ttyBCM, major 204 and minor 187,188. We don't want to
 * conflict with the 8250 driver, which is used if a 16550 UART is also
 * present */
#define SERIAL_BCM97XXX_MAJOR	204
#define MINOR_START		187
#define BCM97XXX_DEVICENAME	"ttyBCM"

#else

/* Use device name ttyS, major 4, minor 64-68.  This is the "usual" serial port
 * name, but it is supposed to be reserved for the 8250 driver. */
#define SERIAL_BCM97XXX_MAJOR	TTY_MAJOR
#define MINOR_START		64
#define BCM97XXX_DEVICENAME	"ttyS"
#endif

/* UART register offsets and fields */
#define UART_RXSTAT		0x00
#define	 BCM_RXSTAT_PE		(1<<5)
#define	 BCM_RXSTAT_FE		(1<<4)
#define	 BCM_RXSTAT_OVRN	(1<<3)
#define	 BCM_RXSTAT_RDA		(1<<2)
#define	 BCM_RXSTAT_RIE		(1<<1)

#define  BCM_RXSTAT_ERR		(BCM_RXSTAT_PE | BCM_RXSTAT_FE | BCM_RXSTAT_OVRN)

#define UART_RXDATA		0x04

#define UART_CONTROL		0x0c
#define	 BCM_CONTROL_STOP2	(1<<6)
#define	 BCM_CONTROL_LOOP	(1<<5)
#define	 BCM_CONTROL_BITM8	(1<<4)
#define	 BCM_CONTROL_PAREN	(1<<3)
#define	 BCM_CONTROL_TXE	(1<<2)
#define	 BCM_CONTROL_RXE	(1<<1)
#define	 BCM_CONTROL_PODD	(1<<0)
#define	 BCM_CONTROL_MASK	(0x7F)

#define UART_BAUDHI		0x10
#define	 BCM_BAUDHI_BRR1	(1<<7)
#define	 BCM_BAUDHI_BRR_MASK	(0x3F)

#define UART_BAUDLO		0x14
#define	 BCM_BAUDLO_BRR_MASK	(0xFF)

#define UART_TXSTAT		0x18
#define	 BCM_TXSTAT_TIE		(1<<2)
#define	 BCM_TXSTAT_IDLE	(1<<1)
#define	 BCM_TXSTAT_TDRE	(1<<0)

#define UART_TXDATA		0x1c

#define uart_read(port, offset)		__raw_readl((void*)((port)->membase + (offset)))
#define uart_write(port, offset, value)	__raw_writel((value), (void*)((port)->membase + (offset)))

static int (*bcm97xxx_open)(struct uart_port *);
static void (*bcm97xxx_close)(struct uart_port *);

static struct uart_ops bcm97xxx_pops;

static struct uart_port bcm97xxx_ports[UART_NR];

#ifdef SUPPORT_SYSRQ
static struct console bcm97xxx_console;
#endif

/*
 * Return TIOCSER_TEMT when transmitter FIFO and Shift register is empty.
 */
static u_int bcm97xxx_tx_empty(struct uart_port *port)
{
	if ((uart_read(port, UART_TXSTAT) & (BCM_TXSTAT_IDLE | BCM_TXSTAT_TDRE)) == (BCM_TXSTAT_IDLE | BCM_TXSTAT_TDRE)) {
		return TIOCSER_TEMT;
	}

	return 0;
}

/*
 * Set state of the modem control output lines
 */
static void bcm97xxx_set_mctrl(struct uart_port *port, u_int mctrl)
{
	/* TODO */
}

/*
 * Get state of the modem control input lines
 */
static u_int bcm97xxx_get_mctrl(struct uart_port *port)
{
	/* TODO */
	return TIOCM_CTS;
}

/*
 * Stop transmitting.
 */
static void bcm97xxx_stop_tx(struct uart_port *port)
{
	uint32_t txstat = uart_read(port, UART_TXSTAT);

	uart_write(port, UART_TXSTAT,  txstat  & ~(BCM_TXSTAT_TIE));
}

/*
 * Start transmitting.
 */
static void bcm97xxx_start_tx(struct uart_port *port)
{
	uint32_t control = uart_read(port, UART_CONTROL);
	uint32_t txstat  = uart_read(port, UART_TXSTAT);

	uart_write(port, UART_CONTROL, control | BCM_CONTROL_TXE);
	uart_write(port, UART_TXSTAT,  txstat  | BCM_TXSTAT_TIE);
}

/*
 * Stop receiving - port is in process of being closed.
 */
static void bcm97xxx_stop_rx(struct uart_port *port)
{
	uint32_t control = uart_read(port, UART_CONTROL);
	uint32_t rxstat  = uart_read(port, UART_RXSTAT);

	uart_write(port, UART_CONTROL, control & ~(BCM_CONTROL_RXE));
	uart_write(port, UART_RXSTAT,  rxstat  & ~(BCM_RXSTAT_RIE));
}

/*
 * Enable modem status interrupts
 */
static void bcm97xxx_enable_ms(struct uart_port *port)
{
	/* TODO */
}

/*
 * Control the transmission of a break signal
 */
static void bcm97xxx_break_ctl(struct uart_port *port, int break_state)
{
	/* TODO */
}

/*
 * Characters received (called from interrupt handler)
 */
static void bcm97xxx_rx_chars(struct uart_port *port)
{
	struct tty_struct *tty = port->info->port.tty;
	unsigned int status, ch, flg;

	/* read Rx status */
	status = uart_read(port, UART_RXSTAT);

	while (status & BCM_RXSTAT_RDA) {
		/* Read pending char and clear status */
		ch = uart_read(port, UART_RXDATA) & 0xFF;

		port->icount.rx++;

		flg = TTY_NORMAL;

		/*
		 * note that the error handling code is
		 * out of the main execution path
		 */
		if (unlikely(status & BCM_RXSTAT_ERR)) {
			if (ch == '\0' && (status & BCM_RXSTAT_ERR) == BCM_RXSTAT_FE) {
				status &= ~(BCM_RXSTAT_ERR);	/* ignore side-effect */
				port->icount.brk++;
				if (uart_handle_break(port))
					goto ignore_char;
			}
			if (status & BCM_RXSTAT_PE)
				port->icount.parity++;
			if (status & BCM_RXSTAT_FE)
				port->icount.frame++;
			if (status & BCM_RXSTAT_OVRN)
				port->icount.overrun++;

			status &= port->read_status_mask;

			if (ch == '\0' && (status & BCM_RXSTAT_ERR) == BCM_RXSTAT_ERR)
				flg = TTY_BREAK;
			else if (status & BCM_RXSTAT_PE)
				flg = TTY_PARITY;
			else if (status & BCM_RXSTAT_FE)
				flg = TTY_FRAME;
		}

		if (uart_handle_sysrq_char(port, ch))
			goto ignore_char;

		uart_insert_char(port, status, BCM_RXSTAT_OVRN, ch, flg);

	ignore_char:
		status = uart_read(port, UART_RXSTAT);
	}

	tty_flip_buffer_push(tty);
}

/*
 * Transmit characters (called from interrupt handler)
 */
static void bcm97xxx_tx_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;
	int count;

	if (port->x_char) {
		uart_write(port, UART_TXDATA, port->x_char);
		port->icount.tx++;
		port->x_char = 0;
		return;
	}
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		bcm97xxx_stop_tx(port);
		return;
	}

	/* bcm97xxx_tx_chars() is called in response to the Tx empty interrupt,
	 * which is asserted when BCM_TXSTAT_TIE is set and BCM_TXSTAT_TDRE is
	 * asserted.
	 * There is a 32-byte Tx FIFO on this type of BCM UART, so in response
	 * to a TX empty interrupt, we can write up to 32 chars into UART_TXDATA.
	 */
	count = port->fifosize;
	do {
		uart_write(port, UART_TXDATA, xmit->buf[xmit->tail]);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		if (uart_circ_empty(xmit))
			break;
	} while (--count);

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		bcm97xxx_stop_tx(port);
}

/*
 * Interrupt handler
 */
static irqreturn_t bcm97xxx_interrupt(int irq, void *dev_id)
{
	struct uart_port *port = dev_id;

	/* Rx interrupt */
	if (uart_read(port, UART_RXSTAT) & BCM_RXSTAT_RIE)
		bcm97xxx_rx_chars(port);

	/* Tx interrupt */
	if (uart_read(port, UART_TXSTAT) & BCM_TXSTAT_TIE)
		bcm97xxx_tx_chars(port);

	return IRQ_HANDLED;
}

/*
 * Perform initialization and enable port for reception
 */
static int bcm97xxx_startup(struct uart_port *port)
{
	int retval;
	uint32_t rxstat, txstat, control;

	rxstat = uart_read(port, UART_RXSTAT);
	txstat = uart_read(port, UART_TXSTAT);

	/* disable Rx and Tx interrupts */
	uart_write(port, UART_RXSTAT, rxstat &= ~(BCM_RXSTAT_RIE));
	uart_write(port, UART_TXSTAT, txstat &= ~(BCM_TXSTAT_TIE));

	/*
	 * Allocate the IRQ
	 */
	retval = request_irq(port->irq, bcm97xxx_interrupt, 0, "bcm3250_serial", port);
	if (retval) {
		printk(KERN_ERR "bcm3250_serial: bcm97xxx_startup - Can't get irq\n");
		return retval;
	}

	/*
	 * If there is a specific "open" function (to register
	 * control line interrupts)
	 */
	if (bcm97xxx_open) {
		retval = bcm97xxx_open(port);
		if (retval) {
			free_irq(port->irq, port);
			return retval;
		}
	}

	/*
	 * Finally, enable the serial port
	 */
	control = uart_read(port, UART_CONTROL);
	uart_write(port, UART_CONTROL, control | BCM_CONTROL_TXE | BCM_CONTROL_RXE); /* enable rx and tx */

	rxstat = uart_read(port, UART_RXSTAT);
	uart_write(port, UART_RXSTAT, rxstat | BCM_RXSTAT_RIE); /* enable Rx interrupt */

	txstat = uart_read(port, UART_TXSTAT);
	uart_write(port, UART_TXSTAT, txstat | BCM_TXSTAT_TIE); /* enable Tx interrupt */

	return 0;
}

/*
 * Disable the port
 */
static void bcm97xxx_shutdown(struct uart_port *port)
{
	/*
	 * Disable all interrupts, port and break condition.
	 */
	uint32_t rxstat = uart_read(port, UART_RXSTAT);
	uint32_t txstat = uart_read(port, UART_TXSTAT);

	/* disable Rx and Tx interrupts */
	uart_write(port, UART_RXSTAT, rxstat &= ~(BCM_RXSTAT_RIE));
	uart_write(port, UART_TXSTAT, txstat &= ~(BCM_TXSTAT_TIE));

	/*
	 * Free the interrupt
	 */
	free_irq(port->irq, port);

	/*
	 * If there is a specific "close" function (to unregister
	 * control line interrupts)
	 */
	if (bcm97xxx_close)
		bcm97xxx_close(port);
}

/*
 * Change the port parameters
 */
static void bcm97xxx_set_termios(struct uart_port *port,
	struct ktermios *termios, struct ktermios *old)
{
	unsigned long flags;
	unsigned int mode, baud, quot;
	uint32_t control, rxstat, txstat;

	baud = uart_get_baud_rate(port, termios, old, 0, port->uartclk/16);
	quot = uart_get_divisor(port, baud) - 1;

	/* Get current mode register */
	mode = uart_read(port, UART_CONTROL) & BCM_CONTROL_MASK;

	/* byte size */
	if ((termios->c_cflag & CSIZE) == CS7)
		mode &= ~(BCM_CONTROL_BITM8);
	else
		mode |=  (BCM_CONTROL_BITM8);

	/* stop bits */
	if (termios->c_cflag & CSTOPB)
		mode |=  (BCM_CONTROL_STOP2);
	else
		mode &= ~(BCM_CONTROL_STOP2);

	/* parity */
	if (termios->c_cflag & PARENB) {
		if (termios->c_cflag & PARODD)
			mode |=  (BCM_CONTROL_PODD);
		else
			mode &= ~(BCM_CONTROL_PODD);
	}
	else
		mode &= ~(BCM_CONTROL_PAREN);

	spin_lock_irqsave(&port->lock, flags);

	port->read_status_mask = BCM_RXSTAT_OVRN;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= (BCM_RXSTAT_PE | BCM_RXSTAT_FE);
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= (BCM_RXSTAT_PE | BCM_RXSTAT_FE | BCM_RXSTAT_OVRN );

	/*
	 * Characters to ignore
	 */
	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= (BCM_RXSTAT_PE | BCM_RXSTAT_FE);
	if (termios->c_iflag & IGNBRK) {
		port->ignore_status_mask |= (BCM_RXSTAT_PE | BCM_RXSTAT_FE | BCM_RXSTAT_OVRN );
		/*
		 * If we're ignoring parity and break indicators,
		 * ignore overruns too (for real raw support).
		 */
		if (termios->c_iflag & IGNPAR)
			port->ignore_status_mask |= BCM_RXSTAT_OVRN ;
	}

	/* TODO: Ignore all characters if CREAD is set. */

	/* update the per-port timeout */
	uart_update_timeout(port, termios->c_cflag, baud);

	/* disable Rx and Tx interrupts */
	rxstat = uart_read(port, UART_RXSTAT);
	txstat = uart_read(port, UART_TXSTAT);
	uart_write(port, UART_RXSTAT, rxstat &= ~(BCM_RXSTAT_RIE));
	uart_write(port, UART_TXSTAT, txstat &= ~(BCM_TXSTAT_TIE));

	/* drain transmitter */
	while ((uart_read(port, UART_TXSTAT) & (BCM_TXSTAT_IDLE | BCM_TXSTAT_TDRE)) != (BCM_TXSTAT_IDLE | BCM_TXSTAT_TDRE))
		barrier();

	/* disable receiver and transmitter */
	control = uart_read(port, UART_CONTROL);
	control &= ~(BCM_CONTROL_RXE|BCM_CONTROL_TXE);
	uart_write(port, UART_CONTROL, control);

	/* set the parity, stop bits and data size */
	uart_write(port, UART_CONTROL, mode);

#if ! defined(CONFIG_BRCM_IKOS)
	/* set the baud rate */
	uart_write(port, UART_BAUDHI, (quot >> 8) & BCM_BAUDHI_BRR_MASK );
	uart_write(port, UART_BAUDLO, (quot & BCM_BAUDLO_BRR_MASK));
#endif

	/* restore interrupts */
	uart_write(port, UART_RXSTAT, rxstat | (BCM_RXSTAT_RIE));
	uart_write(port, UART_TXSTAT, txstat | (BCM_TXSTAT_TIE));

	spin_unlock_irqrestore(&port->lock, flags);
}

/*
 * Return string describing the specified port
 */
static const char *bcm97xxx_type(struct uart_port *port)
{
	return (port->type == PORT_BCMUPG) ? "BCM3250_UPG" : NULL;
}

/*
 * Release the memory region(s) being used by 'port'.
 */
static void bcm97xxx_release_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
	int size = pdev->resource[0].end - pdev->resource[0].start + 1;

	release_mem_region(port->mapbase, size);

	if (port->flags & UPF_IOREMAP) {
		iounmap(port->membase);
		port->membase = NULL;
	}
}

/*
 * Request the memory region(s) being used by 'port'.
 */
static int bcm97xxx_request_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
	int size = pdev->resource[0].end - pdev->resource[0].start + 1;

	if (!request_mem_region(port->mapbase, size, "bcm3250_serial"))
		return -EBUSY;

	if (port->flags & UPF_IOREMAP) {
		port->membase = ioremap(port->mapbase, size);
		if (port->membase == NULL) {
			release_mem_region(port->mapbase, size);
			return -ENOMEM;
		}
	}

	return 0;
}

/*
 * Configure/autoconfigure the port.
 */
static void bcm97xxx_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE) {
		if (bcm97xxx_request_port(port) == 0) {
			port->type = PORT_BCMUPG;
		}
	}
}

/*
 * Verify the new serial_struct (for TIOCSSERIAL).
 */
static int bcm97xxx_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	int ret = 0;
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_BCMUPG)
		ret = -EINVAL;
	if (port->irq != ser->irq)
		ret = -EINVAL;
	if (ser->io_type != SERIAL_IO_MEM)
		ret = -EINVAL;
	if (port->uartclk / 16 != ser->baud_base)
		ret = -EINVAL;
	if ((void *)port->mapbase != ser->iomem_base)
		ret = -EINVAL;
	if (port->iobase != ser->port)
		ret = -EINVAL;
	if (ser->hub6 != 0)
		ret = -EINVAL;
	return ret;
}

static struct uart_ops bcm97xxx_pops = {
	.tx_empty       = bcm97xxx_tx_empty,
	.set_mctrl      = bcm97xxx_set_mctrl,
	.get_mctrl      = bcm97xxx_get_mctrl,
	.stop_tx        = bcm97xxx_stop_tx,
	.start_tx       = bcm97xxx_start_tx,
	.stop_rx        = bcm97xxx_stop_rx,
	.enable_ms      = bcm97xxx_enable_ms,
	.break_ctl      = bcm97xxx_break_ctl,
	.startup        = bcm97xxx_startup,
	.shutdown       = bcm97xxx_shutdown,
	.set_termios    = bcm97xxx_set_termios,
	.type           = bcm97xxx_type,
	.release_port   = bcm97xxx_release_port,
	.request_port   = bcm97xxx_request_port,
	.config_port    = bcm97xxx_config_port,
	.verify_port    = bcm97xxx_verify_port,
};

/*
 * Configure the port from the platform device resource info.
 */
static void bcm97xxx_init_port(struct uart_port *port, struct platform_device *pdev)
{
	port->iotype    = UPIO_MEM;
	port->flags     = UPF_BOOT_AUTOCONF;
	port->ops       = &bcm97xxx_pops;
	port->fifosize  = 32;
	port->line      = pdev->id;
	port->dev       = &pdev->dev;
	port->uartclk   = 27000000;
	port->regshift  = 0;

	port->mapbase   = pdev->resource[0].start;
	port->irq       = pdev->resource[1].start;
	port->flags    |= UPF_IOREMAP;
	port->membase   = NULL;
}

#ifdef CONFIG_SERIAL_BCM3250_CONSOLE
static void bcm97xxx_console_putchar(struct uart_port *port, int ch)
{
	while (!(uart_read(port, UART_TXSTAT) & BCM_TXSTAT_TDRE))
		barrier();
	uart_write(port, UART_TXDATA, ch);
}

/*
 * Interrupts are disabled on entering
 */
static void bcm97xxx_console_write(struct console *co, const char *s, u_int count)
{
	struct uart_port *port = &bcm97xxx_ports[co->index];
	unsigned int txstat;

	txstat = uart_read(port, UART_TXSTAT);

	/* disable Tx interrupt */
	uart_write(port, UART_TXSTAT, txstat & ~(BCM_TXSTAT_TIE));

	/* write count chars to the port */
	uart_console_write(port, s, count, bcm97xxx_console_putchar);

	/* wait for transmitter to become empty */
	while (!(uart_read(port, UART_TXSTAT) & BCM_TXSTAT_TDRE))
		barrier();

	/* restore Tx interrupt */
	uart_write(port, UART_TXSTAT, txstat);
}

/*
 * If the port was already initialised (eg, by a boot loader), try to determine
 * the current setup.
 */
static void __init bcm97xxx_console_get_options(struct uart_port *port, int *baud, int *parity, int *bits)
{
	unsigned int mode, baudlo, baudhi;

	/* Get current mode register */
	mode = uart_read(port, UART_CONTROL) & BCM_CONTROL_MASK;

	if (mode & BCM_CONTROL_BITM8)
		*bits = 8;
	else
		*bits = 7;

	if (mode & BCM_CONTROL_PODD)
		*parity = 'o';
	else
		*parity = 'e';

	baudhi = uart_read(port, UART_BAUDHI) & (BCM_BAUDHI_BRR1 | BCM_BAUDHI_BRR_MASK) << 8;
	baudlo = uart_read(port, UART_BAUDLO) & BCM_BAUDLO_BRR_MASK;

	*baud = port->uartclk / (16 * (baudhi | baudlo));
}

static int __init bcm97xxx_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud   = 115200;
	int bits   = 8;
	int parity = 'n';
	int flow   = 'n';
	uint32_t control, txstat, rxstat;

	port = &bcm97xxx_ports[co->index];

	if (port->membase == 0)
		return -ENODEV;

	control = uart_read(port, UART_CONTROL);
	txstat  = uart_read(port, UART_TXSTAT);
	rxstat  = uart_read(port, UART_RXSTAT);

	/* disable Rx and Tx interrupts */
	uart_write(port, UART_RXSTAT, rxstat & ~(BCM_RXSTAT_RIE));
	uart_write(port, UART_TXSTAT, txstat & ~(BCM_TXSTAT_TIE));

	/* enable rx and tx */
	uart_write(port, UART_CONTROL, control | BCM_CONTROL_TXE | BCM_CONTROL_RXE);

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	else
		bcm97xxx_console_get_options(port, &baud, &parity, &bits);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct uart_driver bcm97xxx_uart;

static struct console bcm97xxx_console = {
	.name   = BCM97XXX_DEVICENAME,
	.write  = bcm97xxx_console_write,
	.device = uart_console_device,
	.setup  = bcm97xxx_console_setup,
	.flags  = CON_PRINTBUFFER,
	.index  = -1,
	.data   = &bcm97xxx_uart,
};

#define BCM97XXX_CONSOLE_DEVICE	&bcm97xxx_console

static int __init bcm97xxx_console_init(void)
{
	register_console(&bcm97xxx_console);
	return(0);
}
console_initcall(bcm97xxx_console_init);

#else
#define BCM97XXX_CONSOLE_DEVICE	NULL
#endif

static struct uart_driver bcm97xxx_uart = {
	.owner          = THIS_MODULE,
	.driver_name    = "bcm3250_serial",
	.dev_name       = BCM97XXX_DEVICENAME,
	.major          = SERIAL_BCM97XXX_MAJOR,
	.minor          = MINOR_START,
	.nr             = UART_NR,
	.cons           = BCM97XXX_CONSOLE_DEVICE,
};

static int bcm97xxx_uart_probe(struct platform_device *pdev)
{
	struct uart_port *port;
	int ret;

	port = &bcm97xxx_ports[pdev->id];
	bcm97xxx_init_port(port, pdev);

	ret = uart_add_one_port(&bcm97xxx_uart, port);
	if (!ret)
	{
		device_init_wakeup(&pdev->dev, 1);
		platform_set_drvdata(pdev, port);
	}

	return ret;
}

static int bcm97xxx_uart_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (port)
		uart_remove_one_port(&bcm97xxx_uart, port);

	return 0;
}

static int bcm97xxx_uart_suspend(struct platform_device *dev, pm_message_t state)
{
	struct uart_port *port = platform_get_drvdata(dev);

	if (port)
		uart_suspend_port(&bcm97xxx_uart, port);

	return 0;
}

static int bcm97xxx_uart_resume(struct platform_device *dev)
{
	struct uart_port *port = platform_get_drvdata(dev);

	if (port)
		uart_resume_port(&bcm97xxx_uart, port);

	return 0;
}

static struct platform_driver bcm97xxx_uart_driver = {
	.probe   = bcm97xxx_uart_probe,
	.remove  = bcm97xxx_uart_remove,
	.suspend = bcm97xxx_uart_suspend,
	.resume  = bcm97xxx_uart_resume,
	.driver  = {
		.name  = "bcm3250_serial",
		.owner = THIS_MODULE,
	},
};

static int __init bcm97xxx_serial_init(void)
{
	int ret;

	printk(KERN_INFO "Serial: bcm97xxx driver $Revision: 1.1.2.2 $\n");

	ret = uart_register_driver(&bcm97xxx_uart);
	if (ret == 0)
	{
		ret = platform_driver_register(&bcm97xxx_uart_driver);
		if (ret)
			uart_unregister_driver(&bcm97xxx_uart);
	}

	return ret;
}

static void __exit bcm97xxx_serial_exit(void)
{
	platform_driver_unregister(&bcm97xxx_uart_driver);
	uart_unregister_driver(&bcm97xxx_uart);
}

module_init(bcm97xxx_serial_init);
module_exit(bcm97xxx_serial_exit);

MODULE_AUTHOR("Pace Microtechnology plc");
MODULE_DESCRIPTION("Broadcom 97XXX UPG DUART driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_CHARDEV_MAJOR(SERIAL_BCM97XXX_MAJOR);
