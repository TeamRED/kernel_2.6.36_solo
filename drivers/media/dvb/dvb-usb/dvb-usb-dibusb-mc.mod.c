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
"depends=dvb-usb-dibusb-common,dvb-usb";

MODULE_ALIAS("usb:v10B8p0BC6d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B8p0BC7d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p8109d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p810Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04CApF000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04CApF001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vEB1ApE360d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vEB1ApE361d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0BC6d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v5032p0BC7d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p810Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05D8p810Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0413p6025d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0413p6026d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B9p5000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B9p5001d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "AEC06EF953644B9A2F45E22");
