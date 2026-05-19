#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Class");
MODULE_DESCRIPTION("Consumer module for usage count practice");

extern void usage_provider_hello(void);

static int __init usage_consumer_init(void)
{
    printk(KERN_INFO "usage_consumer: loaded\n");
    usage_provider_hello();
    return 0;
}

static void __exit usage_consumer_exit(void)
{
    printk(KERN_INFO "usage_consumer: unloaded\n");
}

module_init(usage_consumer_init);
module_exit(usage_consumer_exit);
