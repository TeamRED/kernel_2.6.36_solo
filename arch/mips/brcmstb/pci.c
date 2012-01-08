/*
 * Copyright (C) 2009 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/compiler.h>

#include <asm/delay.h>
#include <asm/io.h>
#include <asm/debug.h>
#include <asm/brcmstb/brcmstb.h>

/* NOTE: all PHYSICAL addresses */
#define PCI_MEM_START		0xd0000000
#define PCI_MEM_SIZE		0x20000000

/* this is really 0x60000 long, but 32k ought to be enough for anyone */
#define PCI_IO_START		0xf0000000
#define PCI_IO_SIZE		0x00008000
#define PCI_IO_ACTUAL_SIZE	0x00600000

#if defined(BCHP_PCIX_BRIDGE_GRB_REG_START)
#define SATA_MEM_START		(BCHP_PCIX_BRIDGE_GRB_REG_START + 0x10010000)
#else
#define SATA_MEM_START		0x10510000
#endif
#define SATA_MEM_SIZE		0x00010000

#define PCIE_MEM_START		0xa0000000
#define PCIE_MEM_SIZE		0x20000000

/* internal controller registers for configuration reads/writes */
#define PCI_IO_REG_START	0xf0600000
#define PCI_IO_REG_SIZE		0x0000000c
#define PCI_CFG_INDEX		0x04
#define PCI_CFG_DATA		0x08

#if   defined(BCHP_PCIX_BRIDGE_SATA_CFG_INDEX)
#define SATA_IO_REG_START	BPHYSADDR(BCHP_PCIX_BRIDGE_SATA_CFG_INDEX)
#elif defined(BCHP_PCI_BRIDGE_SATA_CFG_INDEX)
#define SATA_IO_REG_START	BPHYSADDR(BCHP_PCI_BRIDGE_SATA_CFG_INDEX)
#endif
#define SATA_IO_REG_SIZE	0x00000008
#define SATA_CFG_INDEX		0x00
#define SATA_CFG_DATA		0x04

#define PCIE_IO_REG_START	0xf1000000
#define PCIE_IO_REG_SIZE	0x00000020
#define PCIE_CFG_INDEX		0x00
#define PCIE_CFG_DATA		0x04

/* Use assigned bus numbers so "ops" can tell the controllers apart */
#define BRCM_BUSNO_PCI23	0x00
#define BRCM_BUSNO_SATA		0x01
#define BRCM_BUSNO_PCIE		0x02

static int brcm_pci_read_config(struct pci_bus *bus, unsigned int devfn,
	int where, int size, u32 *data);
static int brcm_pci_write_config(struct pci_bus *bus, unsigned int devfn,
	int where, int size, u32 data);

static inline int get_busno_pci23(void) { return BRCM_BUSNO_PCI23; }
static inline int get_busno_sata(void)  { return BRCM_BUSNO_SATA;  }
static inline int get_busno_pcie(void)  { return BRCM_BUSNO_PCIE;  }

static struct pci_ops brcmstb_pci_ops = {
	.read = brcm_pci_read_config,
	.write = brcm_pci_write_config,
};

/*
 * Notes on PCI IO space:
 *
 * This is only really supported by PCI2.3.
 * The SATA PCI-X controller does support IO space, but libata never uses it.
 * Our PCIe core does not support IO BARs at all.  MEM only.
 * The Linux PCI code insists on having IO resources and io_map_base for
 *   all PCI controllers, so the values for PCI-X and PCIe are totally bogus.
 */
#define IO_ADDR_PCI23		0x400

#define IO_ADDR_SATA		(IO_ADDR_PCI23 + PCI_IO_SIZE)
#define IO_ADDR_PCIE		(IO_ADDR_SATA + SATA_IO_SIZE)
#define SATA_IO_SIZE		0x400
#define PCIE_IO_SIZE		0x400
#define BOGUS_IO_MAP_BASE	1

/* pci_controller resources */

static struct resource pci23_mem_resource = {
	.name			= "External PCI2.3 MEM",
	.start			= PCI_MEM_START,
	.end			= PCI_MEM_START + PCI_MEM_SIZE - 1,
	.flags			= IORESOURCE_MEM,
};

static struct resource sata_mem_resource = {
	.name			= "Internal SATA PCI-X MEM",
	.start			= SATA_MEM_START,
	.end			= SATA_MEM_START + SATA_MEM_SIZE - 1,
	.flags			= IORESOURCE_MEM,
};

static struct resource pcie_mem_resource = {
	.name			= "External PCIe MEM",
	.start			= PCIE_MEM_START,
	.end			= PCIE_MEM_START + PCIE_MEM_SIZE - 1,
	.flags			= IORESOURCE_MEM,
};

static struct resource pci23_io_resource = {
	.name			= "External PCI2.3 IO",
	.start			= IO_ADDR_PCI23,
	.end			= IO_ADDR_PCI23 + PCI_IO_SIZE - 1,
	.flags			= IORESOURCE_IO,
};

static struct resource sata_io_resource = {
	.name			= "Internal SATA PCI-X IO (unavailable)",
	.start			= IO_ADDR_SATA,
	.end			= IO_ADDR_SATA + SATA_IO_SIZE - 1,
	.flags			= IORESOURCE_IO,
};

static struct resource pcie_io_resource = {
	.name			= "External PCIe IO (unavailable)",
	.start			= IO_ADDR_PCIE,
	.end			= IO_ADDR_PCIE + PCIE_IO_SIZE - 1,
	.flags			= IORESOURCE_MEM,
};

/* Definitions for each controller */

static struct pci_controller brcmstb_pci_controller = {
	.pci_ops		= &brcmstb_pci_ops,
	.io_resource		= &pci23_io_resource,
	.mem_resource		= &pci23_mem_resource,
	.get_busno		= &get_busno_pci23,
};

static struct pci_controller brcmstb_sata_controller = {
	.pci_ops		= &brcmstb_pci_ops,
	.io_resource		= &sata_io_resource,
	.mem_resource		= &sata_mem_resource,
	.get_busno		= &get_busno_sata,
	.io_map_base		= BOGUS_IO_MAP_BASE,
};

static struct pci_controller brcmstb_pcie_controller = {
	.pci_ops		= &brcmstb_pci_ops,
	.io_resource		= &pcie_io_resource,
	.mem_resource		= &pcie_mem_resource,
	.get_busno		= &get_busno_pcie,
	.io_map_base		= BOGUS_IO_MAP_BASE,
};

struct brcm_pci_bus {
	struct pci_controller	*controller;
	char			*name;
	unsigned int		hw_busnum;
	unsigned int		busnum_shift;
	unsigned int		slot_shift;
	unsigned int		func_shift;
	int			memory_hole;
	unsigned long		idx_reg;
	unsigned long		data_reg;
};

static struct brcm_pci_bus brcm_buses[] = {
	[BRCM_BUSNO_PCI23] =
		{ &brcmstb_pci_controller,  "PCI2.3",  0, 0,  11, 8,  0 },
	[BRCM_BUSNO_SATA] =
		{ &brcmstb_sata_controller, "SATA",    0, 0,  15, 12, 1 },
	[BRCM_BUSNO_PCIE] =
		{ &brcmstb_pcie_controller, "PCIe",    1, 20, 15, 12, 0 },
};


/***********************************************************************
 * PCI/PCIX/PCIe Bridge setup
 ***********************************************************************/

#define SATA_MEM_ENABLE		1
#define SATA_BUS_MASTER_ENABLE	2
#define SATA_PERR_ENABLE	0x10
#define SATA_SERR_ENABLE	0x20

#define PCI_BUS_MASTER  BCHP_PCI_CFG_STATUS_COMMAND_BUS_MASTER_MASK
#define PCI_IO_ENABLE   BCHP_PCI_CFG_STATUS_COMMAND_MEMORY_SPACE_MASK
#define PCI_MEM_ENABLE  BCHP_PCI_CFG_STATUS_COMMAND_IO_SPACE_MASK

#if defined(CONFIG_CPU_BIG_ENDIAN)
#define	DATA_ENDIAN		2	/* PCI->DDR inbound accesses */
#define MMIO_ENDIAN		2	/* MIPS->PCI outbound accesses */
#else
#define	DATA_ENDIAN		0
#define MMIO_ENDIAN		0
#endif


static inline void brcm_setup_sata_bridge(void)
{
#if defined(CONFIG_BRCM_HAS_SATA1)

	/* Internal PCI SATA bridge setup for 7038, 7401, 7403, 7118, etc. */

	BDEV_SET(BCHP_PCI_BRIDGE_PCI_CTRL,
		(SATA_MEM_ENABLE|SATA_BUS_MASTER_ENABLE|
		 SATA_PERR_ENABLE|SATA_SERR_ENABLE));

	/* PCI slave window (SATA access to MIPS memory) */
	BDEV_WR(BCHP_PCI_BRIDGE_PCI_SLV_MEM_WIN_BASE, 0 | DATA_ENDIAN);

	/* PCI master window (MIPS access to SATA BARs) */
	BDEV_WR(BCHP_PCI_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE,
		SATA_MEM_START | MMIO_ENDIAN);

	BDEV_WR(BCHP_PCI_BRIDGE_CPU_TO_SATA_IO_WIN_BASE, 0 | MMIO_ENDIAN);

#elif defined(CONFIG_BRCM_HAS_SATA2)

	/* Internal PCI-X SATA bridge setup for 7400, 7405, 7335 */

	BDEV_WR(BCHP_PCIX_BRIDGE_PCIX_CTRL,
		(SATA_MEM_ENABLE|SATA_BUS_MASTER_ENABLE|
		 SATA_PERR_ENABLE|SATA_SERR_ENABLE));

	/* PCI slave window (SATA access to MIPS memory) */
	BDEV_WR(BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_BASE, 1);
	BDEV_WR(BCHP_PCIX_BRIDGE_PCIX_SLV_MEM_WIN_MODE, DATA_ENDIAN);

	/* PCI master window (MIPS access to SATA BARs) */
	BDEV_WR(BCHP_PCIX_BRIDGE_CPU_TO_SATA_MEM_WIN_BASE,
		SATA_MEM_START | MMIO_ENDIAN);
	BDEV_WR(BCHP_PCIX_BRIDGE_CPU_TO_SATA_IO_WIN_BASE, MMIO_ENDIAN);

#endif
}

static inline void brcm_setup_pci_bridge(void)
{
#if defined(CONFIG_BRCM_HAS_PCI23)

	unsigned long win_size_mb;

	/* External PCI bridge setup (most chips) */

	BDEV_SET(BCHP_PCI_CFG_STATUS_COMMAND,
		PCI_BUS_MASTER|PCI_IO_ENABLE|PCI_MEM_ENABLE);

	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_MEM_WIN0, PCI_MEM_START + 0x00000000 +
		MMIO_ENDIAN);
	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_MEM_WIN1, PCI_MEM_START + 0x08000000 +
		MMIO_ENDIAN);
	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_MEM_WIN2, PCI_MEM_START + 0x10000000 +
		MMIO_ENDIAN);
	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_MEM_WIN3, PCI_MEM_START + 0x18000000 +
		MMIO_ENDIAN);

	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_IO_WIN0, 0x00000000 | MMIO_ENDIAN);
	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_IO_WIN1, 0x00200000 | MMIO_ENDIAN);
	BDEV_WR(BCHP_PCI_CFG_CPU_2_PCI_IO_WIN2, 0x00400000 | MMIO_ENDIAN);

	/* force PCI->SDRAM window to 1GB on reduced-strap chips */
#if defined(BCHP_HIF_TOP_CTRL_PCI_MWIN_CTRL)
	BDEV_WR_RB(BCHP_HIF_TOP_CTRL_PCI_MWIN_CTRL, 0x1);
#elif defined(BCHP_HIF_TOP_CTRL_MWIN_CTRL)
	/* alternate spelling on 7420b0 */
	BDEV_WR_RB(BCHP_HIF_TOP_CTRL_MWIN_CTRL, 0x1);
#endif

	BDEV_WR(BCHP_PCI_CFG_MEMORY_BASE_W0, 0xfffffff0);
	win_size_mb = (~(BDEV_RD(BCHP_PCI_CFG_MEMORY_BASE_W0) & ~0xfUL) + 1UL) >> 20;
	printk(KERN_INFO "PCI2.3->SDRAM window: %lu MB\n", win_size_mb);
	if(win_size_mb < brcm_dram0_size_mb)
		printk(KERN_WARNING
			"WARNING: PCI2.3 window size is smaller than "
			"system memory\n");
	BDEV_WR(BCHP_PCI_CFG_MEMORY_BASE_W0, 0x00000000);

	/* not used - move them out of the way */
	BDEV_WR(BCHP_PCI_CFG_MEMORY_BASE_W1, 0x80000000);
	BDEV_WR(BCHP_PCI_CFG_MEMORY_BASE_W2, 0x80000000);
	BDEV_WR(BCHP_PCI_CFG_GISB_BASE_W, 0x80000000);

	/* set endianness for W0 */
	BDEV_UNSET(BCHP_PCI_CFG_PCI_SDRAM_ENDIAN_CTRL,
		BCHP_PCI_CFG_PCI_SDRAM_ENDIAN_CTRL_ENDIAN_MODE_MWIN0_MASK);
	BDEV_SET(BCHP_PCI_CFG_PCI_SDRAM_ENDIAN_CTRL, DATA_ENDIAN);
#endif
}

#if defined(CONFIG_BRCM_HAS_PCIE)
static struct wktmr_time pcie_reset_started;

#define PCIE_LINK_UP() \
	(((BDEV_RD(BCHP_PCIE_MISC_PCIE_STATUS) & 0x30) == 0x30) ? 1 : 0)

#else

#define PCIE_LINK_UP() 0

#endif

void brcm_early_pcie_setup(void)
{
#if defined(CONFIG_BRCM_HAS_PCIE)
	/*
	 * Called from bchip_early_setup() in order to start PCIe link
	 * negotiation immediately at kernel boot time.  The RC is supposed
	 * to give the endpoint device 100ms to settle down before
	 * attempting configuration accesses.  So we let the link negotiation
	 * happen in the background instead of busy-waiting.
	 */

	struct wktmr_time tmp;

	/* reset the bridge and the endpoint device */
	BDEV_WR_F_RB(HIF_RGR1_SW_RESET_1, PCIE_BRIDGE_SW_RESET, 1);
	BDEV_WR_F_RB(HIF_RGR1_SW_RESET_1, PCIE_SW_PERST, 1);

	/* delay 100us */
	wktmr_read(&tmp);
	while (wktmr_elapsed(&tmp) < (100 * WKTMR_1US)) { }

	/* take the bridge out of reset */
	BDEV_WR_F_RB(HIF_RGR1_SW_RESET_1, PCIE_BRIDGE_SW_RESET, 0);

	/* enable CSR_READ_UR_MODE and SCB_ACCESS_EN */
	BDEV_WR(BCHP_PCIE_MISC_MISC_CTRL, 0x3000);

	/* set up MIPS->PCIE memory windows (4x 128MB) */
	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN0_LO,
		PCIE_MEM_START + 0x00000000 + MMIO_ENDIAN);
	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN0_HI, 0);

	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN1_LO,
		PCIE_MEM_START + 0x08000000 + MMIO_ENDIAN);
	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN1_HI, 0);

	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN2_LO,
		PCIE_MEM_START + 0x10000000 + MMIO_ENDIAN);
	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN2_HI, 0);

	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN3_LO,
		PCIE_MEM_START + 0x18000000 + MMIO_ENDIAN);
	BDEV_WR(BCHP_PCIE_MISC_CPU_2_PCIE_MEM_WIN3_HI, 0);

	/* set up 1GB PCIE->SCB memory window on BAR2 */
	BDEV_WR(BCHP_PCIE_MISC_RC_BAR2_CONFIG_LO, 0x0000000f);
	BDEV_WR(BCHP_PCIE_MISC_RC_BAR2_CONFIG_HI, 0x00000000);

	/* disable PCIE->GISB window */
	BDEV_WR(BCHP_PCIE_MISC_RC_BAR1_CONFIG_LO, 0x00000000);
	/* disable the other PCIE->SCB memory window */
	BDEV_WR(BCHP_PCIE_MISC_RC_BAR3_CONFIG_LO, 0x00000000);

	/* disable MSI (for now...) */
	BDEV_WR(BCHP_PCIE_MISC_MSI_BAR_CONFIG_LO, 0);

	/* set up L2 interrupt masks */
	BDEV_WR_RB(BCHP_PCIE_INTR2_CPU_CLEAR, 0);
	BDEV_WR_RB(BCHP_PCIE_INTR2_CPU_MASK_CLEAR, 0);
	BDEV_WR_RB(BCHP_PCIE_INTR2_CPU_MASK_SET, 0xffffffff);

	/* take the EP device out of reset */
	BDEV_WR_F_RB(HIF_RGR1_SW_RESET_1, PCIE_SW_PERST, 0);

	/* record the current time */
	wktmr_read(&pcie_reset_started);

#endif
}

static inline void brcm_setup_pcie_bridge(void)
{
#if defined(CONFIG_BRCM_HAS_PCIE)

	/* give the RC/EP time to wake up, before trying to configure RC */
	while (wktmr_elapsed(&pcie_reset_started) < (100 * WKTMR_1MS)) { }

	if (! PCIE_LINK_UP()) {
		printk(KERN_INFO "PCI: PCIe link down\n");
		return;
	}
	printk(KERN_INFO "PCI: PCIe link up\n");

	/* enable MEM_SPACE and BUS_MASTER for RC */
	BDEV_WR(BCHP_PCIE_RC_CFG_TYPE1_STATUS_COMMAND, 0x6);

	/* set base/limit for outbound transactions */
	BDEV_WR(BCHP_PCIE_RC_CFG_TYPE1_RC_MEM_BASE_LIMIT, 0xbff0a000);
	/* disable the prefetch range */
	BDEV_WR(BCHP_PCIE_RC_CFG_TYPE1_RC_PREF_BASE_LIMIT, 0x0000fff0);

	/* set pri/sec bus numbers */
	BDEV_WR(BCHP_PCIE_RC_CFG_TYPE1_PRI_SEC_BUS_NO, 0x00010100);

	/* PCIE->SCB endian mode for BAR2 */
	BDEV_WR_F_RB(PCIE_RC_CFG_VENDOR_VENDOR_SPECIFIC_REG1, ENDIAN_MODE_BAR2,
		DATA_ENDIAN);
#endif
}

/***********************************************************************
 * PCI controller registration
 ***********************************************************************/

static int __init brcmstb_pci_init(void)
{
	unsigned long reg_base;

#if defined(CONFIG_BRCM_HAS_PCI23)
	if (brcm_pci_enabled) {
		request_mem_region(PCI_IO_START, PCI_IO_ACTUAL_SIZE,
			"External PCI2.3 IO");
		request_mem_region(PCI_IO_REG_START, PCI_IO_REG_SIZE,
			"External PCI2.3 registers");

		reg_base = (unsigned long)
			ioremap(PCI_IO_REG_START, PCI_IO_REG_SIZE);
		brcm_buses[BRCM_BUSNO_PCI23].idx_reg = reg_base + PCI_CFG_INDEX;
		brcm_buses[BRCM_BUSNO_PCI23].data_reg = reg_base + PCI_CFG_DATA;

		brcm_setup_pci_bridge();

		brcmstb_pci_controller.io_map_base = (unsigned long)
			ioremap(PCI_IO_START, PCI_IO_SIZE);

		/*
		 * INVALID port ranges include:
		 *   0x0000 .. (IO_ADDR_PCI23 - 1)
		 *   SATA/PCIe IO regions
		 * These are (intentionally) located in the ioremap() guard
		 * pages so that they cause an exception on access.
		 */
		set_io_port_base(brcmstb_pci_controller.io_map_base -
			IO_ADDR_PCI23);

		register_pci_controller(&brcmstb_pci_controller);
	}
#endif
#ifdef CONFIG_BRCM_HAS_SATA
	if (brcm_sata_enabled) {

		request_mem_region(SATA_IO_REG_START, SATA_IO_REG_SIZE,
			"Internal SATA PCI-X registers");
		reg_base = (unsigned long)
			ioremap(SATA_IO_REG_START, SATA_IO_REG_SIZE);

		brcm_setup_sata_bridge();

		brcm_buses[BRCM_BUSNO_SATA].idx_reg = reg_base + SATA_CFG_INDEX;
		brcm_buses[BRCM_BUSNO_SATA].data_reg = reg_base + SATA_CFG_DATA;

		register_pci_controller(&brcmstb_sata_controller);
	}
#endif
#ifdef CONFIG_BRCM_HAS_PCIE
	if (brcm_pcie_enabled) {
		brcm_setup_pcie_bridge();

		request_mem_region(PCIE_IO_REG_START, PCIE_IO_REG_SIZE,
			"External PCIe registers");
		reg_base = (unsigned long)ioremap(PCIE_IO_REG_START,
			PCIE_IO_REG_SIZE);

		brcm_buses[BRCM_BUSNO_PCIE].idx_reg = reg_base + PCIE_CFG_INDEX;
		brcm_buses[BRCM_BUSNO_PCIE].data_reg = reg_base + PCIE_CFG_DATA;

		register_pci_controller(&brcmstb_pcie_controller);
	}
#endif
	return 0;
}

arch_initcall(brcmstb_pci_init);

/***********************************************************************
 * Read/write PCI configuration registers
 ***********************************************************************/

#define CFG_INDEX(bus, devfn, reg) \
	(((PCI_SLOT(devfn) & 0x1f) << (brcm_buses[bus->number].slot_shift)) | \
	 ((PCI_FUNC(devfn) & 0x07) << (brcm_buses[bus->number].func_shift)) | \
	 (brcm_buses[bus->number].hw_busnum << \
	  brcm_buses[bus->number].busnum_shift) | \
	 (reg))

static int devfn_ok(struct pci_bus *bus, unsigned int devfn)
{
	/* SATA is the only device on the bus, with devfn == 0 */
	if (bus->number == BRCM_BUSNO_SATA && devfn != 0)
		return 0;

	/* PCIe: check for link down or invalid slot number */
	if (bus->number == BRCM_BUSNO_PCIE &&
	    (! PCIE_LINK_UP() || PCI_SLOT(devfn) != 0))
		return 0;

	return 1;	/* OK */
}

static int brcm_pci_write_config(struct pci_bus *bus, unsigned int devfn,
	int where, int size, u32 data)
{
	u32 val = 0, mask, shift;

	if (! devfn_ok(bus, devfn))
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	BUG_ON(((where & 3) + size) > 4);

	if (size < 4) {
		/* partial word - read, modify, write */
		DEV_WR_RB(brcm_buses[bus->number].idx_reg,
			CFG_INDEX(bus, devfn, where & ~3));
		val = DEV_RD(brcm_buses[bus->number].data_reg);
	}

	shift = (where & 3) << 3;
	mask = (0xffffffff >> ((4 - size) << 3)) << shift;

	DEV_WR_RB(brcm_buses[bus->number].idx_reg,
		CFG_INDEX(bus, devfn, where & ~3));
	val = (val & ~mask) | ((data << shift) & mask);
	DEV_WR_RB(brcm_buses[bus->number].data_reg, val);

	return PCIBIOS_SUCCESSFUL;
}

static int brcm_pci_read_config(struct pci_bus *bus, unsigned int devfn,
	int where, int size, u32 *data)
{
	u32 val, mask, shift;

	if (! devfn_ok(bus, devfn))
		return PCIBIOS_FUNC_NOT_SUPPORTED;

	BUG_ON(((where & 3) + size) > 4);

	DEV_WR_RB(brcm_buses[bus->number].idx_reg,
		CFG_INDEX(bus, devfn, where & ~3));
	val = DEV_RD(brcm_buses[bus->number].data_reg);

	shift = (where & 3) << 3;
	mask = (0xffffffff >> ((4 - size) << 3)) << shift;

	*data = (val & mask) >> shift;
	return PCIBIOS_SUCCESSFUL;
}

/***********************************************************************
 * PCI slot to IRQ mappings (aka "fixup")
 ***********************************************************************/

#define NUM_SLOTS	16

/* board-specific definitions are in arch/mips/brcmstb/board.c */
extern char irq_tab_brcmstb[NUM_SLOTS][4] __initdata;
extern char irq_tab_brcmstb_docsis[NUM_SLOTS][4] __initdata;

int __devinit pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
#if defined(CONFIG_BRCM_HAS_SATA)
	if (dev->bus->number == BRCM_BUSNO_SATA)
		return BRCM_IRQ_SATA;
#endif
#if defined(CONFIG_BRCM_HAS_PCIE)
	if (dev->bus->number == BRCM_BUSNO_PCIE) {
		const int pcie_irq[] = {
			BRCM_IRQ_PCIE_INTA, BRCM_IRQ_PCIE_INTB,
			BRCM_IRQ_PCIE_INTC, BRCM_IRQ_PCIE_INTD,
		};
		if ((pin - 1) > 3)
			return 0;
		return pcie_irq[pin - 1];
	}
#endif
#if defined(CONFIG_BRCM_HAS_PCI23)
	if ((slot >= NUM_SLOTS) || ((pin - 1) > 3))
		return 0;
	return brcm_docsis_platform ?
		irq_tab_brcmstb_docsis[slot][pin - 1] :
		irq_tab_brcmstb[slot][pin - 1];
#else
	return 0;
#endif
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	return 0;
}

/***********************************************************************
 * Per-device initialization
 ***********************************************************************/

static void __devinit brcm_pcibios_fixup(struct pci_dev *dev)
{
	int slot = PCI_SLOT(dev->devfn);

	printk(KERN_INFO
		"PCI: found device %04x:%04x on %s bus, slot %d (irq %d)\n",
		dev->vendor, dev->device, brcm_buses[dev->bus->number].name,
		slot, pcibios_map_irq(dev, slot, 1));

	/* zero out the BARs and let Linux assign an address */
	pci_write_config_dword(dev, PCI_COMMAND, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_0, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_1, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_2, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_3, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_4, 0);
	pci_write_config_dword(dev, PCI_BASE_ADDRESS_5, 0);
	pci_write_config_dword(dev, PCI_INTERRUPT_LINE, 0);
}

DECLARE_PCI_FIXUP_EARLY(PCI_ANY_ID, PCI_ANY_ID, brcm_pcibios_fixup);

/***********************************************************************
 * DMA address remapping
 ***********************************************************************/

/*
 * There is a 256MB hole at 1000_0000 in the system memory map.  So the
 * first 256MB of RAM starts at PA 0000_0000, but the remaining memory starts
 * at PA 2000_0000 instead of 1000_0000.
 *
 * On PCI2.3 and PCIe, but not on PCI-X, the two memory regions are
 * made contiguous in hardware to avoid making the PCI window 256MB larger
 * than it needs to be.  Therefore, we need to adjust the bus addresses
 * in this case because they will not match our Linux physical addresses.
 */

static int dev_collapses_memory_hole(struct device *dev)
{
#if defined(CONFIG_BRCM_UPPER_MEMORY) || defined(CONFIG_HIGHMEM)

	struct pci_dev *pdev;

	if (unlikely(dev == NULL) ||
	    likely(dev->bus != &pci_bus_type))
		return 0;

	pdev = to_pci_dev(dev);
	if (unlikely(pdev == NULL) ||
	    brcm_buses[pdev->bus->number].memory_hole)
		return 0;

	return 1;
#else
	return 0;
#endif
}

static dma_addr_t brcm_phys_to_pci(struct device *dev, unsigned long phys)
{
	if (dev_collapses_memory_hole(dev) &&
	    (phys >= (BRCM_PCI_HOLE_START + BRCM_PCI_HOLE_SIZE)))
		phys -= BRCM_PCI_HOLE_SIZE;
	return phys;
}

dma_addr_t plat_map_dma_mem(struct device *dev, void *addr, size_t size)
{
	return brcm_phys_to_pci(dev, virt_to_phys(addr));
}

dma_addr_t plat_map_dma_mem_page(struct device *dev, struct page *page)
{
	return brcm_phys_to_pci(dev, page_to_phys(page));
}

unsigned long plat_dma_addr_to_phys(struct device *dev, dma_addr_t dma_addr)
{
	if (dev_collapses_memory_hole(dev) &&
	    (dma_addr >= BRCM_PCI_HOLE_START))
		dma_addr += BRCM_PCI_HOLE_SIZE;
	return dma_addr;
}
