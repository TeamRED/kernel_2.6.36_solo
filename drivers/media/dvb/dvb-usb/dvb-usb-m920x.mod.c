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
"depends=qt1010,tda827x,dvb-usb,tda1004x,tuner-simple,mt352";

MODULE_ALIAS("usb:v0DB0p5580d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10FDp1513d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10FDp0514d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10FDp0513d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1498p9206d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1498pA090d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3211d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "336DF68E9B1EB49202B6A1F");
