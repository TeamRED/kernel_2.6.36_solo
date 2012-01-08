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
"depends=mac80211,led-class,ath,firmware_class,cfg80211";

MODULE_ALIAS("usb:v0CF3p9170d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CF3p1001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CF3p1002d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CF3p1010d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CF3p1011d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:vCACEp0300d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3C10d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07D1p3A09d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0846p9040d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0846p9010d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0846p9001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEp1221d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435p0804d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435p0326d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3417d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CDEp0023d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CDEp0026d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CDEp0027d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApF522d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2019p5304d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v04BBp093Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v057Cp8401d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0409p0249d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v057Cp8402d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1668p1200d*dc*dsc*dp*ic*isc*ip*");
