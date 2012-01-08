cmd_net/unix/unix.o := mipsel-oe-linux-ld  -m elf32ltsmip   -r -o net/unix/unix.o net/unix/af_unix.o net/unix/garbage.o net/unix/sysctl_net_unix.o 
