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

#ifndef _ASM_BRCMSTB_BRCMAPI_H
#define _ASM_BRCMSTB_BRCMAPI_H

#ifdef __KERNEL__

#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/dma-mapping.h>
#include <linux/device.h>
#include <linux/types.h>
#include <net/sock.h>
#include <asm/ptrace.h>
#include <asm/inst.h>

/***********************************************************************
 * Kernel hooks
 ***********************************************************************/

void __init brcm_free_bootmem(unsigned long addr, unsigned long size);
void brcm_tlb_init(void);
unsigned long brcm_setup_ebase(void);
int brcm_simulate_opcode(struct pt_regs *regs, unsigned int opcode);
int brcm_unaligned_fp(void __user *addr, union mips_instruction *insn,
	struct pt_regs *regs);
extern int (*brcm_netif_rx_hook)(struct sk_buff *);
int brcm_cacheflush(unsigned long addr, unsigned long bytes,
	unsigned int cache);
void brcm_sync_for_cpu(unsigned long addr, size_t size,
	enum dma_data_direction dir);
unsigned long brcm_fixup_ticks(unsigned long delta);

/***********************************************************************
 * BSP External API
 ***********************************************************************/

/* RAC extension to sys_cacheflush() */
#define	RACACHE	(1<<31)		/* read-ahead cache / prefetching L2 */

struct brcm_cache_info {
	int			max_writeback;
	int			max_writethrough;
	int			prefetch_enabled;
};

struct brcm_ocap_info {
	unsigned long		ocap_part_start;
	unsigned long		ocap_part_len;
	int			docsis_platform;
};

int bmem_find_region(unsigned long addr, unsigned long size);
int bmem_region_info(int idx, unsigned long *addr, unsigned long *size);

void brcm_inv_prefetch(unsigned long addr, unsigned long size);
void brcm_get_cache_info(struct brcm_cache_info *info);

void brcm_get_ocap_info(struct brcm_ocap_info *info);

static inline void brcm_netif_rx_sethook(int (*fn)(struct sk_buff *)) {
	brcm_netif_rx_hook = fn;
}

int brcm_alloc_macaddr(u8 *buf);

extern spinlock_t brcm_magnum_spinlock;

/***********************************************************************
 * New exports of standard kernel symbols
 ***********************************************************************/

/* for netaccel IPv6 */
extern struct proto udpv6_prot;

/* for performance tuning */
extern void (*cpu_wait)(void);

/* for power management interrupts */
extern unsigned long ebase;

/***********************************************************************
 * Driver->BSP callbacks
 ***********************************************************************/

void brcm_pm_enet_add(int dev_id, resource_size_t base_addr);
void brcm_pm_enet_remove(int dev_id, resource_size_t base_addr);
void brcm_pm_moca_add(void);
void brcm_pm_moca_remove(void);
void brcm_pm_usb_add(void);
void brcm_pm_usb_remove(void);
void brcm_pm_sata_add(void);
void brcm_pm_sata_remove(void);

#endif /* __KERNEL__ */

#endif /* _ASM_BRCMSTB_BRCMAPI_H */
