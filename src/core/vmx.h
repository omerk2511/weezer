#include <stddef.h>

static const size_t VMXON_REGION_SIZE = 4096;

int is_vmx_supported(void);
int is_vmx_disabled(void);

void enable_vmx(void);

void* enter_root_mode(void);
void exit_root_mode(void);

void setup_vmx(void* info);
void cleanup_vmx(void* info);

int vmx_setup_succeeded(void);
