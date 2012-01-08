/*
 * Copyright (C) 2010 Jaedon Shin <jaedon.shin@gmail.com>
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

#include <linux/kernel.h>
#include <linux/gpio.h>

#include <asm/brcmstb/brcmstb.h>

#define BCHP_GPIO_ODEN		(0x00)
#define BCHP_GPIO_DATA		(0x04)
#define BCHP_GPIO_IODIR		(0x08)
#define BCHP_GPIO_EC		(0x0c)
#define BCHP_GPIO_EI		(0x10)
#define BCHP_GPIO_MASK		(0x14)
#define BCHP_GPIO_LEVEL		(0x18)
#define BCHP_GPIO_STAT		(0x1c)

#define BCHP_GPIO_LO		(0x00)
#define BCHP_GPIO_HI		(0x20)
#define BCHP_GPIO_EXT		(0x40)
#define BCHP_GPIO_EXT_HI	(0x60)

/* TODO use GPIO_EXT and GPIO_EXT_HI */
#define BCM_GPIO_REG(gpio)	((gpio < 32 ? BCHP_GPIO_LO : BCHP_GPIO_HI))

struct bcm_gpio_chip {
	void __iomem		*regs;
	struct gpio_chip	chip;
};

static int bcm_gpio_get_value(struct gpio_chip *chip, unsigned gpio)
{
	struct bcm_gpio_chip *gpch = container_of(chip, struct bcm_gpio_chip, chip);

	void __iomem *gpio_in = gpch->regs + BCM_GPIO_REG(gpio) + BCHP_GPIO_DATA;

	return readl(gpio_in) & (1 << gpio);
}

static void bcm_gpio_set_value(struct gpio_chip *chip, unsigned gpio, int value)
{
	struct bcm_gpio_chip *gpch = container_of(chip, struct bcm_gpio_chip, chip);

	void __iomem *gpio_out = gpch->regs + BCM_GPIO_REG(gpio) + BCHP_GPIO_DATA;
	unsigned tmp;

	tmp = readl(gpio_out) & ~(1 << gpio);
	if (value)
		tmp |= 1<< gpio;
	writel(tmp, gpio_out);
}

static int bcm_gpio_direction_input(struct gpio_chip *chip, unsigned gpio)
{
	struct bcm_gpio_chip *gpch = container_of(chip, struct bcm_gpio_chip, chip);

	void __iomem *gpio_dir = gpch->regs + BCM_GPIO_REG(gpio) + BCHP_GPIO_IODIR;

	writel(readl(gpio_dir) | (1 << gpio), gpio_dir);

	return 0;
}

static int bcm_gpio_direction_output(struct gpio_chip *chip, unsigned gpio, int value)
{
	struct bcm_gpio_chip *gpch = container_of(chip, struct bcm_gpio_chip, chip);

	void __iomem *gpio_dir = gpch->regs + BCM_GPIO_REG(gpio) + BCHP_GPIO_IODIR;

	bcm_gpio_set_value(chip, gpio, value);
	writel(readl(gpio_dir) & ~(1 << gpio), gpio_dir);

	return 0;
}

static struct bcm_gpio_chip bcm_gpio_chip = {
	.chip = {
		.label			= "bcm-gpio",
		.direction_input	= bcm_gpio_direction_input,
		.direction_output	= bcm_gpio_direction_output,
		.set			= bcm_gpio_set_value,
		.get			= bcm_gpio_get_value,
		.base			= 0,
		.ngpio			= 64, /* FIXME */
	}
};

static int __init bcm_gpio_init(void)
{
	int ret;

	bcm_gpio_chip.regs = ioremap_nocache(BCHP_PHYSICAL_OFFSET + BCHP_GIO_REG_START,
				BCHP_GIO_REG_END - BCHP_GIO_REG_START);

	if (!bcm_gpio_chip.regs) {
		printk(KERN_ERR "bcm-gpio: failed to ioremap regs\n");
		return -ENOMEM;
	}

	ret = gpiochip_add(&bcm_gpio_chip.chip);
	if (ret) {
		printk(KERN_ERR "bcm-gpio: failed to add gpiochip\n");
		return ret;
	}
	printk(KERN_INFO "bcm-gpio: registered %i GPIOs\n", bcm_gpio_chip.chip.ngpio);

	return ret;
}
arch_initcall(bcm_gpio_init);
