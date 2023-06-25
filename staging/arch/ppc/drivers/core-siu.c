#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stdint.h>

#define SIU_HLT1    ((volatile uint32_t*)(ADDR_SIU + 0x9a4))
#define SIU_RSTVEC1 ((volatile uint32_t*)(ADDR_SIU + 0x9b0))

#define SIU_RSTVEC1_VLE (1 << 0)

/* ASM */
/*@external@*/ extern void arch_core_start(void);

void arch_core_run(picoRTOS_core_t core)
{
    if (!picoRTOS_assert_fatal(core < (picoRTOS_core_t)CONFIG_SMP_CORES))
        return;

    /* start (vle mode) */
    *SIU_RSTVEC1 = (uint32_t)arch_core_start | SIU_RSTVEC1_VLE;
}
