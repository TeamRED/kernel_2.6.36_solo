cmd_arch/mips/brcmstb/sysfs.o := mipsel-oe-linux-gcc -Wp,-MD,arch/mips/brcmstb/.sysfs.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sysfs)"  -D"KBUILD_MODNAME=KBUILD_STR(sysfs)"  -c -o arch/mips/brcmstb/sysfs.o arch/mips/brcmstb/sysfs.c

deps_arch/mips/brcmstb/sysfs.o := \
  arch/mips/brcmstb/sysfs.c \
    $(wildcard include/config/bmips3300.h) \
    $(wildcard include/config/bmips4380.h) \
    $(wildcard include/config/1.h) \
    $(wildcard include/config/brcm/scm/l2.h) \
    $(wildcard include/config/brcm/zscm/l2.h) \
    $(wildcard include/config/bmips5000.h) \
    $(wildcard include/config/brcm/cpu/div.h) \
    $(wildcard include/config/brcm/cpu/pll.h) \
    $(wildcard include/config/brcm/pm.h) \
    $(wildcard include/config/brcm/has/emac/0.h) \
    $(wildcard include/config/brcm/has/genet.h) \
    $(wildcard include/config/brcm/has/moca.h) \
    $(wildcard include/config/brcm/has/sata.h) \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/posix_types.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sgidefs.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cpu.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/cpu-feature-overrides.h \
    $(wildcard include/config/bcm7420.h) \
    $(wildcard include/config/mti/24k.h) \
    $(wildcard include/config/mti/34k.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cachectl.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/prefetch.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/system.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/spaces.h \
    $(wildcard include/config/brcm/upper/memory.h) \
  include/linux/const.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/dsp.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/watch.h \
    $(wildcard include/config/hardware/watchpoints.h) \
  include/linux/stat.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/atomic.h \
  include/asm-generic/atomic64.h \
  include/asm-generic/atomic-long.h \
  include/linux/math64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/current.h \
  include/asm-generic/current.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/pfn.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/io.h \
  include/asm-generic/iomap.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic/ioremap.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/topology.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/timex.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
    $(wildcard include/config/no/hz.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/uaccess.h \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/device.h \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/ops.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/device.h \
  include/asm-generic/device.h \
  include/linux/pm_wakeup.h \
    $(wildcard include/config/pm.h) \
  include/linux/ctype.h \
  include/linux/platform_device.h \
  include/linux/mod_devicetable.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/r4kcache.h \
    $(wildcard include/config/mips/mt.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cacheops.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mipsmtregs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/brcmstb.h \
    $(wildcard include/config/bcm35230a0.h) \
    $(wildcard include/config/bcm3548b0.h) \
    $(wildcard include/config/bcm3563c0.h) \
    $(wildcard include/config/bcm7038c0.h) \
    $(wildcard include/config/bcm7118c0.h) \
    $(wildcard include/config/bcm7125a0.h) \
    $(wildcard include/config/bcm7125c0.h) \
    $(wildcard include/config/bcm7325b0.h) \
    $(wildcard include/config/bcm7335b0.h) \
    $(wildcard include/config/bcm7340a0.h) \
    $(wildcard include/config/bcm7340b0.h) \
    $(wildcard include/config/bcm7342a0.h) \
    $(wildcard include/config/bcm7342b0.h) \
    $(wildcard include/config/bcm7400e0.h) \
    $(wildcard include/config/bcm7401c0.h) \
    $(wildcard include/config/bcm7403a0.h) \
    $(wildcard include/config/bcm7405b0.h) \
    $(wildcard include/config/bcm7405d0.h) \
    $(wildcard include/config/bcm7408a0.h) \
    $(wildcard include/config/bcm7420b0.h) \
    $(wildcard include/config/bcm7420c0.h) \
    $(wildcard include/config/bcm7468a0.h) \
    $(wildcard include/config/bcm7550a0.h) \
    $(wildcard include/config/bcm7601b0.h) \
    $(wildcard include/config/bcm7630b0.h) \
    $(wildcard include/config/bcm7635a0.h) \
    $(wildcard include/config/brcm/shared/uart/irq.h) \
    $(wildcard include/config/brcm/has/pcu/uarts.h) \
    $(wildcard include/config/brcm/has/wktmr.h) \
  include/linux/if_ether.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  include/linux/kmemcheck.h \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/auxvec.h \
  include/linux/prio_tree.h \
  include/linux/rbtree.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mmu.h \
  include/linux/net.h \
  include/linux/socket.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/socket.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sockios.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/sockios.h \
  include/linux/uio.h \
  include/linux/random.h \
  include/linux/ioctl.h \
  include/linux/irqnr.h \
  include/linux/fcntl.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/sysctl.h \
  include/linux/ratelimit.h \
  include/linux/textsearch.h \
  include/linux/err.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  include/linux/slab_def.h \
  include/trace/events/kmem.h \
  include/trace/events/gfpflags.h \
  include/linux/kmalloc_sizes.h \
  include/net/checksum.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/checksum.h \
  include/linux/in6.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/disable/channel/switch.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/bug.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  include/linux/mm.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/brcmstb.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memory/failure.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/pgtable.h \
    $(wildcard include/config/cpu/supports/uncached/accelerated.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/pgtable-32.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/fixmap.h \
    $(wildcard include/config/bcm63xx.h) \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/s390.h) \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/dma-mapping.h \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/setup.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/brcmapi.h \
  include/net/sock.h \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/cgroups.h) \
  include/linux/list_nulls.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
  include/linux/if.h \
  include/linux/hdlc/ioctl.h \
  include/linux/if_packet.h \
  include/linux/if_link.h \
  include/linux/netlink.h \
  include/linux/capability.h \
  include/linux/pm_qos_params.h \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/miscdevice.h \
  include/linux/major.h \
  include/linux/delay.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/delay.h \
  include/linux/rculist.h \
  include/linux/ethtool.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/net.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  include/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/dccp.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  include/linux/netfilter.h \
    $(wildcard include/config/netfilter/debug.h) \
    $(wildcard include/config/nf/nat/needed.h) \
  include/linux/in.h \
  include/net/netns/xfrm.h \
  include/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/seq_file.h \
  include/net/dsa.h \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/irqreturn.h \
  include/linux/hardirq.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/hardirq.h \
  include/asm-generic/hardirq.h \
  include/linux/irq.h \
    $(wildcard include/config/irq/per/cpu.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/intr/remap.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/sparse/irq.h) \
    $(wildcard include/config/numa/irq/desc.h) \
    $(wildcard include/config/generic/hardirqs/no//do/irq.h) \
    $(wildcard include/config/cpumasks/offstack.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/mips/mt/smtc/irqaff.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/irq.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/isadep.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/irq_regs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/hw_irq.h \
  include/linux/irq_cpustat.h \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/securityfs.h) \
  include/linux/fs.h \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/pid.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/inet.h \
  include/linux/fsnotify.h \
  include/linux/fsnotify_backend.h \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/fanotify/access/permissions.h) \
  include/linux/idr.h \
  include/linux/audit.h \
    $(wildcard include/config/change.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/cgroup/sched.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/ipcbuf.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sembuf.h \
  include/linux/signal.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  include/asm-generic/signal-defs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sigcontext.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  include/linux/key.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/linux/binfmts.h \
  include/linux/shm.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/shmparam.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/shmbuf.h \
  include/linux/msg.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/msgbuf.h \
    $(wildcard include/config/cpu/little/endian.h) \
  include/net/flow.h \
  include/linux/filter.h \
  include/linux/rculist_nulls.h \
  include/linux/poll.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/poll.h \
  include/asm-generic/poll.h \
  include/net/dst.h \
    $(wildcard include/config/net/cls/route.h) \
  include/linux/rtnetlink.h \
  include/linux/if_addr.h \
  include/linux/neighbour.h \
  include/net/neighbour.h \
  include/net/rtnetlink.h \
  include/net/netlink.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/inst.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_common.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_ebi.h \
    $(wildcard include/config/0.h) \
    $(wildcard include/config/2.h) \
    $(wildcard include/config/3.h) \
    $(wildcard include/config/4.h) \
    $(wildcard include/config/5.h) \
    $(wildcard include/config/0/mem/io/mask.h) \
    $(wildcard include/config/0/mem/io/shift.h) \
    $(wildcard include/config/0/ta/wait/mask.h) \
    $(wildcard include/config/0/ta/wait/shift.h) \
    $(wildcard include/config/0/wp/mask.h) \
    $(wildcard include/config/0/wp/shift.h) \
    $(wildcard include/config/0/wait/count/mask.h) \
    $(wildcard include/config/0/wait/count/shift.h) \
    $(wildcard include/config/0/t/hold/mask.h) \
    $(wildcard include/config/0/t/hold/shift.h) \
    $(wildcard include/config/0/t/setup/mask.h) \
    $(wildcard include/config/0/t/setup/shift.h) \
    $(wildcard include/config/0/cs/hold/mask.h) \
    $(wildcard include/config/0/cs/hold/shift.h) \
    $(wildcard include/config/0/split/cs/mask.h) \
    $(wildcard include/config/0/split/cs/shift.h) \
    $(wildcard include/config/0/mask/en/mask.h) \
    $(wildcard include/config/0/mask/en/shift.h) \
    $(wildcard include/config/0/ne/sample/mask.h) \
    $(wildcard include/config/0/ne/sample/shift.h) \
    $(wildcard include/config/0/m68k/mask.h) \
    $(wildcard include/config/0/m68k/shift.h) \
    $(wildcard include/config/0/le/mask.h) \
    $(wildcard include/config/0/le/shift.h) \
    $(wildcard include/config/0/fast/read/mask.h) \
    $(wildcard include/config/0/fast/read/shift.h) \
    $(wildcard include/config/0/fast/read/normal.h) \
    $(wildcard include/config/0/fast/read/fast/read/enable.h) \
    $(wildcard include/config/0/size/sel/mask.h) \
    $(wildcard include/config/0/size/sel/shift.h) \
    $(wildcard include/config/0/sync/mask.h) \
    $(wildcard include/config/0/sync/shift.h) \
    $(wildcard include/config/0/polarity/mask.h) \
    $(wildcard include/config/0/polarity/shift.h) \
    $(wildcard include/config/0/we/ctl/mask.h) \
    $(wildcard include/config/0/we/ctl/shift.h) \
    $(wildcard include/config/0/dest/size/mask.h) \
    $(wildcard include/config/0/dest/size/shift.h) \
    $(wildcard include/config/0/ms/inh/mask.h) \
    $(wildcard include/config/0/ms/inh/shift.h) \
    $(wildcard include/config/0/ls/inh/mask.h) \
    $(wildcard include/config/0/ls/inh/shift.h) \
    $(wildcard include/config/0/bcachen/mask.h) \
    $(wildcard include/config/0/bcachen/shift.h) \
    $(wildcard include/config/0/enable/mask.h) \
    $(wildcard include/config/0/enable/shift.h) \
    $(wildcard include/config/1/mem/io/mask.h) \
    $(wildcard include/config/1/mem/io/shift.h) \
    $(wildcard include/config/1/ta/wait/mask.h) \
    $(wildcard include/config/1/ta/wait/shift.h) \
    $(wildcard include/config/1/wp/mask.h) \
    $(wildcard include/config/1/wp/shift.h) \
    $(wildcard include/config/1/wait/count/mask.h) \
    $(wildcard include/config/1/wait/count/shift.h) \
    $(wildcard include/config/1/t/hold/mask.h) \
    $(wildcard include/config/1/t/hold/shift.h) \
    $(wildcard include/config/1/t/setup/mask.h) \
    $(wildcard include/config/1/t/setup/shift.h) \
    $(wildcard include/config/1/cs/hold/mask.h) \
    $(wildcard include/config/1/cs/hold/shift.h) \
    $(wildcard include/config/1/split/cs/mask.h) \
    $(wildcard include/config/1/split/cs/shift.h) \
    $(wildcard include/config/1/mask/en/mask.h) \
    $(wildcard include/config/1/mask/en/shift.h) \
    $(wildcard include/config/1/ne/sample/mask.h) \
    $(wildcard include/config/1/ne/sample/shift.h) \
    $(wildcard include/config/1/m68k/mask.h) \
    $(wildcard include/config/1/m68k/shift.h) \
    $(wildcard include/config/1/le/mask.h) \
    $(wildcard include/config/1/le/shift.h) \
    $(wildcard include/config/1/fast/read/mask.h) \
    $(wildcard include/config/1/fast/read/shift.h) \
    $(wildcard include/config/1/fast/read/normal.h) \
    $(wildcard include/config/1/fast/read/fast/read/enable.h) \
    $(wildcard include/config/1/size/sel/mask.h) \
    $(wildcard include/config/1/size/sel/shift.h) \
    $(wildcard include/config/1/sync/mask.h) \
    $(wildcard include/config/1/sync/shift.h) \
    $(wildcard include/config/1/polarity/mask.h) \
    $(wildcard include/config/1/polarity/shift.h) \
    $(wildcard include/config/1/we/ctl/mask.h) \
    $(wildcard include/config/1/we/ctl/shift.h) \
    $(wildcard include/config/1/dest/size/mask.h) \
    $(wildcard include/config/1/dest/size/shift.h) \
    $(wildcard include/config/1/ms/inh/mask.h) \
    $(wildcard include/config/1/ms/inh/shift.h) \
    $(wildcard include/config/1/ls/inh/mask.h) \
    $(wildcard include/config/1/ls/inh/shift.h) \
    $(wildcard include/config/1/bcachen/mask.h) \
    $(wildcard include/config/1/bcachen/shift.h) \
    $(wildcard include/config/1/enable/mask.h) \
    $(wildcard include/config/1/enable/shift.h) \
    $(wildcard include/config/2/mem/io/mask.h) \
    $(wildcard include/config/2/mem/io/shift.h) \
    $(wildcard include/config/2/ta/wait/mask.h) \
    $(wildcard include/config/2/ta/wait/shift.h) \
    $(wildcard include/config/2/wp/mask.h) \
    $(wildcard include/config/2/wp/shift.h) \
    $(wildcard include/config/2/wait/count/mask.h) \
    $(wildcard include/config/2/wait/count/shift.h) \
    $(wildcard include/config/2/t/hold/mask.h) \
    $(wildcard include/config/2/t/hold/shift.h) \
    $(wildcard include/config/2/t/setup/mask.h) \
    $(wildcard include/config/2/t/setup/shift.h) \
    $(wildcard include/config/2/cs/hold/mask.h) \
    $(wildcard include/config/2/cs/hold/shift.h) \
    $(wildcard include/config/2/split/cs/mask.h) \
    $(wildcard include/config/2/split/cs/shift.h) \
    $(wildcard include/config/2/mask/en/mask.h) \
    $(wildcard include/config/2/mask/en/shift.h) \
    $(wildcard include/config/2/ne/sample/mask.h) \
    $(wildcard include/config/2/ne/sample/shift.h) \
    $(wildcard include/config/2/m68k/mask.h) \
    $(wildcard include/config/2/m68k/shift.h) \
    $(wildcard include/config/2/le/mask.h) \
    $(wildcard include/config/2/le/shift.h) \
    $(wildcard include/config/2/fast/read/mask.h) \
    $(wildcard include/config/2/fast/read/shift.h) \
    $(wildcard include/config/2/fast/read/normal.h) \
    $(wildcard include/config/2/fast/read/fast/read/enable.h) \
    $(wildcard include/config/2/size/sel/mask.h) \
    $(wildcard include/config/2/size/sel/shift.h) \
    $(wildcard include/config/2/sync/mask.h) \
    $(wildcard include/config/2/sync/shift.h) \
    $(wildcard include/config/2/polarity/mask.h) \
    $(wildcard include/config/2/polarity/shift.h) \
    $(wildcard include/config/2/we/ctl/mask.h) \
    $(wildcard include/config/2/we/ctl/shift.h) \
    $(wildcard include/config/2/dest/size/mask.h) \
    $(wildcard include/config/2/dest/size/shift.h) \
    $(wildcard include/config/2/ms/inh/mask.h) \
    $(wildcard include/config/2/ms/inh/shift.h) \
    $(wildcard include/config/2/ls/inh/mask.h) \
    $(wildcard include/config/2/ls/inh/shift.h) \
    $(wildcard include/config/2/bcachen/mask.h) \
    $(wildcard include/config/2/bcachen/shift.h) \
    $(wildcard include/config/2/enable/mask.h) \
    $(wildcard include/config/2/enable/shift.h) \
    $(wildcard include/config/3/mem/io/mask.h) \
    $(wildcard include/config/3/mem/io/shift.h) \
    $(wildcard include/config/3/ta/wait/mask.h) \
    $(wildcard include/config/3/ta/wait/shift.h) \
    $(wildcard include/config/3/wp/mask.h) \
    $(wildcard include/config/3/wp/shift.h) \
    $(wildcard include/config/3/wait/count/mask.h) \
    $(wildcard include/config/3/wait/count/shift.h) \
    $(wildcard include/config/3/t/hold/mask.h) \
    $(wildcard include/config/3/t/hold/shift.h) \
    $(wildcard include/config/3/t/setup/mask.h) \
    $(wildcard include/config/3/t/setup/shift.h) \
    $(wildcard include/config/3/cs/hold/mask.h) \
    $(wildcard include/config/3/cs/hold/shift.h) \
    $(wildcard include/config/3/split/cs/mask.h) \
    $(wildcard include/config/3/split/cs/shift.h) \
    $(wildcard include/config/3/mask/en/mask.h) \
    $(wildcard include/config/3/mask/en/shift.h) \
    $(wildcard include/config/3/ne/sample/mask.h) \
    $(wildcard include/config/3/ne/sample/shift.h) \
    $(wildcard include/config/3/m68k/mask.h) \
    $(wildcard include/config/3/m68k/shift.h) \
    $(wildcard include/config/3/le/mask.h) \
    $(wildcard include/config/3/le/shift.h) \
    $(wildcard include/config/3/fast/read/mask.h) \
    $(wildcard include/config/3/fast/read/shift.h) \
    $(wildcard include/config/3/fast/read/normal.h) \
    $(wildcard include/config/3/fast/read/fast/read/enable.h) \
    $(wildcard include/config/3/size/sel/mask.h) \
    $(wildcard include/config/3/size/sel/shift.h) \
    $(wildcard include/config/3/sync/mask.h) \
    $(wildcard include/config/3/sync/shift.h) \
    $(wildcard include/config/3/polarity/mask.h) \
    $(wildcard include/config/3/polarity/shift.h) \
    $(wildcard include/config/3/we/ctl/mask.h) \
    $(wildcard include/config/3/we/ctl/shift.h) \
    $(wildcard include/config/3/dest/size/mask.h) \
    $(wildcard include/config/3/dest/size/shift.h) \
    $(wildcard include/config/3/ms/inh/mask.h) \
    $(wildcard include/config/3/ms/inh/shift.h) \
    $(wildcard include/config/3/ls/inh/mask.h) \
    $(wildcard include/config/3/ls/inh/shift.h) \
    $(wildcard include/config/3/bcachen/mask.h) \
    $(wildcard include/config/3/bcachen/shift.h) \
    $(wildcard include/config/3/enable/mask.h) \
    $(wildcard include/config/3/enable/shift.h) \
    $(wildcard include/config/4/mem/io/mask.h) \
    $(wildcard include/config/4/mem/io/shift.h) \
    $(wildcard include/config/4/ta/wait/mask.h) \
    $(wildcard include/config/4/ta/wait/shift.h) \
    $(wildcard include/config/4/wp/mask.h) \
    $(wildcard include/config/4/wp/shift.h) \
    $(wildcard include/config/4/wait/count/mask.h) \
    $(wildcard include/config/4/wait/count/shift.h) \
    $(wildcard include/config/4/t/hold/mask.h) \
    $(wildcard include/config/4/t/hold/shift.h) \
    $(wildcard include/config/4/t/setup/mask.h) \
    $(wildcard include/config/4/t/setup/shift.h) \
    $(wildcard include/config/4/cs/hold/mask.h) \
    $(wildcard include/config/4/cs/hold/shift.h) \
    $(wildcard include/config/4/split/cs/mask.h) \
    $(wildcard include/config/4/split/cs/shift.h) \
    $(wildcard include/config/4/mask/en/mask.h) \
    $(wildcard include/config/4/mask/en/shift.h) \
    $(wildcard include/config/4/ne/sample/mask.h) \
    $(wildcard include/config/4/ne/sample/shift.h) \
    $(wildcard include/config/4/m68k/mask.h) \
    $(wildcard include/config/4/m68k/shift.h) \
    $(wildcard include/config/4/le/mask.h) \
    $(wildcard include/config/4/le/shift.h) \
    $(wildcard include/config/4/fast/read/mask.h) \
    $(wildcard include/config/4/fast/read/shift.h) \
    $(wildcard include/config/4/fast/read/normal.h) \
    $(wildcard include/config/4/fast/read/fast/read/enable.h) \
    $(wildcard include/config/4/size/sel/mask.h) \
    $(wildcard include/config/4/size/sel/shift.h) \
    $(wildcard include/config/4/sync/mask.h) \
    $(wildcard include/config/4/sync/shift.h) \
    $(wildcard include/config/4/polarity/mask.h) \
    $(wildcard include/config/4/polarity/shift.h) \
    $(wildcard include/config/4/we/ctl/mask.h) \
    $(wildcard include/config/4/we/ctl/shift.h) \
    $(wildcard include/config/4/dest/size/mask.h) \
    $(wildcard include/config/4/dest/size/shift.h) \
    $(wildcard include/config/4/ms/inh/mask.h) \
    $(wildcard include/config/4/ms/inh/shift.h) \
    $(wildcard include/config/4/ls/inh/mask.h) \
    $(wildcard include/config/4/ls/inh/shift.h) \
    $(wildcard include/config/4/bcachen/mask.h) \
    $(wildcard include/config/4/bcachen/shift.h) \
    $(wildcard include/config/4/enable/mask.h) \
    $(wildcard include/config/4/enable/shift.h) \
    $(wildcard include/config/5/mem/io/mask.h) \
    $(wildcard include/config/5/mem/io/shift.h) \
    $(wildcard include/config/5/ta/wait/mask.h) \
    $(wildcard include/config/5/ta/wait/shift.h) \
    $(wildcard include/config/5/wp/mask.h) \
    $(wildcard include/config/5/wp/shift.h) \
    $(wildcard include/config/5/wait/count/mask.h) \
    $(wildcard include/config/5/wait/count/shift.h) \
    $(wildcard include/config/5/t/hold/mask.h) \
    $(wildcard include/config/5/t/hold/shift.h) \
    $(wildcard include/config/5/t/setup/mask.h) \
    $(wildcard include/config/5/t/setup/shift.h) \
    $(wildcard include/config/5/cs/hold/mask.h) \
    $(wildcard include/config/5/cs/hold/shift.h) \
    $(wildcard include/config/5/split/cs/mask.h) \
    $(wildcard include/config/5/split/cs/shift.h) \
    $(wildcard include/config/5/mask/en/mask.h) \
    $(wildcard include/config/5/mask/en/shift.h) \
    $(wildcard include/config/5/ne/sample/mask.h) \
    $(wildcard include/config/5/ne/sample/shift.h) \
    $(wildcard include/config/5/m68k/mask.h) \
    $(wildcard include/config/5/m68k/shift.h) \
    $(wildcard include/config/5/le/mask.h) \
    $(wildcard include/config/5/le/shift.h) \
    $(wildcard include/config/5/fast/read/mask.h) \
    $(wildcard include/config/5/fast/read/shift.h) \
    $(wildcard include/config/5/fast/read/normal.h) \
    $(wildcard include/config/5/fast/read/fast/read/enable.h) \
    $(wildcard include/config/5/size/sel/mask.h) \
    $(wildcard include/config/5/size/sel/shift.h) \
    $(wildcard include/config/5/sync/mask.h) \
    $(wildcard include/config/5/sync/shift.h) \
    $(wildcard include/config/5/polarity/mask.h) \
    $(wildcard include/config/5/polarity/shift.h) \
    $(wildcard include/config/5/we/ctl/mask.h) \
    $(wildcard include/config/5/we/ctl/shift.h) \
    $(wildcard include/config/5/dest/size/mask.h) \
    $(wildcard include/config/5/dest/size/shift.h) \
    $(wildcard include/config/5/ms/inh/mask.h) \
    $(wildcard include/config/5/ms/inh/shift.h) \
    $(wildcard include/config/5/ls/inh/mask.h) \
    $(wildcard include/config/5/ls/inh/shift.h) \
    $(wildcard include/config/5/bcachen/mask.h) \
    $(wildcard include/config/5/bcachen/shift.h) \
    $(wildcard include/config/5/enable/mask.h) \
    $(wildcard include/config/5/enable/shift.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_hif_cpu_intr1.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_hif_cpu_tp1_intr1.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_hif_intr2.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_irq0.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_irq1.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_memc_0_ddr.h \
    $(wildcard include/config/reserved0/mask.h) \
    $(wildcard include/config/reserved0/shift.h) \
    $(wildcard include/config/device/tech/mask.h) \
    $(wildcard include/config/device/tech/shift.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_nand.h \
    $(wildcard include/config/mask.h) \
    $(wildcard include/config/shift.h) \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/config/lock/mask.h) \
    $(wildcard include/config/lock/mask.h) \
    $(wildcard include/config/config/lock/shift.h) \
    $(wildcard include/config/lock/shift.h) \
    $(wildcard include/config/page/size/mask.h) \
    $(wildcard include/config/page/size/shift.h) \
    $(wildcard include/config/page/size/pg/size/512.h) \
    $(wildcard include/config/page/size/pg/size/2kb.h) \
    $(wildcard include/config/block/size/mask.h) \
    $(wildcard include/config/block/size/shift.h) \
    $(wildcard include/config/block/size/bk/size/512kb.h) \
    $(wildcard include/config/block/size/bk/size/128kb.h) \
    $(wildcard include/config/block/size/bk/size/16kb.h) \
    $(wildcard include/config/block/size/bk/size/8kb.h) \
    $(wildcard include/config/device/size/mask.h) \
    $(wildcard include/config/device/size/shift.h) \
    $(wildcard include/config/device/size/dvc/size/4mb.h) \
    $(wildcard include/config/device/size/dvc/size/8mb.h) \
    $(wildcard include/config/device/size/dvc/size/16mb.h) \
    $(wildcard include/config/device/size/dvc/size/32mb.h) \
    $(wildcard include/config/device/size/dvc/size/64mb.h) \
    $(wildcard include/config/device/size/dvc/size/128mb.h) \
    $(wildcard include/config/device/size/dvc/size/256mb.h) \
    $(wildcard include/config/device/size/dvc/size/512mb.h) \
    $(wildcard include/config/device/size/dvc/size/1gb.h) \
    $(wildcard include/config/device/size/dvc/size/2gb.h) \
    $(wildcard include/config/device/size/dvc/size/4gb.h) \
    $(wildcard include/config/device/size/dvc/size/8gb.h) \
    $(wildcard include/config/device/size/dvc/size/16gb.h) \
    $(wildcard include/config/device/size/dvc/size/32gb.h) \
    $(wildcard include/config/device/size/dvc/size/64gb.h) \
    $(wildcard include/config/device/size/dvc/size/128gb.h) \
    $(wildcard include/config/device/width/mask.h) \
    $(wildcard include/config/device/width/shift.h) \
    $(wildcard include/config/device/width/dvc/width/8.h) \
    $(wildcard include/config/device/width/dvc/width/16.h) \
    $(wildcard include/config/ful/adr/bytes/mask.h) \
    $(wildcard include/config/ful/adr/bytes/shift.h) \
    $(wildcard include/config/reserved1/mask.h) \
    $(wildcard include/config/reserved1/shift.h) \
    $(wildcard include/config/col/adr/bytes/mask.h) \
    $(wildcard include/config/col/adr/bytes/shift.h) \
    $(wildcard include/config/reserved2/mask.h) \
    $(wildcard include/config/reserved2/shift.h) \
    $(wildcard include/config/blk/adr/bytes/mask.h) \
    $(wildcard include/config/blk/adr/bytes/shift.h) \
    $(wildcard include/config/reserved3/mask.h) \
    $(wildcard include/config/reserved3/shift.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_pci_cfg.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_sun_top_ctrl.h \
    $(wildcard include/config/serial/adr/cfg/mask.h) \
    $(wildcard include/config/serial/adr/cfg/shift.h) \
    $(wildcard include/config/probe/mux/sel/mask.h) \
    $(wildcard include/config/probe/mux/sel/shift.h) \
    $(wildcard include/config/dly/disable/mask.h) \
    $(wildcard include/config/dly/disable/shift.h) \
    $(wildcard include/config/spi/mode/mask.h) \
    $(wildcard include/config/spi/mode/shift.h) \
    $(wildcard include/config/ssp/module/enable/mask.h) \
    $(wildcard include/config/ssp/module/enable/shift.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_timer.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_uarta.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_uartb.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_uartc.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_usb_ctrl.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_usb_ehci.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_usb_ehci1.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_usb_ohci.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/brcmstb/7325b0/bchp_usb_ohci1.h \

arch/mips/brcmstb/sysfs.o: $(deps_arch/mips/brcmstb/sysfs.o)

$(deps_arch/mips/brcmstb/sysfs.o):
