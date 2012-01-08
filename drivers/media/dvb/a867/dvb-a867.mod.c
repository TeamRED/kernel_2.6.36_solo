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
"depends=dvb-usb";

MODULE_ALIAS("usb:v07CApA333d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApB867d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp1867d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp0337d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApA867d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp0867d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApF337d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "A6C097CB6E45FEB61A0B06C");
