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

#include <linux/bootmem.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/vmalloc.h>

#include <asm/page.h>
#include <asm/pgtable-32.h>
#include <asm/addrspace.h>
#include <asm/tlbflush.h>
#include <asm/r4kcache.h>
#include <asm/brcmstb/brcmstb.h>

/***********************************************************************
 * BMEM (reserved A/V buffer memory) support
 ***********************************************************************/

#define LINUX_MIN_MEM		64
#define MAX_BMEM_REGIONS	4

struct bmem_region {
	unsigned long		addr;
	unsigned long		size;
	int			valid;
};

static struct bmem_region bmem_regions[MAX_BMEM_REGIONS];
static unsigned int n_bmem_regions = 0;
static unsigned int bmem_disabled = 0;

/*
 * Parses command line for bmem= options
 */
static int __init bmem_setup(char *str)
{
	unsigned long addr = 0, size;
	unsigned long lower_mem_bytes;
	char *orig_str = str;

	lower_mem_bytes = (brcm_dram0_size_mb > BRCM_MAX_LOWER_MB) ?
		(BRCM_MAX_LOWER_MB << 20) : (brcm_dram0_size_mb << 20);

	size = (unsigned long)memparse(str, &str);
	if (*str == '@')
		addr = (unsigned long)memparse(str + 1, &str);

	if ((size > 0x40000000) || (addr > 0x80000000)) {
		printk(KERN_WARNING "bmem: argument '%s' "
			"is out of range, ignoring\n", orig_str);
		return 0;
	}

	if (size == 0) {
		printk(KERN_INFO "bmem: disabling reserved memory\n");
		bmem_disabled = 1;
		return 0;
	}

	if ((addr & ~PAGE_MASK) || (size & ~PAGE_MASK)) {
		printk(KERN_WARNING "bmem: ignoring invalid range '%s' "
			"(is it missing an 'M' suffix?)\n", orig_str);
		return 0;
	}

	if (addr == 0) {
		/*
		 * default: bmem=xxM allocates xx megabytes at the end of
		 * lower memory
		 */
		if (size >= lower_mem_bytes) {
			printk(KERN_WARNING "bmem: '%s' is larger than "
				"lower memory (%lu MB), ignoring\n",
				orig_str, brcm_dram0_size_mb);
			return 0;
		}
		addr = lower_mem_bytes - size;
	}

	if (n_bmem_regions == MAX_BMEM_REGIONS) {
		printk(KERN_WARNING "bmem: too many regions, "
			"ignoring extras\n");
		return 0;
	}

	bmem_regions[n_bmem_regions].addr = addr;
	bmem_regions[n_bmem_regions].size = size;
	n_bmem_regions++;

	return 0;
}

early_param("bmem", bmem_setup);

/*
 * Returns index if the supplied range falls entirely within a bmem region
 */
int bmem_find_region(unsigned long addr, unsigned long size)
{
	int i, idx = 0;

	for (i = 0; i < n_bmem_regions; i++) {
		if (!bmem_regions[i].valid)
			continue;
		if ((addr >= bmem_regions[i].addr) &&
		    ((addr + size) <=
			(bmem_regions[i].addr + bmem_regions[i].size))) {
			return idx;
		}
		idx++;
	}
	return -ENOENT;
}
EXPORT_SYMBOL(bmem_find_region);

/*
 * Finds the IDX'th valid bmem region, and fills in addr/size if it exists.
 * Returns 0 on success, <0 on failure.
 */
int bmem_region_info(int idx, unsigned long *addr, unsigned long *size)
{
	int i;

	for (i = 0; i < n_bmem_regions; i++) {
		if (!bmem_regions[i].valid)
			continue;
		if (!idx) {
			*addr = bmem_regions[i].addr;
			*size = bmem_regions[i].size;
			return 0;
		}
		idx--;
	}
	return -ENOENT;
}
EXPORT_SYMBOL(bmem_region_info);

static int __initdata bmem_defaults_set;

static void __init brcm_set_default_bmem(void)
{
	/*
	 * Default: (no valid bmem= options specified)
	 *
	 * Lower memory is the first 256MB of system RAM.
	 * bmem gets all but (LINUX_MIN_MEM) megabytes of lower memory.
	 * Linux gets all upper and high memory (if present).
	 *
	 * Options:
	 *
	 * Define one or more custom bmem regions, e.g.:
	 *   bmem=128M (128MB region at the end of lower memory)
	 *   bmem=128M@64M (128MB hole at 64MB mark)
	 *   bmem=16M@64M bmem=4M@128M bmem=16M@200M (multiple holes)
	 *
	 * Disable bmem; give all system RAM to the Linux VM:
	 *   bmem=0
	 *
	 * Overlapping or invalid regions will usually be silently dropped.
	 * If you are doing something tricky, watch the boot messages to
	 * make sure it turns out the way you intended.
	 */
	if (bmem_disabled) {
		n_bmem_regions = 0;
	} else {
		if (n_bmem_regions == 0 &&
		    (brcm_dram0_size_mb > LINUX_MIN_MEM)) {
			n_bmem_regions = 1;
			bmem_regions[0].addr = LINUX_MIN_MEM << 20;
			bmem_regions[0].size =
				(((brcm_dram0_size_mb <= BRCM_MAX_LOWER_MB) ?
				  brcm_dram0_size_mb : BRCM_MAX_LOWER_MB) -
				 LINUX_MIN_MEM) << 20;
		}
	}
}

/*
 * Invokes free_bootmem(), but truncates ranges where necessary to
 * avoid releasing the bmem region(s) back to the VM
 */
void __init brcm_free_bootmem(unsigned long addr, unsigned long size)
{
	if (!bmem_defaults_set) {
		brcm_set_default_bmem();
		bmem_defaults_set = 1;
	}

	while (size) {
		unsigned long chunksize = size;
		int i;
		struct bmem_region *r = NULL;

		/*
		 * Find the first bmem region (if any) that fits entirely
		 * within the current bootmem address range.
		 */
		for (i = 0; i < n_bmem_regions; i++) {
			if ((bmem_regions[i].addr >= addr) &&
			    ((bmem_regions[i].addr + bmem_regions[i].size) <=
			     (addr + size))) {
				if (!r || (r->addr > bmem_regions[i].addr))
					r = &bmem_regions[i];
			}
		}

		/*
		 * Skip over every bmem region; call free_bootmem() for
		 * every Linux region.  A Linux region is created for
		 * each chunk of the memory map that is not reserved
		 * for bmem.
		 */
		if (r) {
			if (addr == r->addr) {
				printk(KERN_INFO "bmem: adding %lu MB "
					"RESERVED region at %lu MB "
					"(0x%08lx@0x%08lx)\n",
					r->size >> 20, r->addr >> 20,
					r->size, r->addr);
				chunksize = r->size;
				r->valid = 1;
				goto skip;
			} else {
				BUG_ON(addr > r->addr);
				chunksize = r->addr - addr;
			}
		}
		BUG_ON(chunksize > size);

		printk(KERN_DEBUG "bmem: adding %lu MB LINUX region at %lu MB "
			"(0x%08lx@0x%08lx)\n", chunksize >> 20, addr >> 20,
			chunksize, addr);
		free_bootmem(addr, chunksize);

skip:
		addr += chunksize;
		size -= chunksize;
	}
}

/*
 * Create /proc/iomem entries for bmem
 */
static int __init bmem_region_setup(void)
{
	int i, idx = 0;

	for (i = 0; i < n_bmem_regions; i++) {
		struct resource *r;
		char *name;

		if (!bmem_regions[i].valid)
			continue;

		r = kzalloc(sizeof(*r), GFP_KERNEL);
		name = kzalloc(16, GFP_KERNEL);
		if (!r || !name)
			break;

		sprintf(name, "bmem.%d", idx);
		r->start = bmem_regions[i].addr;
		r->end = bmem_regions[i].addr + bmem_regions[i].size - 1;
		r->flags = IORESOURCE_MEM;
		r->name = name;

		insert_resource(&iomem_resource, r);
		idx++;
	}
	return 0;
}

arch_initcall(bmem_region_setup);

/***********************************************************************
 * Wired TLB mappings for upper memory support
 ***********************************************************************/

#define UNIQUE_ENTRYHI(idx) (CKSEG0 + ((idx) << (PAGE_SHIFT + 1)))

extern void tlb_init(void);
extern void build_tlb_refill_handler(void);

void brcm_tlb_init(void)
{
#ifdef CONFIG_BRCM_UPPER_MEMORY
	if (smp_processor_id() == 0) {
		tlb_init();
		add_wired_entry(0x0080001f, 0x00800017, 0xc0000000, PM_256M);
	} else {
		/* bypass tlb_init() / probe_tlb() for secondary CPU */
		cpu_data[smp_processor_id()].tlbsize = cpu_data[0].tlbsize;
		build_tlb_refill_handler();
	}
#else
	tlb_init();
#endif
}

/*
 * Initialize upper memory TLB entries
 *
 * On TP1 this must happen before we set up $sp/$gp .  It is always
 * possible for stacks, task_structs, thread_info's, and other
 * important structures to be allocated out of upper memory so
 * this happens early on.
 */
asmlinkage __cpuinit void brcm_upper_tlb_setup(void)
{
#ifdef CONFIG_BRCM_UPPER_MEMORY
	int i, tlbsz;

	/* Flush TLB.  local_flush_tlb_all() is not available yet. */
	write_c0_entrylo0(0);
	write_c0_entrylo1(0);
	write_c0_pagemask(PM_DEFAULT_MASK);
	write_c0_wired(0);

	tlbsz = (read_c0_config1() >> 25) & 0x3f;
	for (i = 0; i <= tlbsz; i++) {
		write_c0_entryhi(UNIQUE_ENTRYHI(i));
		write_c0_index(i);
		mtc0_tlbw_hazard();
		tlb_write_indexed();
		tlbw_use_hazard();
	}

	write_c0_entrylo0(0x0080001f);
	write_c0_entrylo1(0x00800017);
	write_c0_entryhi(0xc0000000);
	write_c0_pagemask(PM_256M);
	write_c0_index(0);
	write_c0_wired(1);
	mtc0_tlbw_hazard();
	tlb_write_indexed();
	tlbw_use_hazard();

	write_c0_pagemask(PM_DEFAULT_MASK);
#endif
}
