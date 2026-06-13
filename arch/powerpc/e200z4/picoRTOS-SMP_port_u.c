#include "picoRTOS_device.h"
#include "picoRTOS-SMP_port.h"

/* SW LOCK */
static bool UNPRIVILEGED_DATA aux_core_is_idling
__attribute__((aligned(ARCH_L1_DCACHE_LINESIZE)));

static void aux_core_set_idling(bool status)
{
    aux_core_is_idling = status;
    arch_flush_dcache(&aux_core_is_idling, sizeof(aux_core_is_idling));
}

/* This is a privileged interface for picoRTOS-SMP_port.c */
void PRIVILEGED_TEXT arch_aux_core_reset_status(void)
{
    aux_core_set_idling(false);
}

int PRIVILEGED_TEXT arch_aux_core_is_idling(void)
{
    arch_invalidate_dcache(&aux_core_is_idling, sizeof(aux_core_is_idling));
    if (aux_core_is_idling) return 0;
    return -1;
}

/*
 * idle with extra steps
 */
void arch_idle(void)
{
    /* signal we're idling */
    aux_core_set_idling(true);

    for (;;)
        ASM("wait");
}
