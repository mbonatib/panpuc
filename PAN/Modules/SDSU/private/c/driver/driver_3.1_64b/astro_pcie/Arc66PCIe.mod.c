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
__used
__attribute__((section("__versions"))) = {
	{ 0x28950ef1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe5d95985, __VMLINUX_SYMBOL_STR(param_ops_ulong) },
	{ 0x2cb61da5, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0x450c190, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xacbfe419, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x99487493, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0x7e5df8e3, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x521445b, __VMLINUX_SYMBOL_STR(list_del) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x5f675a65, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xdacd8618, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x196103b4, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x8055d058, __VMLINUX_SYMBOL_STR(pci_iomap) },
	{ 0xc3fc2f, __VMLINUX_SYMBOL_STR(pci_request_regions) },
	{ 0x46734db7, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0xc483a55a, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x65e75cb6, __VMLINUX_SYMBOL_STR(__list_del_entry) },
	{ 0x343a1a8, __VMLINUX_SYMBOL_STR(__list_add) },
	{ 0x41ec4c1a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x98ab5c8d, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x434693dd, __VMLINUX_SYMBOL_STR(alloc_pages_current) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x7f59e59, __VMLINUX_SYMBOL_STR(pci_write_config_dword) },
	{ 0x8bd590db, __VMLINUX_SYMBOL_STR(pci_write_config_word) },
	{ 0xc9426d6d, __VMLINUX_SYMBOL_STR(pci_write_config_byte) },
	{ 0xebfdcb96, __VMLINUX_SYMBOL_STR(pci_read_config_dword) },
	{ 0xc2560ac2, __VMLINUX_SYMBOL_STR(pci_read_config_word) },
	{ 0xc2f7c1b1, __VMLINUX_SYMBOL_STR(pci_read_config_byte) },
	{ 0x6729d3df, __VMLINUX_SYMBOL_STR(__get_user_4) },
	{ 0x71de9b3f, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xe484e35f, __VMLINUX_SYMBOL_STR(ioread32) },
	{ 0x77e2f33, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x71e3cecb, __VMLINUX_SYMBOL_STR(up) },
	{ 0xb2fd5ceb, __VMLINUX_SYMBOL_STR(__put_user_4) },
	{ 0x9f6b4c62, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x436c2179, __VMLINUX_SYMBOL_STR(iowrite32) },
	{ 0xe84cb310, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x45449b56, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0x84c274f9, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0xda3e43d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0xd52bf1ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xc3bf75bc, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x3fa89e8f, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xcf73ce21, __VMLINUX_SYMBOL_STR(pci_release_regions) },
	{ 0x1c3e657e, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0x7cf5b2b3, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x127b8725, __VMLINUX_SYMBOL_STR(pci_iounmap) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xa281370e, __VMLINUX_SYMBOL_STR(__free_pages) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v000010B5d00009056sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "E3324497894D8B1DE6F3926");
MODULE_INFO(rhelversion, "7.3");
