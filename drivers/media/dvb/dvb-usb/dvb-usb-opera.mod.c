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
"depends=dvb-usb,stv0299,i2c-core,dvb-pll,firmware_class";

MODULE_ALIAS("usb:v04B4p2830d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v695Cp3829d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "3AF32197EF713F20A106B4F");
