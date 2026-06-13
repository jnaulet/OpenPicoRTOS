#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <errno.h>
#include <stdint.h>
#include <generated/autoconf.h>

#define MPU_SMPU_COUNT        2
#define MPU_SMPU_ERR_COUNT    16
#define MPU_SMPU_REGION_COUNT 16
#define TASK_COUNT (CONFIG_TASK_COUNT + CONFIG_CORE_COUNT)

struct MPU_SMPU {
    volatile uint32_t CES0;
    volatile uint32_t CES1;
    uint32_t RESERVED0[62];
    struct {
        volatile uint32_t ADRn;
        volatile uint32_t ADTLn;
        volatile uint32_t ACDLn;
        uint32_t RESERVED0;
    } ERR[MPU_SMPU_ERR_COUNT];
    uint32_t RESERVED1[128];
    struct {
        volatile uint32_t WRD0;
        volatile uint32_t WRD1;
        volatile uint32_t WRD2; /* format F0 or F1 */
        volatile uint32_t WRD3;
        volatile uint32_t WRD4;
        volatile uint32_t WRD5;
        uint32_t RESERVED0[10];
    } RGD[MPU_SMPU_REGION_COUNT];
};

#define CES0_MERR_M  0xffffu
#define CES0_MERR(x) (((x) & CES0_MERR_M) << 16)
#define CES0_HRL_M   0x7u
#define CES0_HRL(x)  (((x) & CES0_HRL_M) << 1)
#define CES0_GVLD    (1 << 0)

#define CES1_MEOVR_M  0xffffu
#define CES1_MEOVR(x) (((x) & CES1_MEOVR_M) << 16)
#define CES1_NRGD_M   0xfu
#define CES1_NRGD(x)  ((x) & CES1_NRGD_M)

#define ADTLn_PID_M    0xffu
#define ADTLn_PID(x)   (((x) & ADTLn_PID_M) << 24)
#define ADTLn_EATTR_M  0x3u
#define ADTLn_EATTR(x) (((x) & ADTLn_EATTR_M) << 8)
#define ADTLn_ERW      (1 << 7)
#define ADTLn_EMN_M    0x7fu
#define ADTLn_EMN(x)   ((x) & ADTLn_EMN_M)

#define WRD0_SRTADDR_M  0xfffffff0u
#define WRD0_SRTADDR(x) ((x) & WRD0_SRTADDR_M)

#define WRD1_ENDADDR_M  0xfffffff0u
#define WRD1_ENDADDR(x) (((x) & WRD1_ENDADDR_M) | 0xfu)

/* WRD2: all masters apply ACCSET1 except debug (ACCSET3) */
#define WRD2_DEFAULT 0x55555555u
// #define WRD2_DEFAULT 0xffffffffu

#define WRD3_ACCSET1_M  0x3fu
#define WRD3_ACCSET1(x) (((x) & WRD3_ACCSET1_M) << 26)
#define WRD3_ACCSET2_M  0x3fu
#define WRD3_ACCSET2(x) (((x) & WRD3_ACCSET2_M) << 20)
#define WRD3_ACCSET3_M  0x3fu
#define WRD3_ACCSET3(x) (((x) & WRD3_ACCSET3_M) << 13)
#define WRD3_FMT        (1 << 4)
#define WRD3_CI         (1 << 1)

/* ACCSET */
#define SR (1 << 5)
#define SW (1 << 4)
#define SX (1 << 3)
#define UR (1 << 2)
#define UW (1 << 1)
#define UX (1 << 0)

#define WRD4_PID_M       0xffu
#define WRD4_PID(x)      (((x) & WRD4_PID_M) << 24)
#define WRD4_PID_MASK_M  0xffu
#define WRD4_PID_MASK(x) (((x) & WRD4_PID_M) << 24)
#define WRD4_PIDEN       (1 << 0)

#define WRD4_DEFAULT 0

#define WRD5_MID_M  0xffu
#define WRD5_MID(x) (((x) & WRD5_MID_M) << 24)
#define WRD5_LCK_M  0x3u
#define WRD5_LCK(x) (((x) & WRD5_LCK_M) << 2)
#define WRD5_VLD    (1 << 0)

struct smpu_rgd {
    volatile uint32_t WRD0;
    volatile uint32_t WRD1;
    volatile uint32_t WRD3;
    volatile uint32_t WRD5;
};

static struct {
    /* global (kernel) */
    struct smpu_rgd RGD[MPU_SMPU_REGION_COUNT];
    size_t count;
    /* by process */
    struct mpu_pid {
        struct smpu_rgd RGD[MPU_SMPU_REGION_COUNT];
        size_t count;
    } pid[TASK_COUNT];
} mpu[MPU_SMPU_COUNT];

static struct MPU_SMPU *SMPU[MPU_SMPU_COUNT] = {
    (struct MPU_SMPU*)ADDR_SMPU0,   // NOLINT
    (struct MPU_SMPU*)ADDR_SMPU1,   // NOLINT
};

void arch_mpu_init(void)
{
    mpu[0].count = (size_t)MPU_SMPU_REGION_COUNT;
    mpu[1].count = (size_t)MPU_SMPU_REGION_COUNT;

    /*
     * mandatory regions
     */

    /* Put SMPUs area under kernel protection */
    arch_mpu_add_region(PID_KERNEL, (void*)SMPU[0],
                        (size_t)(ADDR_SMPU1 - ADDR_SMPU0) + sizeof(*SMPU[1]),
                        MM_PRW | MM_NON_CACHEABLE);
    /* The first 4mb are "reserved", i suspect this is related to
     * volatile context save/restore */
    /*@-nullpass@*/
    arch_mpu_add_region(PID_KERNEL, (void*)0x0, (size_t)0x400000,
                        MM_PRW | MM_NON_CACHEABLE);
    /*@=nullpass@*/
    /* The last 16kb are "reserved", i suspect this is related to cache operations */
    arch_mpu_add_region(PID_KERNEL, (void*)0xffffc000, (size_t)0x3ff0,
                        MM_PRX | MM_NON_CACHEABLE);
}

static uint32_t WRD3_from_mode(mpu_mode_t mode)
{
    /* defaults: non-executable, privileged read-only, normal cacheable */
    uint32_t WRD3 = (uint32_t)WRD3_ACCSET1(SR) | WRD3_FMT;

    if ((mode & MM_PRIVILEGED) != 0) {
        if ((mode & MM_EXECUTE) != 0) WRD3 |= WRD3_ACCSET1(SX);
        if ((mode & MM_WRITE) != 0) WRD3 |= WRD3_ACCSET1(SW);
    }else{
        WRD3 |= WRD3_ACCSET1(SR | UR);
        if ((mode & MM_EXECUTE) != 0) WRD3 |= WRD3_ACCSET1(SX | UX);
        if ((mode & MM_WRITE) != 0) WRD3 |= WRD3_ACCSET1(SW | UW);
    }

    if ((mode & MM_NON_CACHEABLE) != 0)
        WRD3 |= WRD3_CI;

    return WRD3;
}

/* FIXME */
/*@external@*/ extern void *__ram_start__[];
/*@external@*/ extern void *__ram_end__[];
#define ADDR_IS_IN_RAM(addr)                            \
  ((uintptr_t)(addr) >= (uintptr_t)__ram_start__ &&     \
   ((uintptr_t)(addr) <= (uintptr_t)__ram_end__))

#define MPU_RGD_MATCH(rgd, addr, n, wrd3)       \
  ((uintptr_t)addr >= (rgd)->WRD0 &&            \
   (uintptr_t)addr + n <= (rgd)->WRD1 &&        \
   wrd3 == (rgd)->WRD3)

static int region_already_exists(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
    arch_assert(pid < TASK_COUNT, return -EINVAL);
    arch_assert(pid >= PID_KERNEL, return -EINVAL);
    arch_assert(n > 0, return -EINVAL);

    uint32_t WRD3 = WRD3_from_mode(mode);
    size_t i = (size_t)MPU_SMPU_REGION_COUNT;
    size_t smpu = (size_t)(ADDR_IS_IN_RAM(addr) ? 1 : 0);

    /* try to match global */
    while (i-- != mpu[smpu].count)
        if (MPU_RGD_MATCH(&mpu[smpu].RGD[i], addr, n, WRD3)) return (int)i;
    /* try to match pid */
    for (i = mpu[smpu].count; i-- != 0;)
        if (MPU_RGD_MATCH(&mpu[smpu].pid[pid].RGD[i], addr, n, WRD3)) return (int)i;

    return -ENOENT;
}

void arch_mpu_add_region(int pid, const void *addr, size_t n, mpu_mode_t mode)
{
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid >= PID_KERNEL, return );

    size_t index;
    bool is_kernel = (pid < 0);
    struct smpu_rgd *rgd = NULL;
    /* 0 = Flash + periph, 1 = RAM */
    size_t smpu = (size_t)(ADDR_IS_IN_RAM(addr) ? 1 : 0);

    /* jump out asap */
    if (n == 0 || region_already_exists(pid, addr, n, mode) > -1)
        return;

    if (is_kernel) {
        index = mpu[smpu].count - 1;
        arch_assert(index < (size_t)MPU_SMPU_REGION_COUNT, return );
        /* valid kernel entry */
        rgd = &mpu[smpu].RGD[index];
        mpu[smpu].count = index;
    }else{
        index = mpu[smpu].pid[pid].count;
        arch_assert(index < mpu[smpu].count, return );
        /* valid pid enttry */
        rgd = &mpu[smpu].pid[pid].RGD[index];
        mpu[smpu].pid[pid].count++;
    }

    /* fill-up rgd entry */
    rgd->WRD0 = (uint32_t)WRD0_SRTADDR((uintptr_t)addr);
    rgd->WRD1 = (uint32_t)WRD1_ENDADDR((uintptr_t)addr + n);
    rgd->WRD3 = WRD3_from_mode(mode);
    rgd->WRD5 = (uint32_t)WRD5_VLD;

    /* apply values directly */
    SMPU[smpu]->RGD[index].WRD0 = rgd->WRD0;
    SMPU[smpu]->RGD[index].WRD1 = rgd->WRD1;
    SMPU[smpu]->RGD[index].WRD2 = (uint32_t)WRD2_DEFAULT;
    SMPU[smpu]->RGD[index].WRD3 = rgd->WRD3;
    // SMPU[smpu]->RGD[index].WRD4 = (uint32_t)WRD4_DEFAULT;
    SMPU[smpu]->RGD[index].WRD5 = rgd->WRD5;
}

void arch_mpu_restore_regions(int pid)
{
    arch_assert(pid < TASK_COUNT, return );
    arch_assert(pid > PID_KERNEL, return );

    size_t i, j;

    /* restore PID regions */
    for (i = (size_t)MPU_SMPU_COUNT; i-- != 0;)
        for (j = mpu[i].count; j-- != 0;) {
            const struct smpu_rgd *rgd = &mpu[i].pid[pid].RGD[j];
            if (rgd->WRD5 != 0) {
                SMPU[i]->RGD[j].WRD0 = rgd->WRD0;
                SMPU[i]->RGD[j].WRD1 = rgd->WRD1;
                SMPU[i]->RGD[j].WRD2 = (uint32_t)WRD2_DEFAULT;
                SMPU[i]->RGD[j].WRD3 = rgd->WRD3;
                // SMPU[i]->RGD[j].WRD4 = (uint32_t)WRD4_DEFAULT;
            }
            /* activate (or not) */
            SMPU[i]->RGD[j].WRD5 = rgd->WRD5;
        }
}

void arch_mpu_enable(void)
{
    SMPU[0]->CES0 = (uint32_t)(CES0_MERR(CES0_MERR_M) | CES0_GVLD);
    SMPU[1]->CES0 = (uint32_t)(CES0_MERR(CES0_MERR_M) | CES0_GVLD);
}
