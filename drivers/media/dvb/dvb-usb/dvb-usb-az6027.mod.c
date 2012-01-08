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
"depends=dvb-usb,i2c-core,stb0899,dvb-core,stb6100";

MODULE_ALIAS("usb:v13D3p3275d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp10A4d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp10ACd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14F7p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14F7p0002d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FD9p002Ad*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "88876D495A504E3B757441D");
