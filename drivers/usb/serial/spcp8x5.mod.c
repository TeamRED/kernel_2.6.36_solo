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
"depends=usbserial";

MODULE_ALIAS("usb:v0471p081Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04FCp0204d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04FCp0231d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04FCp0235d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04FCp0201d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "BDBC1FA61901746141903C9");
