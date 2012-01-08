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
"depends=eeprom_93cx6,firmware_class";

MODULE_ALIAS("usb:v0BDAp8171d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8172d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8173d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8174d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8712d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BDAp8713d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07AAp0047d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3303d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3302d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3300d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9603d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p9605d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v050Dp815Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v06F8pE031d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v7392p7611d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v7392p7612d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v7392p7622d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0045d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0E66p0015d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0E66p0016d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B05p1786d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p004Bd*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "EBF5E6F580EBE043D5DA490");
