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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/smp.h>
#include <linux/platform_device.h>
#include <linux/suspend.h>
#include <linux/mii.h>

#include <asm/cpu-info.h>
#include <asm/r4kcache.h>
#include <asm/mipsregs.h>
#include <asm/cacheflush.h>
#include <asm/brcmstb/brcmstb.h>

#if 0
#define DBG			printk
#else
#define DBG(...)		do { } while(0)
#endif

/***********************************************************************
 * CPU divisor / PLL manipulation
 ***********************************************************************/

#ifdef CONFIG_BMIPS5000
/* CP0 COUNT/COMPARE frequency is affected by the PLL input but not divisor */
#define FIXED_COUNTER_FREQ	1
#else
/* CP0 COUNT/COMPARE frequency is affected by the PLL input AND the divisor */
#define FIXED_COUNTER_FREQ	0
#endif

/* MIPS active standby on 7550 */
#define CPU_PLL_MODE1		216000

/*
 * current ADJUSTED base frequency (reflects the current PLL settings)
 * brcm_cpu_khz (in time.c) always has the ORIGINAL clock frequency and
 *   is never changed after bootup
 */
unsigned long brcm_adj_cpu_khz = 0;

/* current CPU divisor, as set by the user */
static int cpu_div = 1;

/*
 * MIPS clockevent code always assumes the original boot-time CP0 clock rate.
 * This function scales the number of ticks according to the current HW
 * settings.
 */
unsigned long brcm_fixup_ticks(unsigned long delta)
{
	unsigned long long tmp = delta;

	if (unlikely(!brcm_adj_cpu_khz))
		brcm_adj_cpu_khz = brcm_cpu_khz;
#if FIXED_COUNTER_FREQ
	tmp = (tmp * brcm_adj_cpu_khz) / brcm_cpu_khz;
#else
	tmp = (tmp * brcm_adj_cpu_khz) / (brcm_cpu_khz * cpu_div);
#endif
	return (unsigned long)tmp;
}

static unsigned int orig_udelay_val[NR_CPUS];

struct spd_change {
	int			old_div;
	int			new_div;
	int			old_base;
	int			new_base;
};

void brcm_set_cpu_speed(void *arg)
{
	struct spd_change *c = arg;
	uint32_t new_div = (uint32_t)c->new_div;
	unsigned long __maybe_unused count, compare, delta;
	int cpu = smp_processor_id();
	uint32_t __maybe_unused tmp0, tmp1, tmp2, tmp3;

	/* scale udelay_val */
	if (!orig_udelay_val[cpu])
		orig_udelay_val[cpu] = current_cpu_data.udelay_val;
	if (c->new_base == brcm_cpu_khz)
		current_cpu_data.udelay_val = orig_udelay_val[cpu] / new_div;
	else
		current_cpu_data.udelay_val =
			(unsigned long long)orig_udelay_val[cpu] *
			c->new_base / (new_div * c->old_base);

	/* scale any pending timer events */
	compare = read_c0_compare();
	count = read_c0_count();

#if !FIXED_COUNTER_FREQ
	if (compare > count)
		delta = ((unsigned long long)(compare - count) *
			c->old_div * c->new_base) /
			(new_div * c->old_base);
	else
		delta = ((unsigned long long)(count - compare) *
			c->old_div * c->new_base) /
			(new_div * c->old_base);
#else
	if (compare > count)
		delta = ((unsigned long long)(compare - count) *
			c->new_base) / c->old_base;
	else
		delta = ((unsigned long long)(count - compare) *
			c->new_base) / c->old_base;
#endif
	write_c0_compare(read_c0_count() + delta);

	if (cpu != 0)
		return;

#if defined(CONFIG_BRCM_CPU_PLL)
	brcm_adj_cpu_khz = c->new_base;
#if defined(CONFIG_BCM7550)
	if (brcm_adj_cpu_khz == CPU_PLL_MODE1) {
		/* 216Mhz */
		BDEV_WR_RB(BCHP_VCXO_CTL_CONFIG_FSM_PLL_NEXT_CFG_3A,
			0x801b2806);
		BDEV_WR_RB(BCHP_VCXO_CTL_CONFIG_FSM_PLL_NEXT_CFG_3B,
			0x00300618);
	} else {
		/* 324Mhz */
		BDEV_WR_RB(BCHP_VCXO_CTL_CONFIG_FSM_PLL_NEXT_CFG_3A,
			0x801b2806);
		BDEV_WR_RB(BCHP_VCXO_CTL_CONFIG_FSM_PLL_NEXT_CFG_3B,
			0x00300418);
	}
	BDEV_WR_RB(BCHP_VCXO_CTL_CONFIG_FSM_PLL_UPDATE, 1);
#else
#error CPU PLL adjustment not supported on this chip
#endif
#endif

	new_div = ffs(new_div) - 1;

	/* see BMIPS datasheet, CP0 register $22 */

#if defined(CONFIG_BMIPS3300)
	change_c0_brcm_bus_pll(0x07 << 22, (new_div << 23) | (0 << 22));
#elif defined(CONFIG_BMIPS5000)
	change_c0_brcm_mode(0x0f << 4, (1 << 7) | (new_div << 4));
#elif defined(CONFIG_BMIPS4380)
	__asm__ __volatile__(
	"	.set	push			\n"
	"	.set	noreorder		\n"
	"	.set	nomacro			\n"
	"	.set	mips32			\n"
	/* get kseg1 address for CBA into %3 */
	"	mfc0	%3, $22, 6		\n"
	"	li	%2, 0xfffc0000		\n"
	"	and	%3, %2			\n"
	"	li	%2, 0xa0000000		\n"
	"	add	%3, %2			\n"
	/* %1 = async bit, %2 = mask out everything but 30:28 */
	"	lui	%1, 0x1000		\n"
	"	lui	%2, 0x8fff		\n"
	"	beqz	%0, 1f			\n"
	"	ori	%2, 0xffff		\n"
	/* handle SYNC to ASYNC */
	"	sync				\n"
	"	mfc0	%4, $22, 5		\n"
	"	and	%4, %2			\n"
	"	or	%4, %1			\n"
	"	mtc0	%4, $22, 5		\n"
	"	nop				\n"
	"	nop				\n"
	"	lw	%2, 4(%3)		\n"
	"	sw	%2, 4(%3)		\n"
	"	sync				\n"
	"	sll	%0, 29			\n"
	"	or	%4, %0			\n"
	"	mtc0	%4, $22, 5		\n"
	"	nop; nop; nop; nop		\n"
	"	nop; nop; nop; nop		\n"
	"	nop; nop; nop; nop		\n"
	"	nop; nop; nop; nop		\n"
	"	b	2f			\n"
	"	nop				\n"
	/* handle ASYNC to SYNC */
	"1:					\n"
	"	mfc0	%4, $22, 5		\n"
	"	and	%4, %2			\n"
	"	or	%4, %1			\n"
	"	mtc0	%4, $22, 5		\n"
	"	nop; nop; nop; nop		\n"
	"	nop; nop; nop; nop		\n"
	"	sync				\n"
	"	and	%4, %2			\n"
	"	mtc0	%4, $22, 5		\n"
	"	nop				\n"
	"	nop				\n"
	"	lw	%2, 4(%3)		\n"
	"	sw	%2, 4(%3)		\n"
	"	sync				\n"
	"2:					\n"
	"	.set	pop			\n"
	: "+r" (new_div),
	  "=&r" (tmp0), "=&r" (tmp1), "=&r" (tmp2), "=&r" (tmp3));
#endif
}

#ifdef CONFIG_BRCM_CPU_DIV

ssize_t brcm_pm_show_cpu_div(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", cpu_div);
}

ssize_t brcm_pm_store_cpu_div(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int val;
	struct spd_change chg;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;

	if (val != 1 && val != 2 && val != 4 && val != 8
#if defined(CONFIG_BMIPS5000)
		&& val != 16
#endif
			)
		return -EINVAL;

	chg.old_div = cpu_div;
	chg.new_div = val;
	chg.old_base = brcm_adj_cpu_khz;
	chg.new_base = brcm_adj_cpu_khz;

	on_each_cpu(brcm_set_cpu_speed, &chg, 1);
	cpu_div = val;
	return count;
}

#endif /* CONFIG_BRCM_CPU_DIV */

#ifdef CONFIG_BRCM_CPU_PLL

static int cpu_pll_mode = 0;

ssize_t brcm_pm_show_cpu_pll(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", cpu_pll_mode);
}

ssize_t brcm_pm_store_cpu_pll(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int val;
	struct spd_change chg;

	if (sscanf(buf, "%d", &val) != 1)
		return -EINVAL;
	
	if (cpu_pll_mode == val)
		return count;

	switch (val) {
	case 0:
		chg.new_base = brcm_cpu_khz;
		break;
	case 1:
		chg.new_base = CPU_PLL_MODE1;
		break;
	default:
		return -EINVAL;
	}

	chg.old_div = cpu_div;
	chg.new_div = cpu_div;
	chg.old_base = brcm_adj_cpu_khz;
	on_each_cpu(brcm_set_cpu_speed, &chg, 1);

	cpu_pll_mode = val;
	return count;
}

#endif /* CONFIG_BRCM_CPU_PLL */

/***********************************************************************
 * USB / ENET / GENET / MoCA / SATA power management
 ***********************************************************************/

#ifdef CONFIG_BRCM_PM

static atomic_t enet_count = ATOMIC_INIT(1);
static atomic_t moca_count = ATOMIC_INIT(1);
static atomic_t usb_count = ATOMIC_INIT(1);
static atomic_t sata_count = ATOMIC_INIT(1);

#define MII_S_C		(BCHP_EMAC_0_REG_START + 0x10)
#define MII_DATA	(BCHP_EMAC_0_REG_START + 0x14)
#define EMAC_INT	(BCHP_EMAC_0_REG_START + 0x1c)
#define CONTROL		(BCHP_EMAC_0_REG_START + 0x2c)

#define MDIO_WR		0x50020000
#define MDIO_PMD_SHIFT	23
#define MDIO_REG_SHIFT	18
#define EMAC_MDIO_INT	0x01

/* write to the internal PHY's registers */
static void brcm_mii_write(int addr, u16 data)
{
	int i;

	BDEV_WR_RB(EMAC_INT, EMAC_MDIO_INT);

	BDEV_WR(MII_DATA, MDIO_WR | (1 << MDIO_PMD_SHIFT) |
		(addr << MDIO_REG_SHIFT) | data);
	for(i = 0; i < 1000; i++) {
		if(BDEV_RD(EMAC_INT) & EMAC_MDIO_INT)
			return;
		udelay(1);
	}
	printk(KERN_WARNING "brcm-pm: MII write timed out\n");
}

static void enet_enable(int dev_id, resource_size_t base_addr)
{
	unsigned long flags;

	printk(KERN_INFO "brcm-pm: enabling power to ENET block\n");

	spin_lock_irqsave(&brcm_magnum_spinlock, flags);

#if defined(BCHP_VCXO_CTL_MISC_EREF_CTRL_POWERDOWN_MASK)
	BDEV_WR_F_RB(VCXO_CTL_MISC_EREF_CTRL, POWERDOWN, 0);
#endif

#if defined(BCHP_CLK_PM_CTRL_2_DIS_ENET_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL_2, DIS_ENET_216M_CLK, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_0_PWRDN_CLOCK_216_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_0, PWRDN_CLOCK_216_CG_ENET, 0);
#endif

#if defined(BCHP_CLK_PM_CTRL_DIS_ENET_108M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL, DIS_ENET_108M_CLK, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_1_PWRDN_CLOCK_108_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_1, PWRDN_CLOCK_108_CG_ENET, 0);
#endif

#if defined(BCHP_CLK_PM_CTRL_1_DIS_ENET_25M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL_1, DIS_ENET_25M_CLK, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_2_PWRDN_CLOCK_25_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_2, PWRDN_CLOCK_25_CG_ENET, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_3_PWRDN_CLOCK_25_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_3, PWRDN_CLOCK_25_CG_ENET, 0);
#endif
	spin_unlock_irqrestore(&brcm_magnum_spinlock, flags);

	/* exit PHY IDDQ mode */
	if (!(BDEV_RD(CONTROL) & 0x08)) {
		brcm_mii_write(0x1f, 0x008b);
		brcm_mii_write(0x10, 0x0000);
		brcm_mii_write(0x14, 0x0000);
		brcm_mii_write(0x1f, 0x000f);
		brcm_mii_write(0x10, 0x00d0);
		brcm_mii_write(0x1f, 0x000b);
	}
}

static void enet_disable(int dev_id, resource_size_t base_addr)
{
	unsigned long flags;

	printk(KERN_INFO "brcm-pm: disabling power to ENET block\n");

	/* enter PHY IDDQ mode */
	if(! (BDEV_RD(CONTROL) & 0x08)) {
		if((BDEV_RD(MII_S_C) & 0x3f) == 0)
			BDEV_WR(MII_S_C, 0x9f);
		brcm_mii_write(MII_BMCR, BMCR_RESET);
		brcm_mii_write(0x1f, 0x008b);
		brcm_mii_write(0x10, 0x01c0);
		brcm_mii_write(0x14, 0x7000);
		brcm_mii_write(0x1f, 0x000f);
		brcm_mii_write(0x10, 0x20d0);
		brcm_mii_write(0x1f, 0x000b);
	}

	spin_lock_irqsave(&brcm_magnum_spinlock, flags);

#if defined(BCHP_CLKGEN_PWRDN_CTRL_3_PWRDN_CLOCK_25_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_3, PWRDN_CLOCK_25_CG_ENET, 1);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_2_PWRDN_CLOCK_25_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_2, PWRDN_CLOCK_25_CG_ENET, 1);
#endif

#if defined(BCHP_CLK_PM_CTRL_1_DIS_ENET_25M_CLK_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_1, DIS_ENET_25M_CLK, 1);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_1_PWRDN_CLOCK_108_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_1, PWRDN_CLOCK_108_CG_ENET, 1);
#endif

#if defined(BCHP_CLK_PM_CTRL_DIS_ENET_108M_CLK_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL, DIS_ENET_108M_CLK, 1);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_0_PWRDN_CLOCK_216_CG_ENET_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_0, PWRDN_CLOCK_216_CG_ENET, 1);
#endif

#if defined(BCHP_CLK_PM_CTRL_2_DIS_ENET_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL_2, DIS_ENET_216M_CLK, 1);
#endif

#if defined(BCHP_VCXO_CTL_MISC_EREF_CTRL_POWERDOWN_MASK)
	BDEV_WR_F_RB(VCXO_CTL_MISC_EREF_CTRL, POWERDOWN, 1);
#endif
	spin_unlock_irqrestore(&brcm_magnum_spinlock, flags);
}

static void usb_enable(void)
{
	unsigned long flags;

	printk(KERN_INFO "brcm-pm: enabling power to USB block\n");

	spin_lock_irqsave(&brcm_magnum_spinlock, flags);

#if defined(BCHP_CLK_SYS_PLL_0_PLL_4_DIS_CH_MASK)
	BDEV_WR_F_RB(CLK_SYS_PLL_0_PLL_4, DIS_CH, 0);
#endif

#if defined(BCHP_CLK_USB_PM_CTRL_DIS_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_USB_PM_CTRL, DIS_216M_CLK, 0);
#endif

#if defined(BCHP_CLK_PM_CTRL_2_DIS_USB_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL_2, DIS_USB_216M_CLK, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_0_PWRDN_CLOCK_216_CG_USB_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_0, PWRDN_CLOCK_216_CG_USB, 0);
#endif

#if defined(BCHP_CLK_USB_PM_CTRL_DIS_108M_CLK_MASK)
	BDEV_WR_F_RB(CLK_USB_PM_CTRL, DIS_108M_CLK, 0);
#endif

#if defined(BCHP_CLK_PM_CTRL_DIS_USB_108M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL, DIS_USB_108M_CLK, 0);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_1_PWRDN_CLOCK_108_CG_USB_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_1, PWRDN_CLOCK_108_CG_USB, 0);
#endif

	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, PHY_PWDNB, 1);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, PHY1_PWDNB, 1);
	BDEV_WR_F_RB(USB_CTRL_PLL_CTL_1, PLL_PWRDWNB, 1);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, UTMI_PWDNB, 1);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, UTMI1_PWDNB, 1);

	spin_unlock_irqrestore(&brcm_magnum_spinlock, flags);
}

static void usb_disable(void)
{
	unsigned long flags;

	printk(KERN_INFO "brcm-pm: disabling power to USB block\n");

	spin_lock_irqsave(&brcm_magnum_spinlock, flags);

	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, UTMI1_PWDNB, 0);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, UTMI_PWDNB, 0);
	BDEV_WR_F_RB(USB_CTRL_PLL_CTL_1, PLL_PWRDWNB, 0);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, PHY1_PWDNB, 0);
	BDEV_WR_F_RB(USB_CTRL_UTMI_CTL_1, PHY_PWDNB, 0);

#if defined(BCHP_CLKGEN_PWRDN_CTRL_1_PWRDN_CLOCK_108_CG_USB_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_1, PWRDN_CLOCK_108_CG_USB, 1);
#endif

#if defined(BCHP_CLK_PM_CTRL_DIS_USB_108M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL, DIS_USB_108M_CLK, 1);
#endif

#if defined(BCHP_CLK_USB_PM_CTRL_DIS_108M_CLK_MASK)
	BDEV_WR_F_RB(CLK_USB_PM_CTRL, DIS_108M_CLK, 1);
#endif

#if defined(BCHP_CLKGEN_PWRDN_CTRL_0_PWRDN_CLOCK_216_CG_USB_MASK)
	BDEV_WR_F_RB(CLKGEN_PWRDN_CTRL_0, PWRDN_CLOCK_216_CG_USB, 1);
#endif

#if defined(BCHP_CLK_PM_CTRL_2_DIS_USB_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_PM_CTRL_2, DIS_USB_216M_CLK, 1);
#endif

#if defined(BCHP_CLK_USB_PM_CTRL_DIS_216M_CLK_MASK)
	BDEV_WR_F_RB(CLK_USB_PM_CTRL, DIS_216M_CLK, 1);
#endif

#if defined(BCHP_CLK_SYS_PLL_0_PLL_4_DIS_CH_MASK)
	BDEV_WR_F_RB(CLK_SYS_PLL_0_PLL_4, DIS_CH, 1);
#endif

	spin_unlock_irqrestore(&brcm_magnum_spinlock, flags);
}

void brcm_pm_enet_add(int dev_id, resource_size_t base_addr)
{
	if (atomic_inc_return(&enet_count) == 1)
		enet_enable(dev_id, base_addr);
}
EXPORT_SYMBOL(brcm_pm_enet_add);

void brcm_pm_enet_remove(int dev_id, resource_size_t base_addr)
{
	if (atomic_inc_return(&enet_count) == 0)
		enet_disable(dev_id, base_addr);
}
EXPORT_SYMBOL(brcm_pm_enet_remove);

void brcm_pm_moca_add(void)
{
}
EXPORT_SYMBOL(brcm_pm_moca_add);

void brcm_pm_moca_remove(void)
{
}
EXPORT_SYMBOL(brcm_pm_moca_remove);

void brcm_pm_usb_add(void)
{
	if (atomic_inc_return(&usb_count) == 1)
		usb_enable();
}
EXPORT_SYMBOL(brcm_pm_usb_add);

void brcm_pm_usb_remove(void)
{
	if (atomic_dec_return(&usb_count) == 0)
		usb_disable();
}
EXPORT_SYMBOL(brcm_pm_usb_remove);

void brcm_pm_sata_add(void)
{
}
EXPORT_SYMBOL(brcm_pm_sata_add);

void brcm_pm_sata_remove(void)
{
}
EXPORT_SYMBOL(brcm_pm_sata_remove);

/* sysfs attributes */

ssize_t brcm_pm_show_enet_power(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", !!atomic_read(&enet_count));
}

ssize_t brcm_pm_show_moca_power(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", !!atomic_read(&moca_count));
}

ssize_t brcm_pm_show_usb_power(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", !!atomic_read(&usb_count));
}

ssize_t brcm_pm_store_usb_power(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	return -ENODEV;
}

ssize_t brcm_pm_show_sata_power(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", !!atomic_read(&sata_count));
}

ssize_t brcm_pm_store_sata_power(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	return -ENODEV;
}

static int __init brcm_pm_init(void)
{
	brcm_pm_enet_remove(0, BCHP_EMAC_0_REG_START);
	usb_disable();
	return 0;
}

early_initcall(brcm_pm_init);

#endif

/***********************************************************************
 * Passive standby
 ***********************************************************************/

#ifdef CONFIG_BRCM_HAS_STANDBY

static suspend_state_t suspend_state;

static int brcm_pm_prepare(void)
{
	DBG("%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

static int brcm_pm_suspend(void)
{
	u32 entry, vec = ebase;
	u32 oldvec[4];

	DBG("%s:%d\n", __FUNCTION__, __LINE__);

	brcm_irq_standby_enter(BRCM_IRQ_PM);

	/* install jump to special PM exception handler */
	if((read_c0_config() & (1 << 31)) &&
	   (read_c0_config1() & (1 << 31)) &&
	   (read_c0_config2() & (1 << 31)) &&
	   (read_c0_config3() & (1 << 5))) {
		/* vectored interrupts (MIPS 24k/34k VInt bit is set) */

		/* IntCtl 9:5 = vector spacing */
		vec += 0x200 + 2 * (read_c0_intctl() & 0x3e0);
	} else {
		/* interrupts use the general exception vector on this CPU */
		vec += 0x200;
	}

	oldvec[0] = DEV_RD(vec + 0);
	oldvec[1] = DEV_RD(vec + 4);
	oldvec[2] = DEV_RD(vec + 8);
	oldvec[3] = DEV_RD(vec + 12);

	entry = (u32)&brcm_pm_irq;
	DEV_WR(vec + 0, 0x3c1a0000 | (entry >> 16));	  // lui k0, HI(entry)
	DEV_WR(vec + 4, 0x375a0000 | (entry & 0xffff));	  // ori k0, LO(entry)
	DEV_WR(vec + 8, 0x03400008);			  // jr k0
	DEV_WR(vec + 12, 0x00000000);			  // nop

	flush_icache_range(vec, vec + 16);

	brcm_pm_standby(current_cpu_data.icache.linesz, ebase);

	DEV_WR(vec + 0, oldvec[0]);
	DEV_WR(vec + 4, oldvec[1]);
	DEV_WR(vec + 8, oldvec[2]);
	DEV_WR(vec + 12, oldvec[3]);
	flush_icache_range(vec, vec + 16);

	brcm_irq_standby_exit();
	return 0;
}

static int brcm_pm_enter(suspend_state_t unused)
{
	int ret = 0;

	DBG("%s:%d\n", __FUNCTION__, __LINE__);
	switch (suspend_state) {
	case PM_SUSPEND_STANDBY:
	//case PM_SUSPEND_MEM:
		ret = brcm_pm_suspend();
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static void brcm_pm_finish(void)
{
	DBG("%s:%d\n", __FUNCTION__, __LINE__);
}

/* Hooks to enable / disable UART interrupts during suspend */
static int brcm_pm_begin(suspend_state_t state)
{
	DBG("%s:%d\n", __FUNCTION__, __LINE__);
	suspend_state = state;
	return 0;
}

static void brcm_pm_end(void)
{
	DBG("%s:%d\n", __FUNCTION__, __LINE__);
	suspend_state = PM_SUSPEND_ON;
	return;
}

static int brcm_pm_valid(suspend_state_t state)
{
	return state == PM_SUSPEND_STANDBY;
}

static struct platform_suspend_ops brcm_pm_ops = {
	.begin		= brcm_pm_begin,
	.end		= brcm_pm_end,
	.prepare	= brcm_pm_prepare,
	.enter		= brcm_pm_enter,
	.finish		= brcm_pm_finish,
	.valid		= brcm_pm_valid,
};

static int brcm_suspend_init(void)
{
	DBG("%s:%d\n", __FUNCTION__, __LINE__);
	suspend_set_ops(&brcm_pm_ops);
	return 0;
}
late_initcall(brcm_suspend_init);
#endif /* CONFIG_BRCM_HAS_STANDBY */
