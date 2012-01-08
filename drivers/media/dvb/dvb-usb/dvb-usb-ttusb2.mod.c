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
"depends=tda827x,dvb-usb,tda10023,tda10086,tda826x,lnbp21";

MODULE_ALIAS("usb:v2304p020Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2304p0222d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B48p3006d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B48p300Dd*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "D8EEA1B3ED6A3D7C51B4415");
