#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/* CACHES */
#define DCIMVAC ((volatile unsigned long*)0xe000ef5c)
#define DCCMVAC ((volatile unsigned long*)0xe000ef68)

/* CACHE OPS */

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_invalidate_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    unsigned long base = (unsigned long)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        *DCIMVAC = (unsigned long)(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("dsb");
}

/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_flush_dcache(void *addr, size_t n)
{
    arch_assert_void(n > 0);

    size_t npages = (n + (ARCH_L1_DCACHE_LINESIZE - 1)) / ARCH_L1_DCACHE_LINESIZE;
    unsigned long base = (unsigned long)addr & ~(ARCH_L1_DCACHE_LINESIZE - 1);

    while (npages-- != 0)
        *DCCMVAC = (unsigned long)(base + (npages * ARCH_L1_DCACHE_LINESIZE));

    ASM("dsb");
}
