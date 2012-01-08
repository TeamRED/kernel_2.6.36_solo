cmd_fs/jbd/jbd.ko := mipsel-oe-linux-ld -r  -m elf32ltsmip -T /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/scripts/module-common.lds --build-id  -o fs/jbd/jbd.ko fs/jbd/jbd.o fs/jbd/jbd.mod.o
