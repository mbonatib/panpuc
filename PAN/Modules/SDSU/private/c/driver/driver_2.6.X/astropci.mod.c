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

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x6109265e, "struct_module" },
	{ 0x1fc91fb2, "request_irq" },
	{ 0x3762cb6e, "ioremap_nocache" },
	{ 0xe647af55, "pci_request_regions" },
	{ 0x7df4bc8a, "pci_enable_device" },
	{ 0x2d11a26, "module_put" },
	{ 0x1ea7b2f7, "pci_bus_write_config_byte" },
	{ 0x518eb764, "per_cpu__cpu_number" },
	{ 0xf407c0b4, "cdev_add" },
	{ 0xd3bf4aa5, "cdev_init" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x8cd6d8a0, "__pci_register_driver" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0xa03d6a57, "__get_user_4" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0x38e8af44, "remap_pfn_range" },
	{ 0x15e36090, "boot_cpu_data" },
	{ 0x948cde9, "num_physpages" },
	{ 0xc8be7b15, "_spin_lock" },
	{ 0x17d59d01, "schedule_timeout" },
	{ 0x2d61163e, "per_cpu__current_task" },
	{ 0x72270e35, "do_gettimeofday" },
	{ 0xa46dc984, "_spin_unlock_irqrestore" },
	{ 0x6e185827, "_spin_lock_irqsave" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x23302c74, "pci_disable_device" },
	{ 0x6b58b373, "pci_release_regions" },
	{ 0xedc03953, "iounmap" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xebbb40ba, "pci_unregister_driver" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xaa4e2f81, "cdev_del" },
	{ 0x1b7d4074, "printk" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001057d00001801sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "D82C38B06FCE5F694F932D2");
