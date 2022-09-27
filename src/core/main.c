#include "vmx.h"

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

// module metadata
#define MODULE_NAME "weezer"

MODULE_AUTHOR("Omer Katz");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("The core kernel module component of the Weezer Type-2 hypervisor");
MODULE_VERSION("0.1");

static const struct file_operations weezer_fops = {
    .owner = THIS_MODULE,
};

static struct miscdevice weezer_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "weezer",
    .nodename = "weezer",
    .fops = &weezer_fops,
};

static int __init weezer_init(void) {
    int ret;

    printk("%s: module loaded\n", MODULE_NAME);

    if (!is_vmx_supported()) {
        printk("%s: vmx is unsupported, aborting\n", MODULE_NAME);
        return -ENODEV;
    }

    if (is_vmx_disabled()) {
        printk("%s: vmx is disabled, aborting\n", MODULE_NAME);
        return -ENODEV;
    }

    ret = misc_register(&weezer_miscdev);
    if (ret) {
        printk("%s: can't register device\n", MODULE_NAME);
        return ret;
    }

    // enter_root_mode();

    return 0;
}

static void __exit weezer_exit(void) {
    misc_deregister(&weezer_miscdev);
    // exit_root_mode();

    printk("%s: module unloaded\n", MODULE_NAME);
}

module_init(weezer_init);
module_exit(weezer_exit);