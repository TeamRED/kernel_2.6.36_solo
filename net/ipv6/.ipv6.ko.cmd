cmd_net/ipv6/ipv6.ko := mipsel-unknown-linux-gnu-ld -r  -m elf32ltsmip -T /home/kajgan/Skrivebord/kernel_2.6.36/scripts/module-common.lds --build-id  -o net/ipv6/ipv6.ko net/ipv6/ipv6.o net/ipv6/ipv6.mod.o