cmd_arch/mips/kernel/head.o := mipsel-oe-linux-gcc -Wp,-MD,arch/mips/kernel/.head.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic         -c -o arch/mips/kernel/head.o arch/mips/kernel/head.S

deps_arch/mips/kernel/head.o := \
  arch/mips/kernel/head.S \
    $(wildcard include/config/mapped/kernel.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/no/except/fill.h) \
    $(wildcard include/config/boot/raw.h) \
    $(wildcard include/config/smp.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/spaces.h \
    $(wildcard include/config/brcm/upper/memory.h) \
  include/linux/const.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/dma/noncoherent.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sgidefs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asmmacro.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asmmacro-32.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/regdef.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/fpregdef.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
  include/linux/linkage.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/linkage.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/war.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
    $(wildcard include/config/trace/irqflags.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/page.h \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/stackframe.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
    $(wildcard include/config/mips/pgd/c0/context.h) \
    $(wildcard include/config/cpu/jump/workarounds.h) \
    $(wildcard include/config/cpu/loongson2f.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/kernel-entry-init.h \

arch/mips/kernel/head.o: $(deps_arch/mips/kernel/head.o)

$(deps_arch/mips/kernel/head.o):
