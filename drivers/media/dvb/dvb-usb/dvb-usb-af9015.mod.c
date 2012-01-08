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
"depends=qt1010,dvb-usb,mxl5005s,i2c-core,tda18218,mt2060,mc44s803,dvb-pll,mxl5007t,af9013,tda18271";

MODULE_ALIAS("usb:v15A4p9015d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v15A4p9016d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0413p6029d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2304p022Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE399d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3226d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13D3p3237d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp0069d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pC160d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApA815d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1AE7p0381d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1462p8801d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp8150d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v10B9p8000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApA309d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1462p8807d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE396d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE39Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE395d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v15A4p901Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp850Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CApA805d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE397d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pC810d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0458p4012d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE400d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pC161d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE39Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE402d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0413p6A04d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE383d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1B80pE39Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp815Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp0097d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CCDp0099d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07CAp850Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1F4Dp9016d*dc*dsc*dp*ic*isc*ip*");
