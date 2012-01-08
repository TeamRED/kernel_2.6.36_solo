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
"depends=dvb-usb,dvb-core";

MODULE_ALIAS("usb:v3344p1122d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v3344p1120d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "F6752BBB52D6DF896C657DD");
