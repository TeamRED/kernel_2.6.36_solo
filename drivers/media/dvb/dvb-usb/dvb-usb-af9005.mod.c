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
"depends=qt1010,dvb-usb,mt2060";

MODULE_ALIAS("usb:v15A4p9020d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp0055d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B9p6000d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "71BFE0616AC70A08F2E0D64");
