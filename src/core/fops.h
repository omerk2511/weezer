#include <linux/fs.h>

int weezer_open(struct inode* inode, struct file* file);
int weezer_release(struct inode* inode, struct file* file);

long weezer_ioctl(struct file* file, unsigned int cmd, unsigned long arg);