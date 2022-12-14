#include "vmx.h"
#include "x86.h"

#include <asm/special_insns.h>
#include <linux/slab.h>
#include <linux/smp.h>

// TODO:: support cpu hotplug?
//        https://docs.kernel.org/core-api/cpu_hotplug.html

static void* g_vmxon_regions[NR_CPUS];

static void insert_vmxon_region(void* vmxon_region) {
    g_vmxon_regions[smp_processor_id()] = vmxon_region;
}

static void remove_vmxon_region(void) {
    int processor_id;

    processor_id = smp_processor_id();
    if (g_vmxon_regions[processor_id]) {
        kfree(g_vmxon_regions[processor_id]);
        g_vmxon_regions[processor_id] = NULL;
    }
}

int is_vmx_supported(void) {
    cpuid_result_t result;
    features_result_t* features;

    result = get_cpuid(GET_FEATURES);
    features = (features_result_t*)&result;

    return features->ecx.vmx;
}

int is_vmx_disabled(void) {
    cpuid_result_t result;
    features_result_t* features;
    feature_control_t feature_control;

    result = get_cpuid(GET_FEATURES);
    features = (features_result_t*)&result;

    feature_control.raw = read_msr(IA32_FEATURE_CONTROL);

    return feature_control.lock &&
           ((!feature_control.enable_vmxon && !features->ecx.smx) ||
            (!feature_control.enable_smx && features->ecx.smx));
}

static void set_vmxe_bit(void) {
    cr4_t cr4;
    cr4.raw = read_cr4();
    cr4.vmxe = 1;
    write_cr4(cr4.raw);
}

static void enable_vmxon(void) {
    feature_control_t feature_control;

    feature_control.raw = read_msr(IA32_FEATURE_CONTROL);
    if (!feature_control.lock) {
        feature_control.lock = 1;
        feature_control.enable_smx = 1;
        feature_control.enable_vmxon = 1;

        write_msr(IA32_FEATURE_CONTROL, feature_control.raw);
    }
}

static void fixed_update_cr0(void) {
    uint64_t cr0, cr0_fixed0, cr0_fixed1;

    cr0_fixed0 = read_msr(IA32_VMX_CR0_FIXED0);
    cr0_fixed1 = read_msr(IA32_VMX_CR0_FIXED1);

    cr0 = read_cr0();
    cr0 |= cr0_fixed0;
    cr0 &= cr0_fixed1;
    write_cr0(cr0);
}

static void fixed_update_cr4(void) {
    uint64_t cr4, cr4_fixed0, cr4_fixed1;

    cr4_fixed0 = read_msr(IA32_VMX_CR4_FIXED0);
    cr4_fixed1 = read_msr(IA32_VMX_CR4_FIXED1);

    cr4 = read_cr4();
    cr4 |= cr4_fixed0;
    cr4 &= cr4_fixed1;
    write_cr4(cr4);
}

void enable_vmx(void) {
    set_vmxe_bit();
    enable_vmxon();

    fixed_update_cr0();
    fixed_update_cr4();
}

static void* setup_vmxon_region(void) {
    void* vmxon_region;
    vmx_basic_t vmx_basic;

    // TODO:: validate that this memory region is not page-able
    vmxon_region = kzalloc(VMXON_REGION_SIZE, GFP_KERNEL);
    if (!vmxon_region) {
        return NULL;
    }

    vmx_basic.raw = read_msr(IA32_VMX_BASIC);
    *(uint32_t*)vmxon_region = vmx_basic.revision_identifier;

    return vmxon_region;
}

void* enter_root_mode(void) {
    int ret;
    void *vmxon_region, *phys_vmxon_region;

    vmxon_region = setup_vmxon_region();
    if (!vmxon_region) {
        return NULL;
    }

    phys_vmxon_region = (void*)virt_to_phys(vmxon_region);

    ret = vmxon(phys_vmxon_region);
    if (ret) {
        kfree(vmxon_region);
        return NULL;
    }

    return vmxon_region;
}

void exit_root_mode(void) {
    vmxoff();
}

void setup_vmx(void* info) {
    void* vmxon_region;

    enable_vmx();
    vmxon_region = enter_root_mode();
    if (vmxon_region) {
        insert_vmxon_region(vmxon_region);
    }
}

void cleanup_vmx(void* info) {
    exit_root_mode();
    remove_vmxon_region();
}

int vmx_setup_succeeded(void) {
    int processor_id;
    for (processor_id = 0; processor_id < num_online_cpus(); processor_id++) {
        if (!g_vmxon_regions[processor_id]) {
            return 0;
        }
    }

    return 1;
}
