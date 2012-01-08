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
"depends=dvb-usb,ec100,mxl5005s";

MODULE_ALIAS("usb:v18B4p1689d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18B4pFFFAd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18B4pFFFBd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18B4p1001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18B4p1002d*dc*dsc*dp*ic*isc*ip*");
