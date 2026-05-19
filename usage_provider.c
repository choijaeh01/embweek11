#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Class");
MODULE_DESCRIPTION("Provider module for usage count practice");

void usage_provider_hello(void);

void usage_provider_hello(void)
{
    printk(KERN_INFO "usage_provider: exported function called\n");
}
EXPORT_SYMBOL(usage_provider_hello);

static int __init usage_provider_init(void)
{
    printk(KERN_INFO "usage_provider: loaded\n");
    return 0;
}

static void __exit usage_provider_exit(void)
{
    printk(KERN_INFO "usage_provider: unloaded\n");
}

module_init(usage_provider_init);
module_exit(usage_provider_exit);
