	.file	1 "bounds.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3

 # -G value = 0, Arch = mips32r2, ISA = 33
 # GNU C (GCC) version 4.4.3 (mipsel-oe-linux)
 #	compiled by GNU C version 4.4.5, GMP version 4.3.1, MPFR version 2.3.1.
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed:  -nostdinc
 # -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include
 # -Iinclude
 # -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-brcmstb
 # -I/home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/arch/mips/include/asm/mach-generic
 # -iprefix
 # /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/
 # -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0xffffffff80001000 -DDATAOFFSET=0
 # -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(bounds)
 # -DKBUILD_MODNAME=KBUILD_STR(bounds) -isystem
 # /home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/bin/../lib/gcc/mipsel-oe-linux/4.4.3/include
 # -include include/generated/autoconf.h -MD kernel/.bounds.s.d
 # kernel/bounds.c -G 0 -mno-check-zero-division -mabi=32 -mno-abicalls
 # -msoft-float -march=mips32r2 -mllsc -mplt -mno-shared -auxbase-strip
 # kernel/bounds.s -O2 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs
 # -Werror-implicit-function-declaration -Wno-format-security
 # -Wframe-larger-than=1024 -Wdeclaration-after-statement -Wno-pointer-sign
 # -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
 # -ffunction-sections -fno-pic -ffreestanding -fno-stack-protector
 # -fomit-frame-pointer -fno-strict-overflow -fconserve-stack -fverbose-asm
 # options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
 # -fbranch-count-reg -fcaller-saves -fcprop-registers -fcrossjumping
 # -fcse-follow-jumps -fdefer-pop -fdelayed-branch -fearly-inlining
 # -feliminate-unused-debug-types -fexpensive-optimizations
 # -fforward-propagate -ffunction-cse -ffunction-sections -fgcse -fgcse-lm
 # -fguess-branch-probability -fident -fif-conversion -fif-conversion2
 # -findirect-inlining -finline -finline-functions-called-once
 # -finline-small-functions -fipa-cp -fipa-pure-const -fipa-reference
 # -fira-share-save-slots -fira-share-spill-slots -fivopts
 # -fkeep-static-consts -fleading-underscore -fmath-errno -fmerge-constants
 # -fmerge-debug-strings -fmove-loop-invariants -fomit-frame-pointer
 # -foptimize-register-move -foptimize-sibling-calls -fpcc-struct-return
 # -fpeephole -fpeephole2 -fregmove -freorder-blocks -freorder-functions
 # -frerun-cse-after-loop -fsched-interblock -fsched-spec
 # -fsched-stalled-insns-dep -fschedule-insns -fschedule-insns2
 # -fsigned-zeros -fsplit-ivs-in-unroller -fsplit-wide-types -fthread-jumps
 # -ftoplevel-reorder -ftrapping-math -ftree-builtin-call-dce -ftree-ccp
 # -ftree-ch -ftree-copy-prop -ftree-copyrename -ftree-cselim -ftree-dce
 # -ftree-dominator-opts -ftree-dse -ftree-fre -ftree-loop-im
 # -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
 # -ftree-pre -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-sra
 # -ftree-switch-conversion -ftree-ter -ftree-vect-loop-version -ftree-vrp
 # -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss -mdivide-traps
 # -mdouble-float -mel -mexplicit-relocs -mextern-sdata -mfp-exceptions
 # -mfp32 -mfused-madd -mglibc -mgp32 -mgpopt -mllsc -mlocal-sdata -mlong32
 # -mno-mips16 -mno-mips3d -mplt -msoft-float -msplit-addresses

 # Compiler executable checksum: ae8cda1c9b67af697db7da8df2f0c05a

	.section	.text.foo,"ax",@progbits
	.align	2
	.globl	foo
	.set	nomips16
	.ent	foo
	.type	foo, @function
foo:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
 # 16 "kernel/bounds.c" 1
	
->NR_PAGEFLAGS 23 __NR_PAGEFLAGS	 #
 # 0 "" 2
 # 17 "kernel/bounds.c" 1
	
->MAX_NR_ZONES 2 __MAX_NR_ZONES	 #
 # 0 "" 2
#NO_APP
	j	$31
	.end	foo
	.size	foo, .-foo
	.ident	"GCC: (GNU) 4.4.3"
