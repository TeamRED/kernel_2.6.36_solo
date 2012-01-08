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
"depends=usbserial";

MODULE_ALIAS("usb:v050Dp0103d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v056Cp8007d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0565p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0921p1000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0921p1200d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v050Dp1203d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "C8A3AE410607C49FC8D67BA");
