cmd_arch/mips/kernel/r4k_fpu.o := mipsel-oe-linux-gcc -Wp,-MD,arch/mips/kernel/.r4k_fpu.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic         -c -o arch/mips/kernel/r4k_fpu.o arch/mips/kernel/r4k_fpu.S

deps_arch/mips/kernel/r4k_fpu.o := \
  arch/mips/kernel/r4k_fpu.S \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/mips32/compat.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sgidefs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/fpregdef.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/linkage.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb/war.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/regdef.h \

arch/mips/kernel/r4k_fpu.o: $(deps_arch/mips/kernel/r4k_fpu.o)

$(deps_arch/mips/kernel/r4k_fpu.o):
