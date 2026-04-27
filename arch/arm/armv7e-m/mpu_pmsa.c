#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <stdint.h>
#include <generated/autoconf.h>

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

static struct {
    /* global (pre-sheduler) */
    size_t count;
    size_t dregion;
    struct {
        volatile uint32_t MPU_RBAR;
        volatile uint32_t MPU_RASR;
    } MPU[MPU_PMSAV7_DREGION_COUNT];
    /* by process */
    struct mpu_pid {
        struct {
            volatile uint32_t MPU_RBAR;
            volatile uint32_t MPU_RASR;
        } MPU[MPU_PMSAV7_DREGION_COUNT];
        size_t count;
    } pid[CONFIG_TASK_COUNT];
} ctx;

static struct MPU_PMSAV7 *MPU = (struct MPU_PMSAV7*)PMSAV7_BASE; // NOLINT

void arch_mpu_init_opt(void)
{
    size_t n;

    ctx.dregion = (size_t)((MPU->MPU_TYPE >> MPU_TYPE_DREGION_S) & MPU_TYPE_DREGION_M);
    ctx.count = 0;

    arch_assert(ctx.dregion == (size_t)MPU_PMSAV7_DREGION_COUNT, return );

    /* force reset */
    for (n = 0; n < ctx.dregion; n++) {
        MPU->MPU_RNR = (uint32_t)MPU_RNR_REGION(n);
        MPU->MPU_RASR = 0;
        /* init processes */
        int m = CONFIG_TASK_COUNT;
        while (m-- != 0) {
            ctx.pid[m].MPU[n].MPU_RBAR = (uint32_t)(MPU_RBAR_VALID | MPU_RBAR_REGION(n));
            ctx.pid[m].MPU[n].MPU_RASR = 0;
        }
    }
}

static int parse_mode(/*@observer@*/ const char *mode)
{
    char c;
    /* non-executable, read-only, normal cacheable */
    uint32_t MPU_RASR = (uint32_t)(MPU_RASR_XN | MPU_RASR_AP(7) | MPU_RASR_TEX(5) | MPU_RASR_B);

    while ((c = *mode++) != '\0') {
        switch (c) {
        case 'r': break;
        case 'w': MPU_RASR &= ~MPU_RASR_AP(4); break;
        case 'x': MPU_RASR &= ~MPU_RASR_XN; break;
        case 'o': MPU_RASR &= ~(MPU_RASR_TEX(MPU_RASR_TEX_M) | MPU_RASR_C | MPU_RASR_B);  break;
        case 'd':
            if (*mode != '-') {
                /* shared device */
                MPU_RASR &= ~(MPU_RASR_TEX(MPU_RASR_TEX_M) | MPU_RASR_C);
                MPU_RASR |= MPU_RASR_B;
            }else{
                MPU_RASR &= ~(MPU_RASR_TEX(MPU_RASR_TEX_M) | MPU_RASR_C | MPU_RASR_B);
                MPU_RASR |= MPU_RASR_TEX(2);
                mode++; /* consume '-' */
            }
            break;

        case '-':
            /* non-cacheable */
            MPU_RASR &= ~(MPU_RASR_TEX(3) | MPU_RASR_B | MPU_RASR_C);
            break;

        default:
            break; /* ignore */
        }
    }

    return (int)MPU_RASR;
}

void arch_mpu_add_region_opt(int pid, const void *addr, size_t n, const char *mode)
{
#define MPU_RASR_SIZE_COUNT 16
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
    arch_assert(n > 0, return );
    arch_assert(pid < CONFIG_TASK_COUNT, return );

    bool is_general = (pid < 0);
    uint32_t MPU_RASR = (uint32_t)parse_mode(mode);
    /* align & fix len */
    uintptr_t ptr = (uintptr_t)((int)addr & ~(MPU_RBAR_VALID | MPU_RBAR_REGION(MPU_RBAR_REGION_M)));
    n += (size_t)((uintptr_t)addr - ptr);

    while (n != 0) {

        size_t size = 0;
        uint32_t SIZE = 0;
        size_t index = ctx.count + (is_general ? 0 : ctx.pid[pid].count);

        arch_assert(index < (size_t)MPU_PMSAV7_DREGION_COUNT, return );

        /* find closest power of 2 */
        for (SIZE = (uint32_t)4; SIZE < (uint32_t)MPU_RASR_SIZE_COUNT; SIZE++) {
            size = (size_t)(1u << (SIZE + 1));
            if (n <= size) /*@innerbreak@*/ break;
        }

        /* fine-tune with SRD */
        size_t sr_size = size >> 3;
        uint32_t SRD = (uint32_t)0xff00u >> ((MAX(size, n) - n) / sr_size);

        /* set reg */
        MPU->MPU_RNR = (uint32_t)MPU_RNR_REGION(index);
        MPU->MPU_RBAR = (uint32_t)ptr;
        MPU->MPU_RASR = (uint32_t)(MPU_RASR | MPU_RASR_SRD(SRD) |
                                   MPU_RASR_SIZE(SIZE) | MPU_RASR_ENABLE);

        /* prepare next round */
        n -= MIN(n, size);
        ptr += (uintptr_t)size;

        if (is_general) {
            ctx.MPU[index].MPU_RBAR = MPU->MPU_RBAR | MPU_RBAR_VALID;
            ctx.MPU[index].MPU_RASR = MPU->MPU_RASR;
            ctx.count++;
        }else{
            ctx.pid[pid].MPU[index].MPU_RBAR = MPU->MPU_RBAR | MPU_RBAR_VALID;
            ctx.pid[pid].MPU[index].MPU_RASR = MPU->MPU_RASR;
            ctx.pid[pid].count++;
        }
    }
}

void arch_mpu_restore_regions(int pid)
{
    /* ignore idle */
    if (pid >= CONFIG_TASK_COUNT) return;

    if (pid < 0) {
        size_t n = 0;

        for (; n < ctx.count; n++) {
            MPU->MPU_RBAR = ctx.MPU[n].MPU_RBAR;
            MPU->MPU_RASR = ctx.MPU[n].MPU_RASR;
        }
    } else {
        size_t n = ctx.count;
        struct mpu_pid *p = &ctx.pid[pid];

        for (; n < (size_t)MPU_PMSAV7_DREGION_COUNT; n++) {
            MPU->MPU_RBAR = p->MPU[n].MPU_RBAR;
            MPU->MPU_RASR = p->MPU[n].MPU_RASR;
        }
    }
}

void arch_mpu_enable(void)
{
    MPU->MPU_CTRL |= MPU_CTRL_ENABLE;
}
