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
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <asm/bootinfo.h>
#include <asm/pmon.h>
#include <asm/time.h>
#include <asm/cpu.h>
#include <asm/smp.h>
#include <asm/cacheflush.h>
#include <asm/mipsregs.h>
#include <asm/brcmstb/brcmstb.h>

static unsigned long smp_boot_sp;
static unsigned long smp_boot_gp;

/* Early cpumask setup - runs on TP0 */
static void brcmstb_smp_setup(void)
{
	int i;

	/* 1:1 mapping between logical and physical CPUs */
	for (i = 0; i < NR_CPUS; i++) {
		cpu_clear(i, cpu_possible_map);
		__cpu_number_map[i] = i;
		__cpu_logical_map[i] = i;
	}

	/* Enable TP0, TP1 */
	cpu_set(0, cpu_possible_map);
	if (brcm_smp_enabled)
		cpu_set(1, cpu_possible_map);
}

static irqreturn_t brcmstb_ipi_interrupt(int irq, void *dev_id);

/* IRQ setup - runs on TP0 */
static void brcmstb_prepare_cpus(unsigned int max_cpus)
{
	if (request_irq(BRCM_IRQ_IPI0, brcmstb_ipi_interrupt, IRQF_DISABLED,
			"smp_ipi_tp0", NULL))
		panic("Can't request TP0 IPI interrupt\n");
	if (request_irq(BRCM_IRQ_IPI1, brcmstb_ipi_interrupt, IRQF_DISABLED,
			"smp_ipi_tp1", NULL))
		panic("Can't request TP1 IPI interrupt\n");
}

#define CLR_FPR(a, b, c, d) \
	"	mtc1	$0, $" #a "\n" \
	"	mtc1	$0, $" #b "\n" \
	"	mtc1	$0, $" #c "\n" \
	"	mtc1	$0, $" #d "\n"

#define BARRIER() \
	"	ssnop\n" \
	"	ssnop\n" \
	"	ssnop\n"


/* TP1 MIPS init - reset vector jumps here */
static void brcmstb_tp1_entry(void)
{
	unsigned long tmp0, tmp1;

	__asm__ __volatile__(
	"	.set	push\n"
	"	.set	reorder\n"

	/* set up CP0 STATUS; enable FPU */
	"	li	%0, 0x30000000\n"
	"	mtc0	%0, $12\n"
	BARRIER()

	/* set local CP0 CONFIG to make kseg0 cacheable, write-back */
	"	mfc0	%0, $16\n"
	"	ori	%0, 0x7\n"
	"	xori	%0, 0x4\n"
	"	mtc0	%0, $16\n"
	BARRIER()

	/* initialize FPU registers */
	CLR_FPR(f0,  f1,  f2,  f3)
	CLR_FPR(f4,  f5,  f6,  f7)
	CLR_FPR(f8,  f9,  f10, f11)
	CLR_FPR(f12, f13, f14, f15)
	CLR_FPR(f16, f17, f18, f19)
	CLR_FPR(f20, f21, f22, f23)
	CLR_FPR(f24, f25, f26, f27)
	CLR_FPR(f28, f29, f30, f31)

#if defined(CONFIG_BMIPS4380)
	/* initialize TP1's local I-cache */
	"	li	%0, 0x80000000\n"
	"	li	%1, 0x80008000\n"
	"	mtc0	$0, $28\n"
	"	mtc0	$0, $28, 1\n"
	BARRIER()

	"1:	cache	0x08, 0(%0)\n"
	"	addiu	%0, 64\n"
	"	bne	%0, %1, 1b\n"
#elif defined(CONFIG_BMIPS5000)
	/* set exception vector base */
	"	la	%0, ebase\n"
	"	lw	%0, 0(%0)\n"
	"	mtc0	%0, $15, 1\n"
	BARRIER()
#endif

	/* use temporary stack to set up upper memory TLB */
	"	li	$29, 0x80000400\n"
	"	la	%0, brcm_upper_tlb_setup\n"
	"	jalr	%0\n"

	/* switch to permanent stack and continue booting */
	"	la	%0, smp_boot_sp\n"
	"	lw	$29, 0(%0)\n"
	"	la	%0, smp_boot_gp\n"
	"	lw	$28, 0(%0)\n"
	"	la	%0, start_secondary\n"
	"	jr	%0\n"
	"	.set	pop\n"
	: "=&r" (tmp0), "=&r" (tmp1)
	:
	: "memory");
}

/* Tell the hardware to boot TP1 - runs on TP0 */
static void brcmstb_boot_secondary(int cpu, struct task_struct *idle)
{
	u32 entry;

#if defined(CONFIG_BMIPS4380)
	/* NBK and weak order flags */
	set_c0_brcm_config_0(0x30000);

	/*
	 * MIPS interrupts 0,1 (SW INT 0,1) cross over to the other TP
	 * MIPS interrupt 2 (HW INT 0) is the TP0 L1 controller output
	 * MIPS interrupt 3 (HW INT 1) is the TP1 L1 controller output
	 */
	change_c0_brcm_cmt_intr(0xf8018000, (0x02 << 27) | (0x03 << 15));
#elif defined(CONFIG_BMIPS5000)
	/* enable raceless SW interrupts */
	set_c0_brcm_config(0x03 << 22);

	/* clear any pending SW interrupts */
	write_c0_brcm_action(0x2000 | (0 << 9) | (0 << 8));
	write_c0_brcm_action(0x2000 | (0 << 9) | (1 << 8));
	write_c0_brcm_action(0x2000 | (1 << 9) | (0 << 8));
	write_c0_brcm_action(0x2000 | (1 << 9) | (1 << 8));

	/* send HW interrupt 0 to TP0, HW interrupt 1 to TP1 */
	change_c0_brcm_mode(0x1f << 27, 0x02 << 27);
#endif

	smp_boot_sp = __KSTK_TOS(idle);
	smp_boot_gp = (unsigned long)task_thread_info(idle);

	/*
	 * NOTE: TP0/TP1 reset and exception vectors have been relocated
	 * TP1 will start executing the jump code (below) from a000_0000.
	 * Changes are in include/asm/mach-brcmstb/kernel-entry-init.h and
	 * arch/mips/kernel/traps.c (search for BMIPS4380).
	 */

	entry = KSEG1ADDR((u32)&brcmstb_tp1_entry);
	DEV_WR(KSEG0 + 0, 0x3c1a0000 | (entry >> 16));	  // lui k0, HI(entry)
	DEV_WR(KSEG0 + 4, 0x375a0000 | (entry & 0xffff)); // ori k0, LO(entry)
	DEV_WR(KSEG0 + 8, 0x03400008);			  // jr k0
	DEV_WR(KSEG0 + 12, 0x00000000);			  // nop

	dma_cache_wback(KSEG0, 16);
	flush_icache_range(KSEG0, KSEG0 + 16);

	printk(KERN_INFO "SMP: Booting CPU%d...\n", cpu);
#if defined(CONFIG_BMIPS4380)
	set_c0_brcm_cmt_ctrl(0x01);
#elif defined(CONFIG_BMIPS5000)
	write_c0_brcm_action(0x9);
#endif
}

/* Early setup - runs on TP1 after cache probe */
static void brcmstb_init_secondary(void)
{
	printk(KERN_INFO "SMP: CPU%d is running\n", smp_processor_id());

	write_c0_compare(read_c0_count() + mips_hpt_frequency / HZ);
	set_c0_status(IE_SW0 | IE_SW1 | IE_IRQ1 | IE_IRQ5 | ST0_IE);
	irq_enable_hazard();
}

/* Late setup - runs on TP1 before entering the idle loop */
static void brcmstb_smp_finish(void)
{
}

/* Runs on TP0 after all CPUs have been booted */
static void brcmstb_cpus_done(void)
{
}

#if defined(CONFIG_BMIPS4380)
static DEFINE_RAW_SPINLOCK(ipi_lock);

static void brcmstb_send_ipi_single(int cpu, unsigned int action)
{
	unsigned long flags;
	raw_spin_lock_irqsave(&ipi_lock, flags);
	set_c0_cause(smp_processor_id() ? C_SW0 : C_SW1);
	irq_enable_hazard();
	raw_spin_unlock_irqrestore(&ipi_lock, flags);
}

static void brcmstb_ack_ipi(unsigned int action)
{
	unsigned long flags;
	raw_spin_lock_irqsave(&ipi_lock, flags);
	clear_c0_cause(smp_processor_id() ? C_SW1 : C_SW0);
	irq_enable_hazard();
	raw_spin_unlock_irqrestore(&ipi_lock, flags);
}

#elif defined(CONFIG_BMIPS5000)

static void brcmstb_send_ipi_single(int cpu, unsigned int action)
{
	//unsigned int bit = (action == SMP_RESCHEDULE_YOURSELF) ? 0 : 1;
	unsigned int bit = cpu;
	write_c0_brcm_action(0x3000 | (bit << 8) | (cpu << 9));
	irq_enable_hazard();
}

static void brcmstb_ack_ipi(unsigned int irq)
{
	//unsigned int bit = (irq == BRCM_IRQ_IPI0) ? 0 : 1;
	unsigned int bit = smp_processor_id();
	write_c0_brcm_action(0x2000 | (bit << 8) | (smp_processor_id() << 9));
	irq_enable_hazard();
}

#endif

static void brcmstb_send_ipi_mask(const struct cpumask *mask, unsigned int action)
{
	unsigned int i;

	for_each_cpu(i, mask)
		brcmstb_send_ipi_single(i, action);
}

static irqreturn_t brcmstb_ipi_interrupt(int irq, void *dev_id)
{
	brcmstb_ack_ipi(irq);
	smp_call_function_interrupt();
	return IRQ_HANDLED;
}

struct plat_smp_ops brcmstb_smp_ops = {
	.smp_setup		= brcmstb_smp_setup,
	.prepare_cpus		= brcmstb_prepare_cpus,
	.boot_secondary		= brcmstb_boot_secondary,
	.smp_finish		= brcmstb_smp_finish,
	.init_secondary		= brcmstb_init_secondary,
	.cpus_done		= brcmstb_cpus_done,
	.send_ipi_single	= brcmstb_send_ipi_single,
	.send_ipi_mask		= brcmstb_send_ipi_mask,
};
