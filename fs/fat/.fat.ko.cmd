cmd_fs/fat/fat.ko := mipsel-oe-linux-ld -r  -m elf32ltsmip -T /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/scripts/module-common.lds --build-id  -o fs/fat/fat.ko fs/fat/fat.o fs/fat/fat.mod.o
