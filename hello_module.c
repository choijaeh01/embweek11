/* Module example
FILE : hello_module.c */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int module_begin(void)
{
    printk(KERN_ALERT "Hello, Wellcome to module!!\n");
    return 0;
}

static void module_end(void)
{
    printk(KERN_ALERT "Goodbye, Exit module!!\n");
}

module_init(module_begin);
module_exit(module_end);
