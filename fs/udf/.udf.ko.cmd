cmd_fs/udf/udf.ko := mipsel-oe-linux-ld -r  -m elf32ltsmip -T /home/kajgan/Skrivebord/GIGA_KER/kernel_2.6.36_SOLO/scripts/module-common.lds --build-id  -o fs/udf/udf.ko fs/udf/udf.o fs/udf/udf.mod.o
