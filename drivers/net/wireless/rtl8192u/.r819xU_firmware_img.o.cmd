cmd_drivers/net/wireless/rtl8192u/r819xU_firmware_img.o := mipsel-unknown-linux-gnu-gcc -Wp,-MD,drivers/net/wireless/rtl8192u/.r819xU_firmware_img.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/mipsel-unknown-linux-gnu/bin/../lib/gcc/mipsel-unknown-linux-gnu/4.3.2/include -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -std=gnu89 -O2 -DCONFIG_FORCE_HARD_FLOAT=y -DJACKSON_NEW_8187 -DJACKSON_NEW_RX -DTHOMAS_BEACON -DTHOMAS_TASKLET -DTHOMAS_SKB -DTHOMAS_TURBO -DUSE_ONE_PIPE -DENABLE_DOT11D -Idrivers/staging/rtl8192u/ieee80211  -DMODULE -mlong-calls  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(r819xU_firmware_img)"  -D"KBUILD_MODNAME=KBUILD_STR(r8192u_usb)"  -c -o drivers/net/wireless/rtl8192u/r819xU_firmware_img.o drivers/net/wireless/rtl8192u/r819xU_firmware_img.c

deps_drivers/net/wireless/rtl8192u/r819xU_firmware_img.o := \
  drivers/net/wireless/rtl8192u/r819xU_firmware_img.c \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/posix_types.h \
  /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/sgidefs.h \

drivers/net/wireless/rtl8192u/r819xU_firmware_img.o: $(deps_drivers/net/wireless/rtl8192u/r819xU_firmware_img.o)

$(deps_drivers/net/wireless/rtl8192u/r819xU_firmware_img.o):
