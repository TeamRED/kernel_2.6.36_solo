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

MODULE_INFO(staging, "Y");

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0BDAp8192d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8709d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07AAp0043d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v050Dp805Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0031d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9201d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2001p3301d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5A57p0290d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v043Ep7A01d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "6F52F47ADA24F29A7DF2625");
