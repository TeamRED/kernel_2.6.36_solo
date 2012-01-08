/******************************************************************************
 *
 * Copyright 2008 Pace plc
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/io.h>

#define uart_read(base, offset)          __raw_readl((void*)((base)+(offset)))
#define uart_write(base, offset, value)  __raw_writel((value), (void*)((base)+(offset)))

#define DEFAULT_BAUDRATE         115200

#define UART_CONTROL            0x0C
#define  BCM_CONTROL_BITM8       (1<<4)
#define  BCM_CONTROL_TXE         (1<<2)
#define UART_BAUDHI             0x10
#define UART_BAUDLO             0x14
#define UART_TXSTAT             0x18
#define  BCM_TXSTAT_TDRE        (1<<0)
#define UART_TXDATA             0x1c

static void uart_putchar(char c, unsigned long base)
{
	while (!(uart_read(base, UART_TXSTAT) & BCM_TXSTAT_TDRE)) {};

	uart_write(base, UART_TXDATA, c);
}

static void cons_write(struct console *c, const char *s, unsigned n)
{
	unsigned long base = (unsigned long)c->data;

	while (n-- && *s)
	{
		if ('\n' == *s) uart_putchar('\r', base);

		uart_putchar(*s++, base);
	}
}

static struct console cons_bcm7xxx =
{
	.name	= "uart",
	.write	= cons_write,
	.flags	= CON_PRINTBUFFER | CON_BOOT,
	.index	= -1,
};

void __init bcm3250_early_console(unsigned long base)
{
	unsigned long baud;

	/* base is passed in as PA */
	base = KSEG1ADDR(base);

	baud = 27000000 / (DEFAULT_BAUDRATE*16);
	cons_bcm7xxx.data = (void *)base;

	/* set baud rate */
	uart_write(base, UART_BAUDLO, (baud & 0xFF));
	uart_write(base, UART_BAUDHI, ((baud >> 8) & 0xFF));

	/* Enable the UART, 8N1, Tx & Rx enabled */
	uart_write(base, UART_CONTROL, (BCM_CONTROL_BITM8|BCM_CONTROL_TXE));

	register_console(&cons_bcm7xxx);

	printk("bcm7xxx: early console registered\n");
}

void __init disable_early_printk(void)
{
	unregister_console(&cons_bcm7xxx);
}
