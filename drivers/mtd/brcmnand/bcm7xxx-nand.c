/*
 *
 *  drivers/mtd/brcmnand/bcm7xxx-nand.c
 *
    Copyright (c) 2005-2006 Broadcom Corporation                 
    
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 2 as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    File: bcm7xxx-nand.c

    Description: 
    This is a device driver for the Broadcom NAND flash for bcm97xxx boards.
when	who what
-----	---	----
051011	tht	codings derived from OneNand generic.c implementation.

 * THIS DRIVER WAS PORTED FROM THE 2.6.18-7.2 KERNEL RELEASE
 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>

#include <linux/mtd/partitions.h>

// For CFE partitions.
#include <asm/brcmstb/brcmstb.h>


#include <asm/io.h>
//#include <asm/mach/flash.h>

#include "brcmnand.h"
#include "brcmnand_priv.h"

#define PRINTK(...)
//#define PRINTK printk

#define DRIVER_NAME		"brcmnand"
#define DRIVER_INFO		"Broadcom STB NAND controller"

struct brcmnand_info {
	struct mtd_info		mtd;
	struct brcmnand_chip	brcmnand;
#ifdef CONFIG_MTD_PARTITIONS
	int			nr_parts;
	struct mtd_partition*	parts;
#endif
};
static struct brcmnand_info *info;

#ifdef CONFIG_MTD_PARTITIONS
static const char *part_probe_types[] = { "cmdlinepart", "RedBoot", NULL };
#endif

void* get_brcmnand_handle(void)
{
	void* handle = &info->brcmnand;
	return handle;
}

#define NAND_MAX_CS    8
int gNandCS[NAND_MAX_CS];
/* Number of NAND chips, only applicable to v1.0+ NAND controller */
int gNumNand = 0;
int gClearBBT = 0;
char gClearCET = 0;
uint32_t gNandTiming1 = 0, gNandTiming2 = 0;

static char *cmd = NULL;
static unsigned long t1, t2;

module_param(cmd, charp, 0444);
MODULE_PARM_DESC(cmd, "Special command to run on startup: "
		      "rescan - rescan for bad blocks, update BBT; "
		      "showbbt - print out BBT contents; "
		      "erase - erase entire flash, except CFE, and rescan "
		        "for bad blocks; "
		      "eraseall - erase entire flash, including CFE, and "
		        "rescan for bad blocks; "
		      "clearbbt - erase BBT and rescan for bad blocks "
		        "(DANGEROUS, may lose MFG BI's); "
		      "showcet - show correctable error count; "
		      "resetcet - reset correctable error count to 0 and "
		        "table to all 0xff");
module_param(t1, ulong, 0444);
MODULE_PARM_DESC(t1, "NAND timing value 1");
module_param(t2, ulong, 0444);
MODULE_PARM_DESC(t2, "NAND timing value 2");

static void* gPageBuffer;

static int __devinit brcmnanddrv_probe(struct platform_device *pdev)
{
	struct brcmnand_platform_data *cfg = pdev->dev.platform_data;
	//struct flash_platform_data *pdata = pdev->dev.platform_data;
	//struct resource *res = pdev->resource;
	//unsigned long size = res->end - res->start + 1;
	int err = 0;

	gPageBuffer = NULL;
	info = kmalloc(sizeof(struct brcmnand_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	memset(info, 0, sizeof(struct brcmnand_info));

#ifndef CONFIG_MTD_BRCMNAND_EDU
	gPageBuffer = kmalloc(sizeof(struct nand_buffers), GFP_KERNEL);
	info->brcmnand.buffers = (struct nand_buffers*) gPageBuffer;
#else
	/* Align on 32B boundary for efficient DMA transfer */
	gPageBuffer = kmalloc(sizeof(struct nand_buffers) + 31, GFP_DMA);
	info->brcmnand.buffers = (struct nand_buffers*) (((unsigned int) gPageBuffer+31) & (~31));
#endif
	if (!info->brcmnand.buffers) {
		kfree(info);
		return -ENOMEM;
	}

	memset(info->brcmnand.buffers, 0, sizeof(struct nand_buffers));

	memset(gNandCS, 0, MAX_NAND_CS);
	gNumNand = 1;
	gNandCS[0] = cfg->chip_select;

	info->brcmnand.numchips = 1; // For now, we only support 1 chip
	info->brcmnand.chip_shift = 0; // Only 1 chip
	//info->brcmnand.regs = pdev->resource[0].start;
	info->brcmnand.priv = &info->mtd;

	//info->brcmnand.mmcontrol = NULL;  // THT: Sync Burst Read TBD.  pdata->mmcontrol;

	info->mtd.name = dev_name(&pdev->dev);
	info->mtd.priv = &info->brcmnand;
	info->mtd.owner = THIS_MODULE;

	/* Enable the following for a flash based bad block table */
	info->brcmnand.options |= NAND_USE_FLASH_BBT;
	

//printk("brcmnand_scan\n");
	if (brcmnand_scan(&info->mtd, gNumNand)) {
		err = -ENXIO;
		goto out_free_info;
	}

	printk("	numchips=%d, size=%llx\n", info->brcmnand.numchips, device_size(&(info->mtd)));

#ifdef CONFIG_MTD_PARTITIONS
	/* allow cmdlineparts to override the default map */
	err = parse_mtd_partitions(&info->mtd, part_probe_types,
		&info->parts, 0);
	if (err > 0) {
		info->nr_parts = err;
	} else {
		info->parts = cfg->nr_parts ? cfg->parts : NULL;
		info->nr_parts = cfg->nr_parts;
	}

	if (info->nr_parts)
		add_mtd_partitions(&info->mtd, info->parts, info->nr_parts);
	else
		add_mtd_device(&info->mtd);
#else
	add_mtd_device(&info->mtd);
#endif

//printk("	dev_set_drvdata\n");	
	dev_set_drvdata(&pdev->dev, info);
//printk("<-- brcmnanddrv_probe\n");

	return 0;


out_free_info:

	if (gPageBuffer)
		kfree(gPageBuffer);
	kfree(info);
	return err;
}

static int __devexit brcmnanddrv_remove(struct platform_device *pdev)
{
	struct brcmnand_info *info = dev_get_drvdata(&pdev->dev);
	//struct resource *res = pdev->resource;
	//unsigned long size = res->end - res->start + 1;

	dev_set_drvdata(&pdev->dev, NULL);

	if (info) {
#ifdef CONFIG_MTD_PARTITIONS
		if (info->parts)
			del_mtd_partitions(&info->mtd);
		else
			del_mtd_device(&info->mtd);
#else
		del_mtd_device(&info->mtd);
#endif

		brcmnand_release(&info->mtd);
		//release_mem_region(res->start, size);
		//iounmap(info->brcmnand.base);
		kfree(gPageBuffer);
		kfree(info);
	}

	return 0;
}

static struct platform_driver brcmnand_platform_driver = {
	.probe			= brcmnanddrv_probe,
	.remove			= __devexit_p(brcmnanddrv_remove),
	.driver			= {
		.name		= DRIVER_NAME,
	},
};

static struct resource brcmnand_resources[] = {
	[0] = {
		.name		= DRIVER_NAME,
		.start		= BPHYSADDR(BCHP_NAND_REG_START),
		.end		= BPHYSADDR(BCHP_NAND_REG_END) + 3,
		.flags		= IORESOURCE_MEM,
	},
};

static int __init brcmnanddrv_init(void)
{
	int ret;

	if (cmd) {
		if (strcmp(cmd, "rescan") == 0)
			gClearBBT = 1;
		else if (strcmp(cmd, "showbbt") == 0)
			gClearBBT = 2;
		else if (strcmp(cmd, "eraseall") == 0)
			gClearBBT = 8;
		else if (strcmp(cmd, "erase") == 0)
			gClearBBT = 7;
		else if (strcmp(cmd, "clearbbt") == 0)
			gClearBBT = 9;
		else if (strcmp(cmd, "showcet") == 0)
			gClearCET = 1;
		else if (strcmp(cmd, "resetcet") == 0)
			gClearCET = 2;
		else if (strcmp(cmd, "disablecet") == 0)
			gClearCET = 3;
		else
			printk(KERN_WARNING "%s: unknown command '%s'\n",
				__FUNCTION__, cmd);
	}

	gNandTiming1 = t1;
	gNandTiming2 = t2;

	printk (KERN_INFO DRIVER_INFO " (BrcmNand Controller)\n");
	ret = platform_driver_register(&brcmnand_platform_driver);
	if (ret >= 0)
		request_resource(&iomem_resource, &brcmnand_resources[0]);
	
	return 0;
}

static void __exit brcmnanddrv_exit(void)
{
	release_resource(&brcmnand_resources[0]);
	platform_driver_unregister(&brcmnand_platform_driver);
}

module_init(brcmnanddrv_init);
module_exit(brcmnanddrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ton Truong <ttruong@broadcom.com>");
MODULE_DESCRIPTION("Broadcom NAND flash driver");

