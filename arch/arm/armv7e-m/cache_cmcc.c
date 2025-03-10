#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

#define CMCC_MAINT1 ((unsigned long*)ADDR_CMCC)

#define MAINT1_WAY_M    0xfu
#define MAINT1_WAY(x)   (((x) & MAINT1_WAY_M) << 28)
#define MAINT1_INDEX_M  0xffu
#define MAINT1_INDEX(x) (((x) & MAINT1_INDEX_M) << 4)

#define CMCC_WAY_M       0x3u
#define CMCC_WAY(addr)   (CMCC_WAY_M & ((addr) >> 10))
#define CMCC_INDEX_M     0x3fu
#define CMCC_INDEX(addr) (CMCC_INDEX_M & ((addr) >> (ARCH_L1_CACHE_LINESIZE / 4)))

void arch_invalidate_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t nlines = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    unsigned long base = (unsigned long)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (nlines-- != 0) {
        unsigned long x = (unsigned long)(base + (nlines * ARCH_L1_DCACHE_LINESIZE));
        *CMCC_MAINT1 = MAINT1_WAY(CMCC_WAY(x)) | MAINT1_INDEX(CMCC_INDEX(x));
    }

    ASM("dsb");
}
