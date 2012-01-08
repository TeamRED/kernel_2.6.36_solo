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
"depends=dvb-usb,dvb-usb-dibusb-common,dvb-pll,mt352";

MODULE_ALIAS("usb:v15F4p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v15F4p0015d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "972423F778C4D40DD9429EF");
