#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
}

void arch_suspend(void)
{
    /* disable tick */
}

void arch_resume(void)
{
    /* enable tick */
}

picoRTOS_stack_t *arch_prepare_stack(struct picoRTOS_task *task)
{
    picoRTOS_stack_t *sp = task->stack;

    /* hardware stack */
    sp[0] = (picoRTOS_stack_t)0x1;                                  /* call stack size */
    sp[1] = (picoRTOS_stack_t)(unsigned long)task->priv;            /* TOSU */
    sp[2] = (picoRTOS_stack_t)((unsigned long)task->priv >> 8);     /* TOSH */
    sp[3] = (picoRTOS_stack_t)((unsigned long)task->priv >> 16);    /* TOSL */

    /* regs */
    sp[4] = (picoRTOS_stack_t)0x0;      /* PCLATH */
    sp[5] = (picoRTOS_stack_t)0x0;      /* PCLATU */
    sp[6] = (picoRTOS_stack_t)0x0;      /* PRODL */
    sp[7] = (picoRTOS_stack_t)0x0;      /* PRODH */
    sp[8] = (picoRTOS_stack_t)0x0;      /* TBLPTRL */
    sp[9] = (picoRTOS_stack_t)0x0;      /* TBLPTRH */
    sp[10] = (picoRTOS_stack_t)0x0;     /* TBLPTRU */
    sp[11] = (picoRTOS_stack_t)0x0;     /* TABLAT */
    sp[12] = (picoRTOS_stack_t)0x0;     /* FSR0H */
    sp[13] = (picoRTOS_stack_t)0x0;     /* FSR0L */
    sp[14] = (picoRTOS_stack_t)0x0;     /* FSR2H */
    sp[15] = (picoRTOS_stack_t)0x0;     /* FSR2L */
    sp[16] = (picoRTOS_stack_t)0x0;     /* BSR */
    sp[17] = (picoRTOS_stack_t)0xc0;    /* INTCON */
    sp[18] = (picoRTOS_stack_t)0x0;     /* STATUS */
    sp[19] = (picoRTOS_stack_t)0x0;     /* WREG */

    return sp + 20;
}

void arch_idle(void *null)
{
    picoRTOS_assert_fatal(null == NULL);

    for (;;)
        ASM("sleep");
}

/* STATS */

picoRTOS_cycles_t arch_counter(void)
{
    return (picoRTOS_cycles_t)0;
}
