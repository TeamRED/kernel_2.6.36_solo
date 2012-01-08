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

MODULE_ALIAS("usb:v14AAp0201d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0301d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0222d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0221d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp022Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp022Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0225d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0226d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14AAp0220d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v18F3p0220d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "4A58F8590FC3300C2B406E5");
