#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=dvb-usb-dibusb-common,dvb-usb,i2c-core,dib3000mb,dvb-pll";

MODULE_ALIAS("usb:v14AAp0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0002d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v185BpD000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v185BpD001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v145Fp010Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B8p0BB8d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B8p0BB9d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vEB1Ap17DEd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vEB1Ap17DFd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0FA0d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0FA1d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0BB8d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0BB9d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1025p005Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1025p005Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3201d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3202d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1822p3201d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1822p3202d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8105d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8106d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8107d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8108d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v06E1pA333d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v06E1pA334d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0458p701Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0458p701Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vEB2Ap17DEd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8109d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p810Ad*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "66E4338BBBFB66329A0E1B1");
