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
 *
 * Credits: Pierre Ossman, Ovidiu Nastai
 */

#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/mmc/host.h>

#include <asm/scatterlist.h>
#include <asm/io.h>
#include <asm/brcmstb/brcmstb.h>

#include "sdhci.h"

#define DRV_NAME		"sdhci-brcm"

#ifdef CONFIG_CPU_BIG_ENDIAN
#define SWIZZLE8(x)		((x) ^ 3)
#define SWIZZLE16(x)		((x) ^ 2)
#else
#define SWIZZLE8(x)		(x)
#define SWIZZLE16(x)		(x)
#endif

#if defined(CONFIG_BCM7635)

#define SDIO_READ_WAR(host)	do { \
	rmb(); \
	__raw_readl(host->ioaddr + 0x44); \
	} while(0)

static DEFINE_SPINLOCK(sdhci_brcm_lock);

static u32 sdhci_brcm_readl(struct sdhci_host *host, int reg)
{
	unsigned long flags;
	u32 ret;

	spin_lock_irqsave(&sdhci_brcm_lock, flags);
	ret = __raw_readl(host->ioaddr + reg);
	SDIO_READ_WAR(host);
	spin_unlock_irqrestore(&sdhci_brcm_lock, flags);
	return ret;
}

static u16 sdhci_brcm_readw(struct sdhci_host *host, int reg)
{
	unsigned long flags;
	u16 ret;

	spin_lock_irqsave(&sdhci_brcm_lock, flags);
	ret = __raw_readw(host->ioaddr + reg);
	SDIO_READ_WAR(host);
	spin_unlock_irqrestore(&sdhci_brcm_lock, flags);
	return ret;
}

static u8 sdhci_brcm_readb(struct sdhci_host *host, int reg)
{
	unsigned long flags;
	u8 ret;

	spin_lock_irqsave(&sdhci_brcm_lock, flags);
	ret = __raw_readb(host->ioaddr + reg);
	SDIO_READ_WAR(host);
	spin_unlock_irqrestore(&sdhci_brcm_lock, flags);
	return ret;
}

#else

static u32 sdhci_brcm_readl(struct sdhci_host *host, int reg)
{
	return __raw_readl(host->ioaddr + reg);
}

static u16 sdhci_brcm_readw(struct sdhci_host *host, int reg)
{
	return __raw_readw(host->ioaddr + SWIZZLE16(reg));
}

static u8 sdhci_brcm_readb(struct sdhci_host *host, int reg)
{
	return __raw_readb(host->ioaddr + SWIZZLE8(reg));
}

#endif

static void sdhci_brcm_writel(struct sdhci_host *host, u32 val, int reg)
{
	if (unlikely(reg == SDHCI_INT_STATUS)) {
		__raw_writel(val, host->ioaddr + reg);
		HIF_ACK_IRQ(SDIO);
		val = __raw_readl(host->ioaddr + reg);
		if (unlikely(val != 0))
			HIF_TRIGGER_IRQ(SDIO);
		return;
	}
	__raw_writel(val, host->ioaddr + reg);
}

static void sdhci_brcm_writew(struct sdhci_host *host, u16 val, int reg)
{
	__raw_writew(val, host->ioaddr + SWIZZLE16(reg));
}

static void sdhci_brcm_writeb(struct sdhci_host *host, u8 val, int reg)
{
	__raw_writeb(val, host->ioaddr + SWIZZLE8(reg));
}

static struct sdhci_ops sdhci_brcm_ops = {
	.readl			= sdhci_brcm_readl,
	.readw			= sdhci_brcm_readw,
	.readb			= sdhci_brcm_readb,
	.writel			= sdhci_brcm_writel,
	.writew			= sdhci_brcm_writew,
	.writeb			= sdhci_brcm_writeb,
};

static int __devinit sdhci_brcm_probe(struct platform_device *pdev)
{
	struct sdhci_host *host;
	struct resource *mres, *ires;
	int mres_size;
	int err;

	mres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	ires = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!mres || !ires) {
		dev_err(&pdev->dev, "can't get resources\n");
		return -EIO;
	}
	mres_size = mres->end - mres->start + 1;
	if (!request_mem_region(mres->start, mres_size, DRV_NAME)) {
		dev_err(&pdev->dev, "can't request memory\n");
		return -EBUSY;
	}

	host = sdhci_alloc_host(&pdev->dev, 0);
	if (!host) {
		dev_err(&pdev->dev, "out of memory\n");
		return -ENOMEM;
	}
	dev_set_drvdata(&pdev->dev, host);

	host->hw_name = DRV_NAME;
	host->ops = &sdhci_brcm_ops;
	host->flags = 0;
#if defined(CONFIG_BCM7635)
	host->quirks = SDHCI_QUIRK_BROKEN_ADMA | SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;
#else
	host->quirks = SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;
#endif
	host->irq = ires->start;
	host->ioaddr = ioremap(mres->start, mres_size);

	HIF_ACK_IRQ(SDIO);
	HIF_ENABLE_IRQ(SDIO);
	err = sdhci_add_host(host);
	if (err < 0) {
		dev_err(&pdev->dev, "can't register controller (error %d)\n",
			err);
		sdhci_free_host(host);
		return err;
	}

	return 0;
}

static int __devexit sdhci_brcm_remove(struct platform_device *pdev)
{
	struct sdhci_host *host = dev_get_drvdata(&pdev->dev);
	struct resource *mres;
	int mres_size;

	if (host) {
		sdhci_remove_host(host, 0);
		HIF_DISABLE_IRQ(SDIO);

		mres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		mres_size = mres->end - mres->start + 1;
		iounmap(host->ioaddr);
		release_mem_region(mres->start, mres_size);

		sdhci_free_host(host);
	}
	return 0;
}

static struct platform_driver sdhci_brcm_driver = {
	.probe			= sdhci_brcm_probe,
	.remove			= __devexit_p(sdhci_brcm_remove),
	.driver = {
		.name		= DRV_NAME,
	}
};

static int __init sdhci_brcm_drv_init(void)
{
	platform_driver_register(&sdhci_brcm_driver);
	return 0;
}

static void __exit sdhci_brcm_drv_exit(void)
{
	platform_driver_unregister(&sdhci_brcm_driver);
}

module_init(sdhci_brcm_drv_init);
module_exit(sdhci_brcm_drv_exit);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("Secure Digital Host Controller Interface MMIO driver");
MODULE_LICENSE("GPL");
