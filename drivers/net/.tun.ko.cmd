cmd_drivers/net/tun.ko := mipsel-oe-linux-ld -r  -m elf32ltsmip -T /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/scripts/module-common.lds --build-id  -o drivers/net/tun.ko drivers/net/tun.o drivers/net/tun.mod.o