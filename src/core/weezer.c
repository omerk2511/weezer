#include <linux/module.h>
#include <linux/init.h>

// module metadata
#define MODULE_NAME "weezer"

MODULE_AUTHOR("Omer Katz");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("The core kernel module component of the Weezer Type-2 hypervisor");
MODULE_VERSION("0.1");

static int __init weezer_init(void)
{
    pr_info("%s: module loaded at 0x%p\n", MODULE_NAME, weezer_init);
    return 0;
}

static void __exit weezer_exit(void)
{
    pr_info("%s: module unloaded from 0x%p\n", MODULE_NAME, weezer_exit);
}

module_init(weezer_init);
module_exit(weezer_exit);