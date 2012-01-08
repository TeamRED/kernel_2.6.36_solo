/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2009 Broadcom Corporation
 */

#ifndef _ASM_MACH_BRCMSTB_SPACES_H
#define _ASM_MACH_BRCMSTB_SPACES_H

#include <linux/const.h>

#ifdef CONFIG_BRCM_UPPER_MEMORY

/*
 * New virtual address map with CONFIG_BRCM_UPPER_MEMORY=y
 *
 * 8000_0000 - 8fff_ffff: lower 256MB, cached mapping
 * 9000_0000 - 9fff_ffff: EBI/registers, cached mapping (mostly unused)
 * a000_0000 - afff_ffff: lower 256MB, uncached mapping
 * b000_0000 - bfff_ffff: EBI/registers, uncached mapping
 * c000_0000 - cfff_ffff: upper 256MB, cached mapping
 * d000_0000 - dfff_ffff: upper 256MB, uncached mapping
 * e000_0000 - ff1d_7fff: vmalloc region
 * ff1d_8000 - ff1d_ffff: FIXMAP
 */

#define CAC_BASE_UPPER		_AC(0xc0000000, UL)
#define UNCAC_BASE_UPPER	_AC(0xd0000000, UL)
#define MAP_BASE		_AC(0xe0000000, UL)

#define UPPERMEM_START		_AC(0x20000000, UL)
#define HIGHMEM_START		_AC(0xffffffff, UL)

#endif

#define BRCM_PCI_HOLE_START	_AC(0x10000000, UL)
#define BRCM_PCI_HOLE_SIZE	_AC(0x10000000, UL)

#include <asm/mach-generic/spaces.h>

#endif /* _ASM_MACH_BRCMSTB_SPACES_H */
