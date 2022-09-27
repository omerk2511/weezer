#include "vmx.h"

#include <linux/module.h>
#include <linux/init.h>

// module metadata
#define MODULE_NAME "weezer"

MODULE_AUTHOR("Omer Katz");
MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("The core kernel module component of the Weezer Type-2 hypervisor");
MODULE_VERSION("0.1");

static int __init weezer_init(void) {
    printk("%s: module loaded\n", MODULE_NAME);

    if (!is_vmx_supported()) {
        printk("%s: vmx is unsupported, aborting\n", MODULE_NAME);
        return -ENODEV;
    }

    if (is_vmx_disabled()) {
        printk("%s: vmx is disabled, aborting\n", MODULE_NAME);
        return -ENODEV;
    }

    // enter_root_mode();

    // setup_factory_device();

    return 0;
}

static void __exit weezer_exit(void) {
    printk("%s: module unloaded\n", MODULE_NAME);
}

module_init(weezer_init);
module_exit(weezer_exit);