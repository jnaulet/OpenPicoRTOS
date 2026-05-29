#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <errno.h>
#include <stdint.h>
#include <generated/autoconf.h>

/* system control space */
#define SCS_BASE    0xe000e000
#define SCS_LEN     0x1000

#define PMSAV7_BASE 0xe000ed90

struct MPU_PMSAV7 {
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

#define MPU_PMSAV7_DREGION_COUNT 16
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
    struct mpu_entry MPU[MPU_PMSAV7_DREGION_COUNT];
    size_t count;
    /* by process */
    struct mpu_pid {
        struct mpu_entry MPU[MPU_PMSAV7_DREGION_COUNT];
        size_t count;
    } pid[TASK_COUNT];
} mpu;

static struct MPU_PMSAV7 *MPU = (struct MPU_PMSAV7*)PMSAV7_BASE; // NOLINT

void arch_mpu_init(void)
{
    size_t n;

    mpu.dregion = (size_t)((MPU->MPU_TYPE >> MPU_TYPE_DREGION_S) & MPU_TYPE_DREGION_M);
    mpu.count = 0;

    arch_assert(mpu.dregion == (size_t)MPU_PMSAV7_DREGION_COUNT, return );

    /* force reset */
    for (n = 0; n < mpu.dregion; n++) {
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

static int parse_mode(mpu_mode_t mode)
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

    return (int)MPU_RASR;
}

#define MPU_ENTRY_MATCH(entry, addr, n, mode)                           \
  ((uintptr_t)addr >= (entry)->match.addr &&                            \
   (uintptr_t)addr + n <= (entry)->match.addr + n &&                    \
   (mode) == (entry)->match.mode) /* FIXME, some mode should be compatible */

static int region_already_exists(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
    arch_assert(pid < TASK_COUNT, return -EINVAL);
    arch_assert(pid >= PID_KERNEL, return -EINVAL);
    arch_assert(n > 0, return -EINVAL);

    size_t i;

    /* try to match global */
    for (i = 0; i < mpu.count; i++)
        if (MPU_ENTRY_MATCH(&mpu.MPU[i], addr, n, mode)) return (int)i;
    /* try to match pid */
    for (; i < mpu.pid[pid].count; i++)
        if (MPU_ENTRY_MATCH(&mpu.pid[pid].MPU[i], addr, n, mode)) return (int)i;

    return -ENOENT;
}

void arch_mpu_add_region(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
#define MPU_RASR_SIZE_COUNT 31 /* 32 is supported but unrealistic */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
    arch_assert(n > 0, return );
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid >= PID_KERNEL, return );

    /* jump out asap */
    if (region_already_exists(pid, addr, n, mode) > -1)
        return;

    bool is_kernel = (pid < 0);
    uintptr_t MPU_RBAR = (uintptr_t)addr;
    uint32_t MPU_RASR = (uint32_t)parse_mode(mode);

    while (n != 0) {

        size_t size = 0;
        uint32_t SIZE = 0;
        struct mpu_entry *mpu_entry;
        size_t index = mpu.count + (is_kernel ? 0 : mpu.pid[pid].count);

        arch_assert(index < (size_t)MPU_PMSAV7_DREGION_COUNT, return );

        /* find closest power of 2 */
        for (SIZE = (uint32_t)4; SIZE < (uint32_t)MPU_RASR_SIZE_COUNT; SIZE++) {
            size = (size_t)(1u << (SIZE + 1));
            /* align addr & fix len */
            MPU_RBAR = (uintptr_t)((size_t)MPU_RBAR & ~(size - 1));
            size_t n1 = n + (size_t)((uintptr_t)addr - MPU_RBAR);
            if (n1 <= size) /*@innerbreak@*/ break;
        }

        /* fine-tune with SRD */
        size_t sr_size = size >> 3;
        size_t pre = (size_t)((uintptr_t)addr - MPU_RBAR);
        size_t post = (size_t)((MPU_RBAR + (uintptr_t)size) - ((uintptr_t)addr + (uintptr_t)n));
        uint32_t SRD = ((uint32_t)0xff00u >> (post / sr_size) |
                        (uint32_t)0x00ffu >> (8u - (pre / sr_size)));

        /* for regions < 256 bytes */
        if (size < (size_t)256)
            SRD = 0;

        /* set reg */
        MPU->MPU_RNR = (uint32_t)MPU_RNR_REGION(index);
        MPU->MPU_RBAR = (uint32_t)MPU_RBAR;
        MPU->MPU_RASR = (uint32_t)(MPU_RASR | MPU_RASR_SRD(SRD) |
                                   MPU_RASR_SIZE(SIZE) | MPU_RASR_ENABLE);

        if (is_kernel) {
            mpu_entry = &mpu.MPU[index];
            mpu.count++;
        }else{
            mpu_entry = &mpu.pid[pid].MPU[index];
            mpu.pid[pid].count++;
        }

        /* fill-up entry */
        mpu_entry->MPU_RBAR = MPU->MPU_RBAR | MPU_RBAR_VALID;
        mpu_entry->MPU_RASR = MPU->MPU_RASR;
        /* cache values */
        mpu_entry->match.addr = MPU_RBAR;
        mpu_entry->match.n = size;
        mpu_entry->match.mode = mode;

        /* prepare next round */
        n -= MIN(n, size);
        MPU_RBAR += (uintptr_t)size;
    }
}

void arch_mpu_restore_regions(int pid)
{
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid > PID_KERNEL, return );

    size_t n = mpu.count;
    struct mpu_pid *p = &mpu.pid[pid];

    for (; n < (size_t)MPU_PMSAV7_DREGION_COUNT; n++) {
        MPU->MPU_RBAR = p->MPU[n].MPU_RBAR;
        MPU->MPU_RASR = p->MPU[n].MPU_RASR;
    }
}

void arch_mpu_enable(void)
{
    MPU->MPU_CTRL |= MPU_CTRL_ENABLE;
}
