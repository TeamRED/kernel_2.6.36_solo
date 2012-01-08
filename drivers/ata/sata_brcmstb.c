/*
 *  sata_brcmstb.c - ServerWorks / Apple K2 SATA
 *
 *  Maintained by: Benjamin Herrenschmidt <benh@kernel.crashing.org> and
 *		   Jeff Garzik <jgarzik@pobox.com>
 *  		    Please ALWAYS copy linux-ide@vger.kernel.org
 *		    on emails.
 *
 *  Copyright 2003 Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *
 *  Bits from Jeff Garzik, Copyright RedHat, Inc.
 *
 *  This driver probably works with non-Apple versions of the
 *  Broadcom chipset...
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 *  libata documentation is available via 'make {ps|pdf}docs',
 *  as Documentation/DocBook/libata.*
 *
 *  Hardware documentation available under NDA.
 *
 *  BRCM NOTE:
 *  2.6.29.1 version was ported from 2.6.18-6.6a.  PMP support is disabled.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi.h>
#include <linux/libata.h>

#ifdef CONFIG_PPC_OF
#include <asm/prom.h>
#include <asm/pci-bridge.h>
#endif /* CONFIG_PPC_OF */

#define DRV_NAME	"sata_brcmstb"
#define DRV_VERSION	"4.0"

enum {
	/* ap->flags bits */
	K2_FLAG_SATA_8_PORTS		= (1 << 24),
	K2_FLAG_NO_ATAPI_DMA		= (1 << 25),
	K2_FLAG_BAR_POS_3			= (1 << 26),
	K2_FLAG_BRCM_SATA2		= (1 << 27),

	/* Taskfile registers offsets */
	K2_SATA_TF_CMD_OFFSET		= 0x00,
	K2_SATA_TF_DATA_OFFSET		= 0x00,
	K2_SATA_TF_ERROR_OFFSET		= 0x04,
	K2_SATA_TF_NSECT_OFFSET		= 0x08,
	K2_SATA_TF_LBAL_OFFSET		= 0x0c,
	K2_SATA_TF_LBAM_OFFSET		= 0x10,
	K2_SATA_TF_LBAH_OFFSET		= 0x14,
	K2_SATA_TF_DEVICE_OFFSET	= 0x18,
	K2_SATA_TF_CMDSTAT_OFFSET      	= 0x1c,
	K2_SATA_TF_CTL_OFFSET		= 0x20,

	/* DMA base */
	K2_SATA_DMA_CMD_OFFSET		= 0x30,

	/* SCRs base */
	K2_SATA_SCR_STATUS_OFFSET	= 0x40,
	K2_SATA_SCR_ERROR_OFFSET	= 0x44,
	K2_SATA_SCR_CONTROL_OFFSET	= 0x48,

	/* Others */
	K2_SATA_SICR1_OFFSET		= 0x80,
	K2_SATA_SICR2_OFFSET		= 0x84,
	K2_SATA_SIM_OFFSET		= 0x88,
	K2_SATA_MDIO_OFFSET		= 0x8c,	/* SATA MDIO access register */
	K2_SATA_QCR_OFFSET		= 0xb0,	/* QDMA control register */
	K2_SATA_QSR_OFFSET		= 0xb8,	/* QDMA status register */

	K2_SATA_E0_OFFSET		= 0xe0,
	K2_SATA_F0_OFFSET		= 0xf0,

	/* Port stride */
	K2_SATA_PORT_OFFSET		= 0x100,

	chip_sata1			= 0,
	chip_sata2			= 1,
};

static u8 k2_stat_check_status(struct ata_port *ap);


static int k2_sata_check_atapi_dma(struct ata_queued_cmd *qc)
{
	u8 cmnd = qc->scsicmd->cmnd[0];

	if (qc->ap->flags & K2_FLAG_NO_ATAPI_DMA)
		return -1;	/* ATAPI DMA not supported */
	else {
		switch (cmnd) {
		case READ_10:
		case READ_12:
		case READ_16:
		case WRITE_10:
		case WRITE_12:
		case WRITE_16:
			return 0;

		default:
			return -1;
		}

	}
}

#ifdef CONFIG_BRCMSTB
#include <asm/brcmstb/brcmstb.h>

static int ssc = 0;	/* aka: bcmssc, gSataInterpolation */
static int s2 = 0;	/* aka: bcmsata2, gSata2_3Gbps */

module_param(ssc, int, 0444);
module_param(s2, int, 0444);

#if defined(CONFIG_BRCM_PM)

#define SLEEP_FLAG(host) (long)(host->private_data)
#define SET_SLEEP_FLAG(host, val) do { \
	host->private_data = (void *)(val); \
	} while(0)

#define K2_AWAKE	0
#define K2_SLEEPING	1

static int k2_power_on(void *arg);

#define K2_POWER_ON(host) do { \
	if(SLEEP_FLAG(host) != K2_AWAKE) \
		k2_power_on(host); \
	} while(0)

static DEFINE_SPINLOCK(sleep_lock);

#else

#define K2_POWER_ON(host) do { } while(0)

#endif
// jipeng - if bcmsata2=1, but device only support SATA I, then downgrade to SATA I and reset SATA core
#define	AUTO_NEG_SPEED

static unsigned int new_speed_mask = 0;

#ifdef	SATA_SVW_BRCM_WA
extern int dma_write_wa_needed;
#endif

#define PORT_BASE(x, y)		((x) + (K2_SATA_PORT_OFFSET * (unsigned)(y)))
#define PORT_MMIO(x)		PORT_BASE((void __iomem *) \
					((x)->host->iomap), \
					(x)->port_no)

/*
 * Extra init functions for stblinux platforms
 */

#define WRITE_CMD			1
#define READ_CMD			2
#define CMD_DONE			(1 << 15)
#define SATA_MMIO			0x24

// 1. port is SATA port ( 0 or 1)
// 2. reg is the address of the MDIO register ( see spec )
// 3. MMIO_BASE_ADDR  is MMIO base address from SATA PCI configuration
// registers addr 24-27
static uint16_t mdio_read_reg(void __iomem *mmio_base, int port,
	int reg)
{
	void __iomem *mdio = mmio_base + K2_SATA_MDIO_OFFSET;
	volatile unsigned int pSel = 1 << port;
	uint32_t cmd  = WRITE_CMD;

	if( reg > 0x13 )
	return( -1 );

	//Select Port
	writel(pSel<<16 | (cmd << 13) | 7, mdio);             //using dev_addr 0
	while( !(readl(mdio) & CMD_DONE) )
	udelay( 1);     //wait

	writel((READ_CMD << 13) + reg, mdio);
	while( !(readl(mdio) & CMD_DONE) )
	udelay( 1 );    //wait

	return( readl(mdio) >> 16 );
}

static void mdio_write_reg(void __iomem *mmio_base, int port,
	int reg, uint16_t val )
{
	void __iomem *mdio = mmio_base + K2_SATA_MDIO_OFFSET;
	volatile unsigned int pSel = 1 << port;
	uint32_t cmd  = WRITE_CMD;

	if( reg > 0x13 )
	return;

	//Select Port
	writel(pSel<<16 | (cmd << 13) | 7, mdio);             //using dev_addr 0
	while( !(readl(mdio) & CMD_DONE) )
	udelay( 1);     //wait

	writel((val << 16) + (WRITE_CMD << 13) + reg, mdio);          //using dev_addr 0
	while( !(readl(mdio) & CMD_DONE) )
	udelay( 1 );    //wait
}

static void DisablePHY(void __iomem *mmio_base, int port)
{
	uint32_t *pScr2 = PORT_BASE(mmio_base, port) + K2_SATA_SCR_CONTROL_OFFSET;
	writel(1, pScr2);
}

static void EnablePHY(void __iomem *mmio_base, int port)
{
	uint32_t *pScr2 = PORT_BASE(mmio_base, port) + K2_SATA_SCR_CONTROL_OFFSET;
	writel(0, pScr2);
}

static void bcm_sg_workaround(void __iomem *mmio_base, int port)
{
	int tmp16;

	DisablePHY(mmio_base, port);

	/*
	 * Do Interpolation when
	  * spread spectrucm clocking (SSC) is NOT enabled. But, the code must
	  * be used for a system with SSC-enabled drive.
	  * ssc is not zero when the argument bcmssc=1 is specified
	  */

	if (ssc) {
		tmp16 = mdio_read_reg(mmio_base, port, 9);
		mdio_write_reg(mmio_base, port, 9, tmp16 | 1); //Bump up interpolation
	}

	//Do analog reset
	tmp16 = mdio_read_reg(mmio_base, port,4);
	tmp16 |= 8;
	mdio_write_reg(mmio_base, port,4,tmp16);

	udelay( 1000 );      //wait 1 ms
	tmp16 &= 0xFFF7;
	mdio_write_reg(mmio_base, port,4,tmp16 ); // Take reset out
	udelay( 1000 );      //wait

	//Enable PHY
	EnablePHY(mmio_base, port);
}

// Fix to turn on 3Gbps

//This routine change (lower) bandwidth of SATA PLL to lower jitter from main internal // ref clk in attempt to use on chip refclock.
static void brcm_SetPllTxRxCtrl(void __iomem *mmio_base, int port)
{
	uint16_t tmp16;

	//Change Tx control
	mdio_write_reg(mmio_base, port, 0xa, 0x0260);
	mdio_write_reg(mmio_base, port, 0x11, 0x0a10);
	//Change Rx control
	tmp16 = mdio_read_reg(mmio_base, port, 0xF);
	tmp16 &= 0xc000;
	tmp16 |= 0x1000;
	mdio_write_reg(mmio_base, port, 0xF, tmp16);
}

static void brcm_TunePLL(void __iomem *mmio_base, int port)
{
	volatile uint16_t tmp;
	int i;

	//program VCO step bit [12:10] start with 111
	mdio_write_reg(mmio_base, port, 0x13, 0x1c00);

	udelay(100000);

	//start pll tuner
	mdio_write_reg(mmio_base, port, 0x13, 0x1e00); //

	udelay(10000); // wait

	//check lock bit
	tmp = mdio_read_reg(mmio_base, port,0x7);

	for(i = 0; i < 10000; i++) {
		tmp = mdio_read_reg(mmio_base, port, 0x7);
		if((tmp & 0x8000) == 0x8000)
			return;
		udelay(1);
	}
	DPRINTK("PLL did not lock\n");
}

static void brcm_AnalogReset(void __iomem *mmio_base, int port)
{
	//do analog reset
	mdio_write_reg(mmio_base, port, 0x4, 8);
	udelay(10000); // wait
	mdio_write_reg(mmio_base, port, 0x4, 0);

	bcm_sg_workaround(mmio_base,port);
}

static void brcm_InitSata_1_5Gb(void __iomem *mmio_base, int port)
{
	volatile uint16_t tmp;
	volatile uint32_t tmp32;
	void __iomem *port_mmio;

	port_mmio = PORT_BASE(mmio_base, port);

	//reset deskew TX FIFO
	//1. select port
	mdio_write_reg(mmio_base, port, 7, 1<<port);
	//toggle reset bit
	mdio_write_reg(mmio_base, port, 0xd, 0x4800);
	udelay(10000); // wait
	mdio_write_reg(mmio_base, port, 0xd, 0x0800);
	udelay(10000); // wait

	//Enable low speed (1.5G) mode
	tmp = mdio_read_reg(mmio_base, port, 8);
	mdio_write_reg(mmio_base, port, 8, tmp | 0x10);

	//Enable lock monitor.. if not set the lock bit is not updated
	tmp = mdio_read_reg(mmio_base, port, 0x13);

	mdio_write_reg(mmio_base, port, 0x13, tmp|2);
	udelay(10000);

#ifdef	AUTO_NEG_SPEED
	// disable 3G feature
	tmp32 = readl(port_mmio + K2_SATA_F0_OFFSET);
	writel(tmp32 & 0xfffffbff, port_mmio + K2_SATA_F0_OFFSET);
	udelay(10000);
#endif

	//enable 4G addressing support
	tmp32 = readl(port_mmio + K2_SATA_SICR2_OFFSET);
	writel(tmp32 | 0x20009400, port_mmio + K2_SATA_SICR2_OFFSET);

	tmp32 = readl(port_mmio + K2_SATA_SICR1_OFFSET);
	tmp32 &= 0xffff0000;
	writel(tmp32 | 0x00000f10, port_mmio + K2_SATA_SICR1_OFFSET);

	//Clean up the fifo -- per Ajay
	tmp32 = readl(port_mmio + K2_SATA_E0_OFFSET);
	writel(tmp32 | 2, port_mmio + K2_SATA_E0_OFFSET);

	brcm_SetPllTxRxCtrl(mmio_base, port);

	if(!port)
	{
		//Lower BW
#ifdef CONFIG_BRCM_SATA_75MHZ_PLL
		/* use 75Mhz PLL clock */
		mdio_write_reg(mmio_base, port, 0, 0x2004);
#else
		/* use 100Mhz PLL clock */
		mdio_write_reg(mmio_base, port, 0, 0x1404);
#endif
		brcm_TunePLL(mmio_base, port);
		brcm_AnalogReset(mmio_base, port);
		udelay(10000); // wait
	}

	writel(0, port_mmio + K2_SATA_SCR_CONTROL_OFFSET);
}

static void brcm_InitSata2_3Gb(void __iomem *mmio_base, int port)
{
	volatile uint16_t tmp;
	volatile uint32_t tmp32;
	void __iomem *port_base;

	port_base = PORT_BASE(mmio_base, port);

	//reset deskew TX FIFO
	//1. select port
	mdio_write_reg(mmio_base, port, 7, 1<<port);
	//toggle reset bit
	mdio_write_reg(mmio_base, port, 0xd, 0x4800);
	udelay(10000); // wait
	mdio_write_reg(mmio_base, port, 0xd, 0x0800);
	udelay(10000); // wait

	//run at 3G mode
	tmp = mdio_read_reg(mmio_base, port, 8);
	mdio_write_reg(mmio_base, port, 8, tmp & ~(0x10));
	udelay(10000); // wait

	//Enable lock monitor.. if not set the lock bit is not updated
	tmp = mdio_read_reg(mmio_base, port, 0x13);
	mdio_write_reg(mmio_base, port, 0x13, tmp|2);

	//Enable 3Gb interface
	tmp32 = readl(port_base + K2_SATA_F0_OFFSET);
	writel(tmp32 | 0x10500, port_base + K2_SATA_F0_OFFSET);
	udelay(10000); // wait

	//enable 4G addressing support
	tmp32 = readl(port_base + K2_SATA_SICR2_OFFSET);
	writel(tmp32 | 0x20009400, port_base + K2_SATA_SICR2_OFFSET);

	tmp32 = readl(port_base + K2_SATA_SICR1_OFFSET);
	tmp32 &= 0xffff0000;
	writel(tmp32 | 0x00000f10, port_base + K2_SATA_SICR1_OFFSET);

	//Clean up the fifo -- per Ajay
	tmp32 = readl(port_base + K2_SATA_E0_OFFSET);
	writel(tmp32|2, port_base + K2_SATA_E0_OFFSET);

	//Tweak PLL, Tx, and Rx
	brcm_SetPllTxRxCtrl(mmio_base, port);

	if(!port)
	{
		//Lower BW
#ifdef BRCM_75MHZ_SATA_PLL
		/* use 75Mhz PLL clock */
		mdio_write_reg(mmio_base, port, 0, 0x2004);
#else
		/* use 100Mhz PLL clock */
		mdio_write_reg(mmio_base, port, 0, 0x1404);
#endif
		brcm_TunePLL(mmio_base, port);
		brcm_AnalogReset(mmio_base, port);
		udelay(10000); // wait
	}

	writel(0, port_base + K2_SATA_SCR_CONTROL_OFFSET);
}

/* Kernel argument bcmsata2=0|1 */
static inline void brcm_initsata2(void __iomem *mmio_base, int num_ports)
{
	int port;
	unsigned int first=1;

retry_brcm_initsata2:
	for (port=0; port < num_ports; port++)
	writel(1, (void *)(mmio_base + port*K2_SATA_PORT_OFFSET + K2_SATA_SCR_CONTROL_OFFSET));

	for (port=0; port < num_ports; port++) {
		/*
		 * Turn on 3Gbps if bcmsata2=1 is specified as kernel argument
		 * during bootup
		 */
		if(s2) {
			if(new_speed_mask == 0)
				brcm_InitSata2_3Gb(mmio_base, port);
			else if(new_speed_mask & (1<<port))
				brcm_InitSata_1_5Gb(mmio_base, port);
		}
		else
			brcm_InitSata_1_5Gb(mmio_base, port);
	}

#ifdef  AUTO_NEG_SPEED
	if(s2 && first)
	{
		int port;
		msleep(10);

		for (port=0; port < num_ports; port++)
		{
			unsigned int status = readl((void *)(mmio_base + port*K2_SATA_PORT_OFFSET + K2_SATA_SCR_STATUS_OFFSET));
			if( (status & 0xf) >= 0x5 && (status & 0xf0) == 0 && (status & 0xf00) == 0 )
			new_speed_mask |= 1<<port;
		}
		first = 0;
		if(new_speed_mask)
		goto retry_brcm_initsata2;
	}


	new_speed_mask = 0;
#endif
}

static void bcm97xxx_sata_init(struct pci_dev *dev, void __iomem *mmio_base, int n_ports,
	int sata2_core)
{
	unsigned int reg;
	int port;
	void __iomem *port_base;

	/* minimum grant, to avoid Latency being reset to lower value */
	pci_write_config_byte(dev, PCI_MIN_GNT, 0x0f);

	/* SATA master latency timer */
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, 0xff);

	if (sata2_core) {
		brcm_initsata2(mmio_base, n_ports);
		return; /* Skip all workarounds.  Those have been fixed with 65nm */
	}

	/*
	* Before accessing the MDIO registers through pci space disable external MDIO access.
	* write MDIO register at offset 0x07 with (1 << port number) where port number starts at 0.
	* Read MDIO register at offset 0x0D into variable reg.
	- reg_0d = reg_0d | 0x04
	- Write reg_0d to MDIO register at offset 0x0D.
	*/
	for (port = 0; port < n_ports; port++) {
		// Put PHY in reset
		port_base = PORT_BASE(mmio_base, 0);
		reg = readl(port_base+K2_SATA_SCR_CONTROL_OFFSET);
		writel((reg & ~0xf) | 1, port_base+K2_SATA_SCR_CONTROL_OFFSET);

		// Choose the port and read reg 0xd
		reg = mdio_read_reg(mmio_base, port, 0xd);
		reg |= 0x4;
		mdio_write_reg(mmio_base, port, 0xd, reg);
		reg = mdio_read_reg(mmio_base, port, 0xd);

		// Re-enable PHY
		reg = readl(port_base+K2_SATA_SCR_CONTROL_OFFSET);
		writel(reg & ~0xf, port_base+K2_SATA_SCR_CONTROL_OFFSET);
	}

//PR22401: Identify Seagate drives with ST controllers.
	for (port=0; port < n_ports; port++)
		bcm_sg_workaround(mmio_base,port);
}

#if defined(CONFIG_BRCM_PM)
static int k2_power_off(void *arg)
{
	struct ata_host *host = arg;
	int i, active = 0;
	long flags;

	spin_lock_irqsave(&sleep_lock, flags);
	if(SLEEP_FLAG(host) == K2_SLEEPING) {
		spin_unlock_irqrestore(&sleep_lock, flags);
		return(-1);
	}

	for(i = 0; i < host->n_ports; i++) {
		struct ata_port *ap;
		struct ata_link *link;
		struct ata_device *dev;

		ap = host->ports[i];

		/*
		 * dev->sdev is set to NULL in ata_scsi_slave_destroy()
		 * upon hot or warm unplug.  If it is non-NULL, the device
		 * is still enabled and it is unsafe to power off the core.
		 */
		ata_for_each_link(link, ap, EDGE) {
			ata_link_for_each_dev(dev, link) {
				if(dev->sdev)
					active++;
			}
		}
	}

	if(active) {
		spin_unlock_irqrestore(&sleep_lock, flags);
		printk(KERN_INFO DRV_NAME
			": can't sleep with %d device(s) active\n", active);
		return(-1);
	} else {
		void __iomem *port_base;

		SET_SLEEP_FLAG(host, K2_SLEEPING);

		/* put all ports in Slumber mode */
		for(i = 0; i < host->n_ports; i++) {
			port_base = PORT_BASE(host->mmio_base, i);
			writel(readl(port_base + K2_SATA_SICR1_OFFSET) |
				(1 << 25), port_base + K2_SATA_SICR1_OFFSET);
			udelay(10);
			writel((readl(port_base + K2_SATA_SICR2_OFFSET) &
				~(7 << 18)) | (2 << 18),
				port_base + K2_SATA_SICR2_OFFSET);
			udelay(50);
			writel((readl(port_base + K2_SATA_SICR2_OFFSET) &
				~(7 << 18)) | (0 << 18),
				port_base + K2_SATA_SICR2_OFFSET);
		}

		disable_irq(host->irq);
		brcm_pm_sata_remove();
		spin_unlock_irqrestore(&sleep_lock, flags);

		return(0);
	}
}

static int k2_power_on(void *arg)
{
	struct ata_host *host = arg;
	void __iomem *port_base;
	int i;
	long flags;

	spin_lock_irqsave(&sleep_lock, flags);
	if(SLEEP_FLAG(host) == K2_AWAKE) {
		spin_unlock_irqrestore(&sleep_lock, flags);
		return(-1);
	}

	brcm_pm_sata_add();
	brcm_initsata2(host->mmio_base, host->n_ports);
	enable_irq(host->irq);

	/* wake up all ports */
	for(i = 0; i < host->n_ports; i++) {
		port_base = PORT_BASE(host->mmio_base, i);
		writel(readl(port_base + K2_SATA_SICR1_OFFSET) &
			~(1 << 25), port_base + K2_SATA_SICR1_OFFSET);
		udelay(10);
		writel((readl(port_base + K2_SATA_SICR2_OFFSET) &
			~(7 << 18)) | (4 << 18),
			port_base + K2_SATA_SICR2_OFFSET);
		udelay(50);
		writel((readl(port_base + K2_SATA_SICR2_OFFSET) &
			~(7 << 18)) | (0 << 18),
			port_base + K2_SATA_SICR2_OFFSET);
	}

	SET_SLEEP_FLAG(host, K2_AWAKE);
	spin_unlock_irqrestore(&sleep_lock, flags);

	return(0);
}

static void k2_sata_remove_one(struct pci_dev *pdev)
{
	struct device *dev = pci_dev_to_dev(pdev);
	struct ata_host *host = dev_get_drvdata(dev);

	K2_POWER_ON(host);
	ata_pci_remove_one(pdev);

	brcm_pm_unregister_sata();
}
#endif /* CONFIG_BRCM_PM */
#endif

static int k2_sata_scr_read(struct ata_link *link,
			    unsigned int sc_reg, u32 *val)
{
	if (sc_reg > SCR_CONTROL)
		return -EINVAL;
	*val = readl(link->ap->ioaddr.scr_addr + (sc_reg * 4));
	return 0;
}


static int k2_sata_scr_write(struct ata_link *link,
			     unsigned int sc_reg, u32 val)
{
	if (sc_reg > SCR_CONTROL)
		return -EINVAL;
	writel(val, link->ap->ioaddr.scr_addr + (sc_reg * 4));
	return 0;
}


static void k2_sata_tf_load(struct ata_port *ap, const struct ata_taskfile *tf)
{
	struct ata_ioports *ioaddr = &ap->ioaddr;
	unsigned int is_addr = tf->flags & ATA_TFLAG_ISADDR;

	if (tf->ctl != ap->last_ctl) {
		writeb(tf->ctl, ioaddr->ctl_addr);
		ap->last_ctl = tf->ctl;
		ata_wait_idle(ap);
	}
	if (is_addr && (tf->flags & ATA_TFLAG_LBA48)) {
		writew(tf->feature | (((u16)tf->hob_feature) << 8),
		       ioaddr->feature_addr);
		writew(tf->nsect | (((u16)tf->hob_nsect) << 8),
		       ioaddr->nsect_addr);
		writew(tf->lbal | (((u16)tf->hob_lbal) << 8),
		       ioaddr->lbal_addr);
		writew(tf->lbam | (((u16)tf->hob_lbam) << 8),
		       ioaddr->lbam_addr);
		writew(tf->lbah | (((u16)tf->hob_lbah) << 8),
		       ioaddr->lbah_addr);
	} else if (is_addr) {
		writew(tf->feature, ioaddr->feature_addr);
		writew(tf->nsect, ioaddr->nsect_addr);
		writew(tf->lbal, ioaddr->lbal_addr);
		writew(tf->lbam, ioaddr->lbam_addr);
		writew(tf->lbah, ioaddr->lbah_addr);
	}

	if (tf->flags & ATA_TFLAG_DEVICE)
		writeb(tf->device, ioaddr->device_addr);

	ata_wait_idle(ap);
}


static void k2_sata_tf_read(struct ata_port *ap, struct ata_taskfile *tf)
{
	struct ata_ioports *ioaddr = &ap->ioaddr;
	u16 nsect, lbal, lbam, lbah, feature;

	tf->command = k2_stat_check_status(ap);
	tf->device = readw(ioaddr->device_addr);
	feature = readw(ioaddr->error_addr);
	nsect = readw(ioaddr->nsect_addr);
	lbal = readw(ioaddr->lbal_addr);
	lbam = readw(ioaddr->lbam_addr);
	lbah = readw(ioaddr->lbah_addr);

	tf->feature = feature;
	tf->nsect = nsect;
	tf->lbal = lbal;
	tf->lbam = lbam;
	tf->lbah = lbah;

	if (tf->flags & ATA_TFLAG_LBA48) {
		tf->hob_feature = feature >> 8;
		tf->hob_nsect = nsect >> 8;
		tf->hob_lbal = lbal >> 8;
		tf->hob_lbam = lbam >> 8;
		tf->hob_lbah = lbah >> 8;
	}
}

/**
 *	k2_bmdma_setup_mmio - Set up PCI IDE BMDMA transaction (MMIO)
 *	@qc: Info associated with this ATA transaction.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */

static void k2_bmdma_setup_mmio(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;
	unsigned int rw = (qc->tf.flags & ATA_TFLAG_WRITE);
	u8 dmactl;
	void __iomem *mmio = ap->ioaddr.bmdma_addr;

	/* load PRD table addr. */
	mb();	/* make sure PRD table writes are visible to controller */
	writel(ap->bmdma_prd_dma, mmio + ATA_DMA_TABLE_OFS);

	/* specify data direction, triple-check start bit is clear */
	dmactl = readb(mmio + ATA_DMA_CMD);
	dmactl &= ~(ATA_DMA_WR | ATA_DMA_START);
	if (!rw)
		dmactl |= ATA_DMA_WR;
	writeb(dmactl, mmio + ATA_DMA_CMD);

	/* issue r/w command if this is not a ATA DMA command*/
	if (qc->tf.protocol != ATA_PROT_DMA)
		ap->ops->sff_exec_command(ap, &qc->tf);
}

/**
 *	k2_bmdma_start_mmio - Start a PCI IDE BMDMA transaction (MMIO)
 *	@qc: Info associated with this ATA transaction.
 *
 *	LOCKING:
 *	spin_lock_irqsave(host lock)
 */

static void k2_bmdma_start_mmio(struct ata_queued_cmd *qc)
{
	struct ata_port *ap = qc->ap;
	void __iomem *mmio = ap->ioaddr.bmdma_addr;
	u8 dmactl;

	/* start host DMA transaction */
	dmactl = readb(mmio + ATA_DMA_CMD);
	writeb(dmactl | ATA_DMA_START, mmio + ATA_DMA_CMD);
	/* This works around possible data corruption.

	   On certain SATA controllers that can be seen when the r/w
	   command is given to the controller before the host DMA is
	   started.

	   On a Read command, the controller would initiate the
	   command to the drive even before it sees the DMA
	   start. When there are very fast drives connected to the
	   controller, or when the data request hits in the drive
	   cache, there is the possibility that the drive returns a
	   part or all of the requested data to the controller before
	   the DMA start is issued.  In this case, the controller
	   would become confused as to what to do with the data.  In
	   the worst case when all the data is returned back to the
	   controller, the controller could hang. In other cases it
	   could return partial data returning in data
	   corruption. This problem has been seen in PPC systems and
	   can also appear on an system with very fast disks, where
	   the SATA controller is sitting behind a number of bridges,
	   and hence there is significant latency between the r/w
	   command and the start command. */
	/* issue r/w command if the access is to ATA */
	if (qc->tf.protocol == ATA_PROT_DMA)
		ap->ops->sff_exec_command(ap, &qc->tf);
}


static u8 k2_stat_check_status(struct ata_port *ap)
{
	return readl(ap->ioaddr.status_addr);
}

#ifdef CONFIG_PPC_OF
/*
 * k2_sata_proc_info
 * inout : decides on the direction of the dataflow and the meaning of the
 *	   variables
 * buffer: If inout==FALSE data is being written to it else read from it
 * *start: If inout==FALSE start of the valid data in the buffer
 * offset: If inout==FALSE offset from the beginning of the imaginary file
 *	   from which we start writing into the buffer
 * length: If inout==FALSE max number of bytes to be written into the buffer
 *	   else number of bytes in the buffer
 */
static int k2_sata_proc_info(struct Scsi_Host *shost, char *page, char **start,
			     off_t offset, int count, int inout)
{
	struct ata_port *ap;
	struct device_node *np;
	int len, index;

	/* Find  the ata_port */
	ap = ata_shost_to_port(shost);
	if (ap == NULL)
		return 0;

	/* Find the OF node for the PCI device proper */
	np = pci_device_to_OF_node(to_pci_dev(ap->host->dev));
	if (np == NULL)
		return 0;

	/* Match it to a port node */
	index = (ap == ap->host->ports[0]) ? 0 : 1;
	for (np = np->child; np != NULL; np = np->sibling) {
		const u32 *reg = of_get_property(np, "reg", NULL);
		if (!reg)
			continue;
		if (index == *reg)
			break;
	}
	if (np == NULL)
		return 0;

	len = sprintf(page, "devspec: %s\n", np->full_name);

	return len;
}
#endif /* CONFIG_PPC_OF */


static struct scsi_host_template k2_sata_sht = {
	ATA_BMDMA_SHT(DRV_NAME),
#ifdef CONFIG_PPC_OF
	.proc_info		= k2_sata_proc_info,
#endif
};


static struct ata_port_operations k2_sata_ops = {
	.inherits		= &ata_bmdma_port_ops,
	.sff_tf_load		= k2_sata_tf_load,
	.sff_tf_read		= k2_sata_tf_read,
	.sff_check_status	= k2_stat_check_status,
	.check_atapi_dma	= k2_sata_check_atapi_dma,
	.bmdma_setup		= k2_bmdma_setup_mmio,
	.bmdma_start		= k2_bmdma_start_mmio,
	.scr_read		= k2_sata_scr_read,
	.scr_write		= k2_sata_scr_write,
};

static const struct ata_port_info k2_port_info[] = {
	/* chip_sata1 */
	{
		.flags		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
				  ATA_FLAG_MMIO | K2_FLAG_NO_ATAPI_DMA,
		.pio_mask	= ATA_PIO4,
		.mwdma_mask	= ATA_MWDMA2,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &k2_sata_ops,
	},
	/* chip_sata2 */
	{
		.flags		= ATA_FLAG_SATA | ATA_FLAG_NO_LEGACY |
				  ATA_FLAG_MMIO | K2_FLAG_NO_ATAPI_DMA |
				  K2_FLAG_BRCM_SATA2,
		.pio_mask	= ATA_PIO4,
		.mwdma_mask	= ATA_MWDMA2,
		.udma_mask	= ATA_UDMA6,
		.port_ops	= &k2_sata_ops,
	},
};

static void k2_sata_setup_port(struct ata_ioports *port, void __iomem *base)
{
	port->cmd_addr		= base + K2_SATA_TF_CMD_OFFSET;
	port->data_addr		= base + K2_SATA_TF_DATA_OFFSET;
	port->feature_addr	=
	port->error_addr	= base + K2_SATA_TF_ERROR_OFFSET;
	port->nsect_addr	= base + K2_SATA_TF_NSECT_OFFSET;
	port->lbal_addr		= base + K2_SATA_TF_LBAL_OFFSET;
	port->lbam_addr		= base + K2_SATA_TF_LBAM_OFFSET;
	port->lbah_addr		= base + K2_SATA_TF_LBAH_OFFSET;
	port->device_addr	= base + K2_SATA_TF_DEVICE_OFFSET;
	port->command_addr	=
	port->status_addr	= base + K2_SATA_TF_CMDSTAT_OFFSET;
	port->altstatus_addr	=
	port->ctl_addr		= base + K2_SATA_TF_CTL_OFFSET;
	port->bmdma_addr	= base + K2_SATA_DMA_CMD_OFFSET;
	port->scr_addr		= base + K2_SATA_SCR_STATUS_OFFSET;
}


static int k2_sata_init_one(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	static int printed_version;
	const struct ata_port_info *ppi[] =
		{ &k2_port_info[ent->driver_data], NULL };
	struct ata_host *host;
	void __iomem *mmio_base;
	int n_ports, i, rc, bar_pos;

	if (!printed_version++)
		dev_printk(KERN_DEBUG, &pdev->dev, "version " DRV_VERSION "\n");

	/* allocate host */
#ifdef CONFIG_BRCM_SATA_SINGLE_PORT
	n_ports = 1;
#else
	n_ports = 2;
#endif

	host = ata_host_alloc_pinfo(&pdev->dev, ppi, n_ports);
	if (!host)
		return -ENOMEM;

	bar_pos = 5;
	if (ppi[0]->flags & K2_FLAG_BAR_POS_3)
		bar_pos = 3;
	/*
	 * If this driver happens to only be useful on Apple's K2, then
	 * we should check that here as it has a normal Serverworks ID
	 */
	rc = pcim_enable_device(pdev);
	if (rc)
		return rc;

	/*
	 * Check if we have resources mapped at all (second function may
	 * have been disabled by firmware)
	 */
	if (pci_resource_len(pdev, bar_pos) == 0) {
		/* In IDE mode we need to pin the device to ensure that
			pcim_release does not clear the busmaster bit in config
			space, clearing causes busmaster DMA to fail on
			ports 3 & 4 */
		pcim_pin_device(pdev);
		return -ENODEV;
	}

	/* Request and iomap PCI regions */
	rc = pcim_iomap_regions(pdev, 1 << bar_pos, DRV_NAME);
	if (rc == -EBUSY)
		pcim_pin_device(pdev);
	if (rc)
		return rc;
	host->iomap = pcim_iomap_table(pdev);
	mmio_base = host->iomap[bar_pos];

	/* different controllers have different number of ports - currently 4 or 8 */
	/* All ports are on the same function. Multi-function device is no
	 * longer available. This should not be seen in any system. */
	for (i = 0; i < host->n_ports; i++) {
		struct ata_port *ap = host->ports[i];
		unsigned int offset = i * K2_SATA_PORT_OFFSET;

		k2_sata_setup_port(&ap->ioaddr, mmio_base + offset);

		ata_port_pbar_desc(ap, 5, -1, "mmio");
		ata_port_pbar_desc(ap, 5, offset, "port");
	}

	rc = pci_set_dma_mask(pdev, ATA_DMA_MASK);
	if (rc)
		return rc;
	rc = pci_set_consistent_dma_mask(pdev, ATA_DMA_MASK);
	if (rc)
		return rc;

	/* Clear a magic bit in SCR1 according to Darwin, those help
	 * some funky seagate drives (though so far, those were already
	 * set by the firmware on the machines I had access to)
	 */
	writel(readl(mmio_base + K2_SATA_SICR1_OFFSET) & ~0x00040000,
	       mmio_base + K2_SATA_SICR1_OFFSET);

	/* Clear SATA error & interrupts we don't use */
	writel(0xffffffff, mmio_base + K2_SATA_SCR_ERROR_OFFSET);
	writel(0x0, mmio_base + K2_SATA_SIM_OFFSET);

	pci_set_master(pdev);
#ifdef CONFIG_BRCMSTB
	bcm97xxx_sata_init(pdev, mmio_base, n_ports, ppi[0]->flags & K2_FLAG_BRCM_SATA2);
#endif
	return ata_host_activate(host, pdev->irq, ata_bmdma_interrupt,
				 IRQF_SHARED, &k2_sata_sht);
}

static const struct pci_device_id k2_sata_pci_tbl[] = {
	{ PCI_VDEVICE(SERVERWORKS, 0x0242), chip_sata1 },	/* 7401 and older */
	{ PCI_VDEVICE(BROADCOM,    0x8602), chip_sata2 },	/* 7400 and newer */
	{ }
};

static struct pci_driver k2_sata_pci_driver = {
	.name			= DRV_NAME,
	.id_table		= k2_sata_pci_tbl,
	.probe			= k2_sata_init_one,
#ifdef CONFIG_BRCM_PM
	.remove			= k2_sata_remove_one,
#else
	.remove			= ata_pci_remove_one,
#endif
};

static int __init k2_sata_init(void)
{
	return pci_register_driver(&k2_sata_pci_driver);
}

static void __exit k2_sata_exit(void)
{
	pci_unregister_driver(&k2_sata_pci_driver);
}

MODULE_AUTHOR("Benjamin Herrenschmidt");
MODULE_DESCRIPTION("low-level driver for K2 SATA controller");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(pci, k2_sata_pci_tbl);
MODULE_VERSION(DRV_VERSION);

module_init(k2_sata_init);
module_exit(k2_sata_exit);
