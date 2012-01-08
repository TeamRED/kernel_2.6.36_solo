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
"depends=cx22702,dvb-usb,lgdt330x,dib0070,zl10353,mxl5005s,tuner-simple,dib7000p,max2165,dvb-pll,tuner-xc2028,mt352,lgs8gxx,atbm8830";

MODULE_ALIAS("usb:v1660p0932d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pD500d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pD501d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB50d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB51d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB00d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB01d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB10d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB11d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB54d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB55d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB58d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB59d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB78d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB70d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB71d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApA868d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FE9pDB98d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0572p86D6d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0572pD811d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "53A118991E392629160934E");
