#include <asm/cpufeature.h>

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_result_t;

typedef struct {
    union {
        uint32_t raw;
    } eax;

    union {
        uint32_t raw;
    } ebx;

    union {
        struct {
            uint32_t sse3 : 1;
            uint32_t pclmul : 1;
            uint32_t dtes64 : 1;
            uint32_t monitor : 1;
            uint32_t ds_cpl : 1;
            uint32_t vmx : 1;
            uint32_t smx : 1;
            uint32_t est : 1;
            uint32_t tm2 : 1;
            uint32_t ssse3 : 1;
            uint32_t cid : 1;
            uint32_t fma : 1;
            uint32_t cx16 : 1;
            uint32_t etprd : 1;
            uint32_t pdcm : 1;
            uint32_t : 1;
            uint32_t pcid : 1;
            uint32_t dca : 1;
            uint32_t sse4_1 : 1;
            uint32_t sse4_2 : 1;
            uint32_t x2apic : 1;
            uint32_t movbe : 1;
            uint32_t popcnt : 1;
            uint32_t aes : 1;
            uint32_t xsave : 1;
            uint32_t osxsave : 1;
            uint32_t avx : 1;
            uint32_t f16c : 1;
            uint32_t rdrnd : 1;
            uint32_t hypervisor : 1;
        };

        uint32_t raw;
    } ecx;

    union {
        struct {
            uint32_t fpu : 1;
            uint32_t vme : 1;
            uint32_t de : 1;
            uint32_t pse : 1;
            uint32_t tsc : 1;
            uint32_t msr : 1;
            uint32_t pae : 1;
            uint32_t mce : 1;
            uint32_t cx8 : 1;
            uint32_t apic : 1;
            uint32_t : 1;
            uint32_t sep : 1;
            uint32_t mtrr : 1;
            uint32_t pge : 1;
            uint32_t mca : 1;
            uint32_t cmov : 1;
            uint32_t pat : 1;
            uint32_t pse36 : 1;
            uint32_t psn : 1;
            uint32_t clfsh : 1;
            uint32_t : 1;
            uint32_t ds : 1;
            uint32_t acpi : 1;
            uint32_t mmx : 1;
            uint32_t fxsr : 1;
            uint32_t sse : 1;
            uint32_t sse2 : 1;
            uint32_t ss : 1;
            uint32_t htt : 1;
            uint32_t tm1 : 1;
            uint32_t ia64 : 1;
            uint32_t pbe : 1;
        };

        uint32_t raw;
    } edx;
} features_result_t;

typedef enum {
    GET_VENDOR,
    GET_FEATURES,
} cpuid_function_t;

cpuid_result_t get_cpuid(uint32_t function) {
    cpuid_result_t result = {};
    cpuid(function, &result.eax, &result.ebx, &result.ecx, &result.edx);
    return result;
}

typedef enum {
    IA32_FEATURE_CONTROL = 0x3a,
    IA32_VMX_BASIC = 0x480,
    IA32_VMX_CR0_FIXED0 = 0x486,
    IA32_VMX_CR0_FIXED1 = 0x487,
    IA32_VMX_CR4_FIXED0 = 0x488,
    IA32_VMX_CR4_FIXED1 = 0x489,
} msr_t;

typedef union {
    uint64_t raw;

    struct {
        uint64_t lock : 1;
        uint64_t enable_smx : 1;
        uint64_t enable_vmxon : 1;
    };
} feature_control_t;

typedef union {
    uint64_t raw;

    struct {
        uint64_t revision_identifier : 31;
        uint64_t : 1;
        uint64_t region_size : 12;
        uint64_t : 3;
        uint64_t supported_ia64 : 1;
        uint64_t supported_dual_monitor : 1;
        uint64_t memory_type : 4;
        uint64_t vm_exit_report : 1;
        uint64_t vmx_capability_hint : 1;
        uint64_t : 8;
    };
} vmx_basic_t;

uint64_t read_msr(uint32_t msr) {
    uint32_t msr_l, msr_h;
    asm volatile("rdmsr" : "=a"(msr_l), "=d"(msr_h) : "c"(msr));
    return ((uint64_t)msr_h << 32) | msr_l;
}

void write_msr(uint32_t msr, uint64_t value) {
    asm volatile("wrmsr" : : "c"(msr), "A"(value));
}

typedef union {
    uint64_t raw;

    struct {
        uint64_t vme : 1;
        uint64_t pvi : 1;
        uint64_t tsd : 1;
        uint64_t de : 1;
        uint64_t pse : 1;
        uint64_t pae : 1;
        uint64_t mce : 1;
        uint64_t pge : 1;
        uint64_t pce : 1;
        uint64_t osfxsr : 1;
        uint64_t osxmmexcpt : 1;
        uint64_t umip : 1;
        uint64_t la57 : 1;
        uint64_t vmxe : 1;
        uint64_t smxe : 1;
        uint64_t fsgsbase : 1;
        uint64_t pcide : 1;
        uint64_t osxsave : 1;
        uint64_t smep : 1;
        uint64_t smap : 1;
        uint64_t pke : 1;
        uint64_t cet : 1;
        uint64_t pks : 1;
    };
} cr4_t;

int vmxon(void* phys_vmxon_region) {
    int error;
	asm volatile("vmxon %1"
			     : "=r"(error)
			     : "m"(phys_vmxon_region)
			     : "memory");

    return error;
}

void vmxoff(void) {
    asm volatile("vmxoff");
}
