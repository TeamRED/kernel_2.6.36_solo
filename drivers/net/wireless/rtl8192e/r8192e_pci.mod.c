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
"depends=firmware_class";

MODULE_ALIAS("pci:v000010ECd00008192sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000007AAd00000044sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000007AAd00000047sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "03027B698FE2553A6144177");
