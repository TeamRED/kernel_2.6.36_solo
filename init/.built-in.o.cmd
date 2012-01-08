cmd_init/built-in.o :=  mipsel-oe-linux-ld  -m elf32ltsmip   -r -o init/built-in.o init/main.o init/version.o init/mounts.o init/initramfs.o init/calibrate.o 
