cmd_arch/mips/lib/csum_partial.o := mipsel-oe-linux-gcc -Wp,-MD,arch/mips/lib/.csum_partial.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic        -c -o arch/mips/lib/csum_partial.o arch/mips/lib/csum_partial.S

deps_arch/mips/lib/csum_partial.o := \
  arch/mips/lib/csum_partial.S \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/linux/errno.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sgidefs.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/regdef.h \

arch/mips/lib/csum_partial.o: $(deps_arch/mips/lib/csum_partial.o)

$(deps_arch/mips/lib/csum_partial.o):
