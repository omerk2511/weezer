#include <linux/fs.h>
#include <linux/ioctl.h>

typedef struct {

} start_vm_args_t;

typedef struct {

} stop_vm_args_t;

typedef struct {

} list_vms_args_t;

typedef struct {

} query_vm_args_t;

typedef struct {

} bind_vm_args_t;

#define WEEZER_TYPE 'W'

#define WEEZER_START_VM _IOW(WEEZER_TYPE, 0, start_vm_args_t)
#define WEEZER_STOP_VM _IOW(WEEZER_TYPE, 1, stop_vm_args_t)

#define WEEZER_LIST_VMS _IOR(WEEZER_TYPE, 2, list_vms_args_t)
#define WEEZER_QUERY_VM _IOWR(WEEZER_TYPE, 3, query_vm_args_t)

#define WEEZER_BIND_VM _IOW(WEEZER_TYPE, 4, bind_vm_args_t)

long ioctl_start_vm(struct file* file, start_vm_args_t* arg);
long ioctl_stop_vm(struct file* file, stop_vm_args_t* arg);

long ioctl_list_vms(struct file* file, list_vms_args_t* arg);
long ioctl_query_vm(struct file* file, query_vm_args_t* arg);

long ioctl_bind_vm(struct file* file, bind_vm_args_t* arg);
