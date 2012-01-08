/*
 * Flash mapping for BCM7325 NOR board
 *
 * Copyright (C) 2011 Ramon-Tomislav Rebersak <fergy@TeamRED>
 *
 * This file is NOT under GPL but PROPRIATRY of author
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/init.h>
#include <asm/brcmstb/brcmstb.h>

#define PRINTK(...)

extern int gFlashSize;

#define WINDOW_ADDR (0x18000000)
#define WINDOW_SIZE (0x20000000	 - WINDOW_ADDR)
#define BUSWIDTH 2

static struct mtd_info *bcm9XXXX_mtd;

struct map_info bcm9XXXX_map = {
	name: "physmap-flash.0",
	bankwidth: BUSWIDTH,
};

#define SMALLEST_FLASH_SIZE		(16<<20)
#define DEFAULT_RESERVED_SIZE 	(4<<20)
#define ROOTFS_PART				(0)

#ifdef CONFIG_MTD_ECM_PARTITION
#define DEFAULT_OCAP_SIZE		(6<<20)
#define DEFAULT_AVAIL1_SIZE 	(32<<20)
#define DEFAULT_ECM_SIZE 		(DEFAULT_OCAP_SIZE+DEFAULT_AVAIL1_SIZE)
#define AVAIL1_PART				(1)
#define OCAP_PART				(2)
#else
#define DEFAULT_ECM_SIZE		(0)
#define DEFAULT_OCAP_SIZE		(0)
#define DEFAULT_AVAIL1_SIZE		(0)
#define AVAIL1_PART				(-1)
#endif

#define DEFAULT_ROOTFS_SIZE (DEFAULT_SIZE_MB - DEFAULT_RESERVED_SIZE - DEFAULT_ECM_SIZE)
#define DEFAULT_SIZE_MB 128

static struct mtd_partition bcm9XXXX_parts[3] = {
	{
		.name	=	"rootfs",
		.offset	=	0x400000,
		.size	=	0x7800000,
	},
	{
		.name	=	"all",
		.offset	=	0,
		.size	=	0x8000000,
	},
	{
		.name	=	"kernel",
		.offset	=	0,
		.size	=	0x400000,
	},
};

int __init init_bcm9XXXX_map(void)
{
//	unsigned int avail1_size = DEFAULT_AVAIL1_SIZE;
	int i, numparts;

	printk(KERN_NOTICE "GigaBox NOR flash device: 0x%08x @ 0x%08x\n", WINDOW_SIZE, WINDOW_ADDR);
	bcm9XXXX_map.size = WINDOW_SIZE;
	numparts = ARRAY_SIZE(bcm9XXXX_parts);

	if (WINDOW_SIZE != (DEFAULT_SIZE_MB << 20)) {
		
		bcm9XXXX_parts[0].size += WINDOW_SIZE - (DEFAULT_SIZE_MB << 20);
PRINTK("Part[0] name=%s, size=%x, offset=%x\n", bcm9XXXX_parts[0].name, bcm9XXXX_parts[0].size, bcm9XXXX_parts[0].offset);
		for (i=1; i<ARRAY_SIZE(bcm9XXXX_parts); i++) {
			bcm9XXXX_parts[i].offset += WINDOW_SIZE - (DEFAULT_SIZE_MB << 20);
PRINTK("Part[%d] name=%s, size=%x, offset=%x\n", i, bcm9XXXX_parts[i].name, bcm9XXXX_parts[i].size, bcm9XXXX_parts[i].offset);
		}
	}
	bcm9XXXX_map.virt = ioremap((unsigned long)WINDOW_ADDR, WINDOW_SIZE);

	if (!bcm9XXXX_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	
	bcm9XXXX_mtd = do_map_probe("cfi_probe", &bcm9XXXX_map);
	if (!bcm9XXXX_mtd) {
		iounmap((void *)bcm9XXXX_map.virt);
		return -ENXIO;
	}
		
	add_mtd_partitions(bcm9XXXX_mtd, bcm9XXXX_parts, numparts);
	bcm9XXXX_mtd->owner = THIS_MODULE;
	return 0;
}

void __exit cleanup_bcm9XXXX_map(void)
{
	if (bcm9XXXX_mtd) {
		del_mtd_partitions(bcm9XXXX_mtd);
		map_destroy(bcm9XXXX_mtd);
	}
	if (bcm9XXXX_map.virt) {
		iounmap((void *)bcm9XXXX_map.virt);
		bcm9XXXX_map.virt = 0;
	}
}

module_init(init_bcm9XXXX_map);
module_exit(cleanup_bcm9XXXX_map);

MODULE_LICENSE("Proprietary");
MODULE_AUTHOR("Ramon-Tomislav Rebersak <fergy@TeamRED>");
MODULE_DESCRIPTION("Broadcom 7325 NOR MTD map driver");
