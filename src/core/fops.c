#include "config.h"
#include "ioctl.h"
#include "fops.h"

int weezer_open(struct inode* inode, struct file* file) {
    printk("%s: /dev/weezer was opened\n", MODULE_NAME);
    return 0;
}

int weezer_release(struct inode* inode, struct file* file) {
    printk("%s: /dev/weezer was released\n", MODULE_NAME);
    return 0;
}

long weezer_ioctl(struct file* file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case WEEZER_START_VM: {
            return ioctl_start_vm(file, (start_vm_args_t*)arg);
        }

        case WEEZER_STOP_VM: {
            return ioctl_stop_vm(file, (stop_vm_args_t*)arg);
        }

        case WEEZER_LIST_VMS: {
            return ioctl_list_vms(file, (list_vms_args_t*)arg);
        }

        case WEEZER_QUERY_VM: {
            return ioctl_query_vm(file, (query_vm_args_t*)arg);
        }

        case WEEZER_BIND_VM: {
            return ioctl_bind_vm(file, (bind_vm_args_t*)arg);
        }

        default: {
            return -EINVAL;
        }
    }

    return 0;
}