#include "config.h"
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
    return 0;
}