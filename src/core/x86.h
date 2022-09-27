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
} msr_t;

typedef union {
    uint64_t raw;

    struct {
        uint64_t lock : 1;
        uint64_t enable_smx : 1;
        uint64_t enable_vmxon : 1;
    };
} feature_control_t;

uint64_t read_msr(uint32_t msr) {
    uint32_t msr_l, msr_h;
    asm volatile("rdmsr" : "=a"(msr_l), "=d"(msr_h) : "c"(msr));
    return (msr_h << 32) | msr_l;
}

void write_msr(uint32_t msr, uint64_t value) {
    asm volatile("wrmsr" : : "c"(msr), "A"(value));
}
