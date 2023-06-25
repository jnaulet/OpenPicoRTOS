#include "picoRTOS.h"
#include "picoRTOS_device.h"

#include <stddef.h>
#include <stdint.h>

#define INTC_INTCON  ((volatile uint32_t*)(ADDR_INTC + 0x0))
#define INTC_INTSTAT ((volatile uint32_t*)(ADDR_INTC + 0x20))
#define INTC_IFSn    ((volatile uint32_t*)(ADDR_INTC + 0x40))
#define INTC_IECn    ((volatile uint32_t*)(ADDR_INTC + 0xc0))
#define INTC_IPCn    ((volatile uint32_t*)(ADDR_INTC + 0x140))

void arch_init_intc(void)
{
    /* set multi-vector mode */
    *INTC_INTCON |= (1 << 12);

    /* enable interrupts */
    INTC_IFSn[0] &= ~0x1;
    INTC_IECn[0] |= 0x1;
    INTC_IPCn[0] |= 0x5;
}

/*@external@*/
extern struct {
    picoRTOS_isr_fn fn;
    /*@temp@*/ /*@null@*/ void *priv;
} ISR_TABLE[DEVICE_INTERRUPT_VECTOR_COUNT];

void arch_register_interrupt(picoRTOS_irq_t irq, picoRTOS_isr_fn fn, void *priv)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    ISR_TABLE[irq].fn = fn;
    ISR_TABLE[irq].priv = priv;
}

void arch_enable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    /* find the correct IEC offset */
    size_t IEC_index = (size_t)(irq >> 5);
    size_t IEC_bit = (size_t)(0x1fu & irq);

    /* find the correct IPC offset */
    size_t IPC_index = (size_t)(irq >> 6);
    size_t IPC_shift = (size_t)((0x1u & irq) << 4);

    /* Beware: indexes have to * 4 cause of microchip clear/set/invert structure
     * TODO: find a way to make this more generic and portable */
    INTC_IFSn[IEC_index << 2] &= ~(1 << IEC_bit);
    INTC_IPCn[IPC_index << 2] |= (0x5 << IPC_shift);
    INTC_IECn[IEC_index << 2] |= (1 << IEC_bit);
}

void arch_disable_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    /* find the correct IEC offset */
    size_t IEC_index = (size_t)(irq >> 5);
    size_t IEC_bit = (size_t)(0x1f & irq);

    /* Beware: indexes have to * 4 cause of microchip clear/set/invert structure
     * TODO: find a way to make this more generic and portable */
    INTC_IECn[IEC_index << 2] &= ~(1 << IEC_bit);
}

/* Beware:
 * These functions are called from the assembly only,
 * we need to setup the static analysis accordingly
 */

/*@external@*/
/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
void arch_ack_interrupt(picoRTOS_irq_t irq)
{
    if (!picoRTOS_assert_fatal(irq < (picoRTOS_irq_t)DEVICE_INTERRUPT_VECTOR_COUNT)) return;

    /* find the correct IEC offset */
    size_t IFS_index = (size_t)(irq >> 5);
    size_t IFS_bit = (size_t)(0x1f & irq);

    INTC_IFSn[IFS_index << 2] &= ~(1 << IFS_bit);
}

/*@external@*/
/* cppcheck-suppress [unusedFunction,unmatchedSuppression] */
picoRTOS_irq_t arch_get_interrupt(void)
{
    return (picoRTOS_irq_t)(0xffu & *INTC_INTSTAT);
}
