#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Class");
MODULE_DESCRIPTION("Module metadata practice for Chapter 9");
MODULE_VERSION("1.0");

static int __init module_info_init(void)
{
    printk(KERN_INFO "module_info: loaded\n");
    printk(KERN_INFO "module_info: check this module with modinfo module_info.ko\n");
    return 0;
}

static void __exit module_info_exit(void)
{
    printk(KERN_INFO "module_info: unloaded\n");
}

module_init(module_info_init);
module_exit(module_info_exit);

