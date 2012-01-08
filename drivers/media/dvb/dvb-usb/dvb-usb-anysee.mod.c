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
"depends=dvb-usb,i2c-core,stv0900,zl10353,isl6423,tda18212,tda10023,cx24116,dvb-pll,stv6110,mt352";

MODULE_ALIAS("usb:v04B4p861Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1C73p861Fd*dc*dsc*dp*ic*isc*ip*");
