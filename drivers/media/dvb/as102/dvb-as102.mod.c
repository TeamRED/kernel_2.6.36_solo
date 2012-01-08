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
"depends=dvb-core,firmware_class";

MODULE_ALIAS("usb:v1BA6p0001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2013p0246d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0FD9p002Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2137p0001d*dc*dsc*dp*ic*isc*ip*");
