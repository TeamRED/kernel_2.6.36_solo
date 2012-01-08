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

#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/brcmstb/brcmstb.h>

#define XTALFREQ		27000000
#define REG_OFFSET		(0xa0000000 | BCHP_PHYSICAL_OFFSET)
#define UPG_USEC_TICKS		(XTALFREQ / 1000000)

/* for best results, turn the UPG 27 clock back on to see debug output */
#define DEBUG_ASM
#define DEBUG_NO_UPG27_DISABLE
#define UART_BASE		BCHP_UARTA_REG_START

#if defined(CONFIG_BRCM_HAS_16550)
#define UART_LSR_OFFSET		0x14
#define UART_LSR_MASK		BCHP_UARTA_LSR_THRE_MASK
#define UART_TX_OFFSET		0x00
#else
#define UART_LSR_OFFSET		0x18
#define UART_LSR_MASK		BCHP_UARTA_XMTSTAT_TDRE_MASK
#define UART_TX_OFFSET		0x1c
#endif

	.text
	.set	noreorder
	.align	5

# a0 (s0) is 1 to power down pads, 0 to leave them enabled,
#   or -1 for self refresh + pads on
# a1 is icache line size
# a2 is the interrupt vector (or general exception base)

# s1 is saved SCB refresh client setting

	LEAF(brcm_pm_standby)
	subu	sp, 32
	sw	ra, 0(sp)
	sw	s0, 4(sp)
	sw	s1, 8(sp)

# lock the asm section into the I-cache
	move	s0, a0
	addiu	t1, s0, -1
	not	t1

	la	t0, brcm_pm_standby
	and	t0, t1

	la	t2, passiveIRQ_end
	and	t2, t1

1:	cache	0x1c, 0(t0)
	bne	t0, t2, 1b
	addu	t0, s0

# now lock the interrupt vector
	cache	0x1c, 0(a1)

	bal	dputc
	li	a0, 's'

# Flush memory transactions and enter self-refresh mode
	sync

	li	t0, REG_OFFSET + BCHP_MEMC_0_DDR_POWER_DOWN_MODE
	li	t1, 0x300a
	sw	t1, 0(t0)

#ifndef DEBUG_NO_UPG27_DISABLE
# kill UPG 27 clock
	li	t0, REG_OFFSET + BCHP_CLKGEN_PWRDN_CTRL_3
	lw	t2, 0(t0)
	li	t1, BCHP_CLKGEN_PWRDN_CTRL_3_PWRDN_CLOCK_27_CG_SUN_MASK
	or	t2, t1
	sw	t2, 0(t0)
#endif

# kill MIPS and PLLs, then enter passive standby
	li	t0, REG_OFFSET + BCHP_CLKGEN_PM_CTRL
	sw	zero, 0(t0)
	lw	zero, 0(t0)

	li	t1, 3
	sw	t1, 0(t0)

wait_loop:
# enable int2 and then wait for an interrupt
	mfc0	t0, CP0_STATUS
	li	t1, ~(ST0_IM | ST0_IE)
	and	t0, t1
	ori	t0, STATUSF_IP2
	mtc0	t0, CP0_STATUS
	nop
	nop
	nop
	ori	t0, ST0_IE
	mtc0	t0, CP0_STATUS
	wait
	nop


# MIPS sleep bit is not self-clearing
	li	t0, REG_OFFSET + BCHP_CLKGEN_PM_CTRL
	sw	zero, 0(t0)

# restore UPG 27 clock
	li	t0, REG_OFFSET + BCHP_CLKGEN_PWRDN_CTRL_3
	lw	t2, 0(t0)
	li	t1, ~BCHP_CLKGEN_PWRDN_CTRL_3_PWRDN_CLOCK_27_CG_SUN_MASK
	and	t2, t1
	sw	t2, 0(t0)

# exit self-refresh mode
	li	t0, REG_OFFSET + BCHP_MEMC_0_DDR_POWER_DOWN_MODE
	sw	zero, 0(t0)

	sync

rtn_from_wait:
	bal	dputc
	li	a0, 'S'

# unlock I$ lines
	addiu	t1, s0, -1
	not	t1

	la	t0, brcm_pm_standby
	and	t0, t1

	la	t2, passiveIRQ_end
	and	t2, t1

1:	cache	Hit_Invalidate_I, 0(t0)
	bne	t0, t2, 1b
	addu	t0, s0

# return to caller
	lw	s1, 8(sp)
	lw	s0, 4(sp)
	lw	ra, 0(sp)
	addiu	sp, 32

	jr	ra
	nop
	END(brcm_pm_standby)

# debug character output

dputc:
#ifdef DEBUG_ASM
	/* INPUT: char in a0 */
	/* USES: t1, t2 */
	li 	t1, (UART_BASE | REG_OFFSET)
1:
	lw 	t2, UART_LSR_OFFSET(t1)
	andi	t2, UART_LSR_MASK
	beqz	t2, 1b
	nop

	sw	a0, UART_TX_OFFSET(t1)

1:	lw 	t2, UART_LSR_OFFSET(t1)
	andi	t2, UART_LSR_MASK
	beqz	t2, 1b
	nop
	jr	ra
	nop
#else
	jr	ra
	nop
#endif

delay:
# need to reset TIMER1 since it is not normally used
	li	t2, 0x0
	li	t1, (BCHP_TIMER_TIMER1_CTRL | REG_OFFSET)
	sw	t2, 0(t1)
	lw	t2, 0(t1)

	li	t2, 0xbfffffff
	sw	t2, 0(t1)
	lw	t2, 0(t1)

	li	t2, 0x3fffffff
	li	t1, (BCHP_TIMER_TIMER1_STAT | REG_OFFSET)
	lw	t0, 0(t1)
	and	t0, t2
	add	t0, a0
	not	t2
	and	t2, t0
	bnez	t2, delay_overflow
	nop

# wait for timer value (t2) to exceed expiry time (t0)
1:	lw	t2, 0(t1)
	sltu	t2, t2, t0
	bnez	t2, 1b
	nop

	jr	ra
	nop

# timer1 overflow (this should never happen)
delay_overflow:
	move	t4, ra
	bal	dputc
	li	a0, 'O'

	jr	t4
	nop

	LEAF(brcm_pm_irq)

# Block interrupts and then return to the wait loop
	mfc0	k0, CP0_STATUS
	li	k1, ~ST0_IE
	and	k0, k1
	mtc0	k0, CP0_STATUS
	eret
	nop

	END(brcm_pm_irq)
	.globl passiveIRQ_end
passiveIRQ_end:
	nop
