#!/bin/sh

export CROSSTOOLS=/home/kajgan/Skrivebord/GIGA_KER/mipsel-oe-linux/
export PATH=.:$CROSSTOOLS/bin:$PATH
export INSTALL_MOD_PATH=images

if [ $# -lt 1 ]; then
    echo "Usage: build.sh [up/clean]"
    exit 1
fi

if [ $1 == "config" ]; then
	make menuconfig
fi

if [ $1 == "up" ]; then
	make
	gzip -9fc vmlinux > vmlinux.gz
fi

if [ $1 == "lib" ]; then
	make modules_install
fi

if [ $1 == "clean" ]; then
	make clean
fi

if [ $1 == "distclean" ]; then
	make distclean
fi
