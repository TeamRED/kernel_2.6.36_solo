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

#include <linux/platform_device.h>
#include <asm/brcmstb/brcmstb.h>

static int ohci_brcm_reset(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);

	ohci->flags |= OHCI_QUIRK_BE_MMIO;
	ohci_hcd_init(ohci);
	return ohci_init(ohci);
}

static int ohci_brcm_start(struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	struct ohci_regs __iomem *regs;

	regs = hcd->regs;

	ohci_writel(ohci, 1, &regs->cmdstatus);
	ohci_readl(ohci, &regs->cmdstatus);
	mdelay(10);

	ohci_hcd_init(ohci);
	ohci_init(ohci);
	ohci_run(ohci);
	hcd->state = HC_STATE_RUNNING;
	return 0;
}

static const struct hc_driver ohci_brcm_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Broadcom STB OHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ohci_hcd),
	.irq			= ohci_irq,
	.flags			= HCD_USB11 | HCD_MEMORY,
	.reset			= ohci_brcm_reset,
	.start			= ohci_brcm_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,
	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,
	.get_frame_number	= ohci_get_frame,
	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif
	.start_port_reset	= ohci_start_port_reset,
};

static int ohci_hcd_brcm_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct usb_hcd *hcd;
	int irq;
	int ret;

	if (usb_disabled())
		return -ENODEV;

#ifdef CONFIG_BRCM_PM
	brcm_pm_usb_add();
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		err("platform_get_resource error.");
		return -ENODEV;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		err("platform_get_irq error.");
		return -ENODEV;
	}

	/* initialize hcd */
	hcd = usb_create_hcd(&ohci_brcm_hc_driver, &pdev->dev, (char *)hcd_name);
	if (!hcd) {
		err("Failed to create hcd");
		return -ENOMEM;
	}

	hcd->regs = ioremap(res->start, res->end - res->start);
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = (res->end - res->start) + 1;

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret != 0) {
		err("Failed to add hcd");
		iounmap(hcd->regs);
		usb_put_hcd(hcd);
		return ret;
	}

	return ret;
}

static int ohci_hcd_brcm_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	usb_put_hcd(hcd);

	return 0;
}

static struct platform_driver ohci_hcd_brcm_driver = {
	.probe		= ohci_hcd_brcm_probe,
	.remove		= ohci_hcd_brcm_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name	= "ohci-brcm",
		.owner	= THIS_MODULE,
	},
};

MODULE_ALIAS("ohci-brcm");
