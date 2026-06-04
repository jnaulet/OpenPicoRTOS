#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <errno.h>
#include <stdint.h>
#include <generated/autoconf.h>

/* system control space */
#define SCS_BASE    0xe000e000
#define SCS_LEN     0x1000

#define PMSAV6_BASE 0xe000ed90

struct MPU_PMSAV6 {
    volatile uint32_t MPU_TYPE;
    volatile uint32_t MPU_CTRL;
    volatile uint32_t MPU_RNR;
    volatile uint32_t MPU_RBAR;
    volatile uint32_t MPU_RASR;
    volatile uint32_t MPU_RBAR_A1;
    volatile uint32_t MPU_RASR_A1;
    volatile uint32_t MPU_RBAR_A2;
    volatile uint32_t MPU_RASR_A2;
    volatile uint32_t MPU_RBAR_A3;
    volatile uint32_t MPU_RASR_A3;
};

#define MPU_TYPE_IREGION_M 0xffu
#define MPU_TYPE_IREGION_S 16
#define MPU_TYPE_DREGION_M 0xffu
#define MPU_TYPE_DREGION_S 8
#define MPU_TYPE_SEPARATE  (1 << 0)

#define MPU_CTRL_PRIVDEFENA (1 << 2)
#define MPU_CTRL_HFNMIENA   (1 << 1)
#define MPU_CTRL_ENABLE     (1 << 0)

#define MPU_RNR_REGION_M  0xffu
#define MPU_RNR_REGION_S  0
#define MPU_RNR_REGION(x) ((x) & MPU_RNR_REGION_M)

#define MPU_RBAR_ADDR_M    0x7ffffffu
#define MPU_RBAR_ADDR_S    5
#define MPU_RBAR_ADDR(x)   (((x) & MPU_RBAR_ADDR_M) << MPU_RBAR_ADDR_S)
#define MPU_RBAR_VALID     (1 << 4)
#define MPU_RBAR_REGION_M  0xfu
#define MPU_RBAR_REGION_S  0
#define MPU_RBAR_REGION(x) ((x) & MPU_RBAR_REGION_M)

#define MPU_RASR_XN      (1 << 28)
#define MPU_RASR_AP_M    0x7u
#define MPU_RASR_AP(x)   (((x) & MPU_RASR_AP_M) << 24)
#define MPU_RASR_TEX_M   0x7u
#define MPU_RASR_TEX(x)  (((x) & MPU_RASR_TEX_M) << 19)
#define MPU_RASR_S       (1 << 18)
#define MPU_RASR_C       (1 << 17)
#define MPU_RASR_B       (1 << 16)
#define MPU_RASR_SRD_M   0xffu
#define MPU_RASR_SRD(x)  (((x) & MPU_RASR_SRD_M) << 8)
#define MPU_RASR_SIZE_M  0x1fu
#define MPU_RASR_SIZE(x) (((x) & MPU_RASR_SIZE_M) << 1)
#define MPU_RASR_ENABLE  (1 << 0)

#define MPU_PMSAV6_DREGION_COUNT 8
#define TASK_COUNT (CONFIG_TASK_COUNT + CONFIG_CORE_COUNT)

struct mpu_entry {
    volatile uint32_t MPU_RBAR;
    volatile uint32_t MPU_RASR;
    struct {
        uintptr_t addr;
        mpu_mode_t mode;
        size_t n;
    } match;
};

static struct {
    /* global (pre-sheduler) */
    size_t dregion;
    struct mpu_entry MPU[MPU_PMSAV6_DREGION_COUNT];
    size_t count;
    /* by process */
    struct mpu_pid {
        struct mpu_entry MPU[MPU_PMSAV6_DREGION_COUNT];
        size_t count;
    } pid[TASK_COUNT];
} mpu;

static struct MPU_PMSAV6 *MPU = (struct MPU_PMSAV6*)PMSAV6_BASE; // NOLINT

void arch_mpu_init(void)
{
    size_t n;

    mpu.dregion = (size_t)((MPU->MPU_TYPE >> MPU_TYPE_DREGION_S) & MPU_TYPE_DREGION_M);
    arch_assert(mpu.dregion == (size_t)MPU_PMSAV6_DREGION_COUNT, return );

    /* force reset */
    mpu.count = mpu.dregion;
    for (n = 0; n < mpu.count; n++) {
        MPU->MPU_RNR = (uint32_t)MPU_RNR_REGION(n);
        MPU->MPU_RASR = 0;
        /* init processes */
        int m = TASK_COUNT;
        while (m-- != 0) {
            mpu.pid[m].MPU[n].MPU_RBAR = (uint32_t)(MPU_RBAR_VALID | MPU_RBAR_REGION(n));
            mpu.pid[m].MPU[n].MPU_RASR = 0;
        }
    }

    /* add system region to the memory map, prw
     * FIXME: might want to put that somewhere else */
    arch_mpu_add_region(PID_KERNEL, (void*)SCS_BASE, (size_t)SCS_LEN, 0xeu);
}

static uint32_t RASR_from_mode(mpu_mode_t mode)
{
    /* defaults: non-executable, privileged read-only, normal cacheable */
    uint32_t MPU_RASR = (uint32_t)(MPU_RASR_XN | MPU_RASR_AP(5) |
                                   MPU_RASR_TEX(5) | MPU_RASR_B);

    /* anyway */
    if ((mode & MM_EXECUTE) != 0)
        MPU_RASR &= ~MPU_RASR_XN;

    if ((mode & MM_PRIVILEGED) != 0) {
        /* Privileged read-only */
        if ((mode & MM_WRITE) != 0)
            /* Privileged access only */
            MPU_RASR &= ~MPU_RASR_AP(4);
    }else{
        /* Any unprivileged write generates a permission fault */
        MPU_RASR &= ~MPU_RASR_AP(MPU_RASR_AP_M);
        MPU_RASR |= MPU_RASR_AP(2);
        if ((mode & MM_WRITE) != 0)
            /* Full access */
            MPU_RASR |= MPU_RASR_AP(1);
    }

    return MPU_RASR;
}

#define MPU_ENTRY_MATCH(entry, addr, n, mode)                           \
  ((uintptr_t)addr >= (entry)->match.addr &&                            \
   (uintptr_t)addr + n <= (entry)->match.addr + (entry)->match.n &&     \
   (mode) == (entry)->match.mode) /* FIXME, some mode should be compatible */

static int region_already_exists(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
    arch_assert(pid < TASK_COUNT, return -EINVAL);
    arch_assert(pid >= PID_KERNEL, return -EINVAL);
    arch_assert(n > 0, return -EINVAL);

    size_t i = (size_t)MPU_PMSAV6_DREGION_COUNT;

    /* try to match global */
    while (i-- != mpu.count)
        if (MPU_ENTRY_MATCH(&mpu.MPU[i], addr, n, mode)) return (int)i;
    /* try to match pid */
    for (i = mpu.count; i-- != 0;)
        if (MPU_ENTRY_MATCH(&mpu.pid[pid].MPU[i], addr, n, mode)) return (int)i;

    return -ENOENT;
}

static uint32_t RBAR_SIZE_from_addr_n(uintptr_t addr, size_t n,
                                      /*@reldef@*/ uintptr_t *rRBAR)
{
#define MPU_RASR_SIZE_COUNT 31 /* up to 32 in theory */
    arch_assert(n > 0, return 0);

    uint32_t SIZE;
    uintptr_t RBAR = addr;

    /* find closest power of 2 */
    for (SIZE = (uint32_t)7; SIZE < (uint32_t)MPU_RASR_SIZE_COUNT; SIZE++) {
        size_t size = (size_t)(1u << (SIZE + 1));
        /* align addr & fix len */
        RBAR = (uintptr_t)((size_t)RBAR & ~(size - 1));
        size_t n1 = n + (size_t)((uintptr_t)addr - RBAR);
        if (n1 <= size) break;
    }

    /*
     * No fine tuning on armv6-m as we're already short on entries
     */
    *rRBAR = RBAR;
    return SIZE;
}

static uint32_t SRD_from_RBAR_SIZE(uintptr_t addr, size_t n,
                                   uintptr_t RBAR, uint32_t SIZE)
{
    arch_assert(n > 0, return 0);

    /* fine-tune with SRD */
    size_t size = (size_t)(1u << (SIZE + 1));
    size_t sr_size = size / (size_t)8;
    size_t pre = (size_t)(addr - RBAR);
    size_t post = (size_t)((RBAR + size) - (addr + n));
    /* last segment will be cut short if needed */
    uint32_t SRD = ((uint32_t)0xff00u >> (post / sr_size) |
                    (uint32_t)0x00ffu >> (8u - (pre / sr_size)));

    /* for regions < 256 bytes */
    if (size < (size_t)256)
        SRD = 0;

    return SRD;
}

static void addr_n_from_RBAR_SIZE_SRD(uintptr_t RBAR, uint32_t SIZE, uint32_t SRD,
                                      /*@reldef@*/ uintptr_t *raddr,
                                      /*@reldef@*/ uintptr_t *rn)
{
    size_t i;
    size_t n = (size_t)(1u << (SIZE + 1));
    size_t sr_size = n >> 3;

    for (i = (size_t)8; i-- != 0;)
        if ((SRD & (uint32_t)(1u << i)) != 0) n -= sr_size;

    for (i = 0; i < (size_t)8; i++) {
        if ((SRD & (uint32_t)(1u << i)) == 0) break;
        RBAR += sr_size;
    }

    *raddr = RBAR;
    *rn = n;
}

void arch_mpu_add_region(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid >= PID_KERNEL, return );

    /* jump out asap */
    if (n == 0 || region_already_exists(pid, addr, n, mode) > -1)
        return;

    size_t index;
    struct mpu_entry *entry;
    bool is_kernel = (pid < 0);
    uintptr_t start = (uintptr_t)addr;
    uint32_t MPU_RASR = RASR_from_mode(mode);

    if (is_kernel) {
        index = mpu.count - 1;
        arch_assert(index < (size_t)MPU_PMSAV6_DREGION_COUNT, return );
    }else{
        index = mpu.pid[pid].count;
        /*
         * The ARMv6-M has very limited resources and we exhausted them.
         * Show must go on, we sacrifice this tasks' safety net but
         * still preserve the kernel space, at least
         */
        if (index == mpu.count) {
            entry = &mpu.pid[pid].MPU[--index];
            entry->MPU_RBAR = (uint32_t)(MPU_RBAR_REGION(index) | MPU_RBAR_VALID);
            entry->MPU_RASR = (uint32_t)(MPU_RASR | MPU_RASR_SRD(0) |
                                         MPU_RASR_SIZE(MPU_RASR_SIZE_M) |
                                         MPU_RASR_ENABLE);
            /* go back */
            arch_mpu_restore_regions(pid);
            return;
        }
    }

    uintptr_t MPU_RBAR = 0;
    uint32_t SIZE = RBAR_SIZE_from_addr_n(start, n, &MPU_RBAR);
    uint32_t SRD = SRD_from_RBAR_SIZE(start, n, MPU_RBAR, SIZE);

    /* set reg */
    MPU->MPU_RNR = (uint32_t)MPU_RNR_REGION(index);
    MPU->MPU_RBAR = (uint32_t)MPU_RBAR;
    MPU->MPU_RASR = (uint32_t)(MPU_RASR | MPU_RASR_SRD(SRD) |
                               MPU_RASR_SIZE(SIZE) | MPU_RASR_ENABLE);

    /* kernel regions are put last (highest priority) */
    if (is_kernel) entry = &mpu.MPU[--mpu.count];
    else entry = &mpu.pid[pid].MPU[mpu.pid[pid].count++];

    /* fill-up entry */
    entry->MPU_RBAR = MPU->MPU_RBAR | MPU_RBAR_VALID;
    entry->MPU_RASR = MPU->MPU_RASR;
    /* cache values */
    entry->match.mode = mode;
    addr_n_from_RBAR_SIZE_SRD(MPU_RBAR, SIZE, SRD,
                              &entry->match.addr,
                              &entry->match.n);

    /* no checks on armv6-m */
}

void arch_mpu_restore_regions(int pid)
{
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid >= PID_KERNEL, return );

    bool is_kernel = (pid < 0);

    if (is_kernel) {
        /* only used for SMP */
        size_t n = (size_t)MPU_PMSAV6_DREGION_COUNT;
        while (n-- != mpu.count) {
            MPU->MPU_RBAR = mpu.MPU[n].MPU_RBAR;
            MPU->MPU_RASR = mpu.MPU[n].MPU_RASR;
        }
    }else{
        size_t n = mpu.count;
        struct mpu_pid *p = &mpu.pid[pid];

        while (n-- != 0) {
            MPU->MPU_RBAR = p->MPU[n].MPU_RBAR;
            MPU->MPU_RASR = p->MPU[n].MPU_RASR;
        }
    }
}

void arch_mpu_enable(void)
{
    MPU->MPU_CTRL |= MPU_CTRL_ENABLE;
}
