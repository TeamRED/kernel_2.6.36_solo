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
"depends=dvb-core,tda1004x,stv0297,tda8083,lnbp21,ves1820,i2c-core,cx22700,stv0299,firmware_class";

MODULE_ALIAS("usb:v0B48p1003d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B48p1004d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B48p1005d*dc*dsc*dp*ic*isc*ip*");
