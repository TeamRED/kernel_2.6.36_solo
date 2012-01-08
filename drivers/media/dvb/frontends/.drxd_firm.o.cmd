cmd_drivers/media/dvb/frontends/drxd_firm.o := mipsel-unknown-linux-gnu-gcc -Wp,-MD,drivers/media/dvb/frontends/.drxd_firm.o.d  -nostdinc -isystem /home/kajgan/Skrivebord/kernel_2.6.36/mipsel-unknown-linux-gnu/bin/../lib/gcc/mipsel-unknown-linux-gnu/4.3.2/include -I/home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80001000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/mach-brcmstb -I/home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -Idrivers/media/dvb/dvb-core/ -Idrivers/media/common/tuners/  -DMODULE -mlong-calls  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(drxd_firm)"  -D"KBUILD_MODNAME=KBUILD_STR(drxd)"  -c -o drivers/media/dvb/frontends/drxd_firm.o drivers/media/dvb/frontends/drxd_firm.c

deps_drivers/media/dvb/frontends/drxd_firm.o := \
  drivers/media/dvb/frontends/drxd_firm.c \
    $(wildcard include/config//a.h) \
    $(wildcard include/config/fr/enable//m.h) \
    $(wildcard include/config/freqscan//m.h) \
    $(wildcard include/config/div/echo/enable//m.h) \
    $(wildcard include/config/slave//m.h) \
    $(wildcard include/config/div/blank/enable//m.h) \
  drivers/media/dvb/frontends/drxd_firm.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/bitsperlong.h \
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
  /home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/posix_types.h \
  /home/kajgan/Skrivebord/kernel_2.6.36/arch/mips/include/asm/sgidefs.h \
  drivers/media/dvb/frontends/drxd_map_firm.h \

drivers/media/dvb/frontends/drxd_firm.o: $(deps_drivers/media/dvb/frontends/drxd_firm.o)

$(deps_drivers/media/dvb/frontends/drxd_firm.o):
