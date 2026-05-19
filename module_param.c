#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static char *name = "embedded";
static bool debug = false;

module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "Name or group label printed when the module loads");

module_param(debug, bool, 0644);
MODULE_PARM_DESC(debug, "Enable extra debug log when true");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Class");
MODULE_DESCRIPTION("Module parameter practice for Chapter 9");
MODULE_VERSION("1.0");

static int __init module_param_init(void)
{
    printk(KERN_INFO "module_param: hello %s\n", name);

    if (debug) {
        printk(KERN_INFO "module_param: debug mode enabled\n");
    }

    return 0;
}

static void __exit module_param_exit(void)
{
    printk(KERN_INFO "module_param: goodbye %s\n", name);
}

module_init(module_param_init);
module_exit(module_param_exit);
