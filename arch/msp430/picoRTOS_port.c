#include "picoRTOS.h"
#include "picoRTOS_port.h"
#include "picoRTOS_device.h"

#include <generated/autoconf.h>

/* ASM */
/*@external@*/ extern /*@temp@*/
picoRTOS_stack_t *arch_save_first_context(picoRTOS_stack_t *sp,
                                          arch_entry_point_fn fn,
                                          /*@null@*/ void *priv);

/*@external@*/ extern void arch_start_first_task(picoRTOS_stack_t *sp);
/*@external@*/ extern void arch_syscall(syscall_t syscall, void *priv);

/*@external@*/ extern void SysTick_Handler(void *priv);

/* WDTPLUS */
struct WDTPLUS {
    volatile uint16_t WDTCTL;
};

#define WDTCTL_WDTPW_M   0xffu
#define WDTCTL_WDTPW(x)  (((x) & WDTCTL_WDTPW_M) << 8)
#define WDTCTL_WDTHOLD   (1u << 7)
#define WDTCTL_WDTNMIES  (1u << 6)
#define WDTCTL_WDTNMI    (1u << 5)
#define WDTCTL_WDTTMSEL  (1u << 4)
#define WDTCTL_WDTCNTCL  (1u << 3)
#define WDTCTL_WDTSSEL   (1u << 2)
#define WDTCTL_WDTISx_M  0x3u
#define WDTCTL_WDTISx(x) ((x) & WDTCTL_WDTISx_M)

/* TIMER */
static struct WDTPLUS *WDT = (struct WDTPLUS*)ADDR_WDTPLUS;

/* FUNCTIONS TO IMPLEMENT */

void arch_init(void)
{
    /* disable interrupts */
    ASM("dint");
    ASM("nop");

    /* TIMER */
    picoRTOS_register_interrupt((picoRTOS_irq_t)IRQ_WDT,
                                SysTick_Handler, NULL);

    WDT->WDTCTL = (uint16_t)(WDTCTL_WDTPW(0x5a) |
                             WDTCTL_WDTTMSEL |
                             WDTCTL_WDTCNTCL |
                             WDTCTL_WDTSSEL |
                             WDTCTL_WDTISx(0x3));
    /* enable */
    *(uint8_t*)ADDR_IE1 |= (uint8_t)0x1;
}

void arch_suspend(void)
{
    /* disable tick */
    ASM("dint");
    ASM("nop");
}

void arch_resume(void)
{
    /* enable tick */
    ASM("nop");
    ASM("eint");
}

picoRTOS_stack_t *arch_prepare_stack(picoRTOS_stack_t *stack,
                                     size_t stack_count,
                                     arch_entry_point_fn fn,
                                     void *priv)
{
    arch_assert_void(stack_count >= (size_t)ARCH_MIN_STACK_COUNT);
    /* MSP430 have a pre-decrementing stack */
    return arch_save_first_context(stack + stack_count, fn, priv);
}

void __attribute__((weak)) arch_idle(void)
{
    for (;;)
        ASM("bis #0x90, R0"); /* LPM3 */
}

/* ATOMIC OPS EMULATION */

picoRTOS_atomic_t arch_compare_and_swap(picoRTOS_atomic_t *var,
                                        picoRTOS_atomic_t old,
                                        picoRTOS_atomic_t val)
{
    ASM("dint");
    ASM("nop");

    if (*var == old) {
        *var = val;
        val = old;
    }

    ASM("nop");
    ASM("eint");
    ASM("nop");

    return val;
}

picoRTOS_atomic_t arch_test_and_set(picoRTOS_atomic_t *ptr)
{
    return arch_compare_and_swap(ptr, (picoRTOS_atomic_t)0,
                                 (picoRTOS_atomic_t)1);
}

/* INTERRUPTS MANAGEMENT */

void arch_enable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert(irq > (picoRTOS_irq_t)0, return );
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    /* no effect */
}

void arch_disable_interrupt_ext(picoRTOS_irq_t irq, picoRTOS_mask_t core_mask)
{
    /*@i@*/ (void)core_mask;
    arch_assert(irq > (picoRTOS_irq_t)0, return );
    arch_assert(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT, return );
    /* no effect */
}
