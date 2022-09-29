#include "config.h"
#include "fops.h"
#include "vmx.h"

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

// module metadata
MODULE_AUTHOR("Omer Katz");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("The core kernel module component of the Weezer Type-2 hypervisor");
MODULE_VERSION("0.1");

static const struct file_operations weezer_fops = {
    .owner = THIS_MODULE,
    .open = weezer_open,
    .release = weezer_release,
    .unlocked_ioctl = weezer_ioctl,
};

static struct miscdevice weezer_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "weezer",
    .nodename = "weezer",
    .fops = &weezer_fops,
};

static int __init weezer_init(void) {
    int ret;

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

    on_each_cpu(setup_vmx, NULL, true);
    if (!vmx_setup_succeeded()) {
        printk("%s: vmx setup failed\n", MODULE_NAME);

        misc_deregister(&weezer_miscdev);
        on_each_cpu(cleanup_vmx, NULL, true);

        return -ENODEV;
    }

    printk("%s: module successfully initialized\n", MODULE_NAME);
    return 0;
}

static void __exit weezer_exit(void) {
    misc_deregister(&weezer_miscdev);
    on_each_cpu(cleanup_vmx, NULL, true);

    printk("%s: module successfully unloaded\n", MODULE_NAME);
}

module_init(weezer_init);
module_exit(weezer_exit);