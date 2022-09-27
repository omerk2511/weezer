#include "vmx.h"
#include "x86.h"

int is_vmx_supported(void) {
    cpuid_result_t result = get_cpuid(GET_FEATURES);
    features_result_t* features = (features_result_t*)&result;

    return features->ecx.vmx;
}

int is_vmx_disabled(void) {
    cpuid_result_t result = get_cpuid(GET_FEATURES);
    features_result_t* features = (features_result_t*)&result;

    feature_control_t feature_control = { read_msr(IA32_FEATURE_CONTROL) };

    return feature_control.lock &&
           ((!feature_control.enable_vmxon && !features->ecx.smx) ||
            (!feature_control.enable_smx && features->ecx.smx));
}
