#ifndef MISC_PIC32MX_H
#define MISC_PIC32MX_H

#include <stdint.h>

/* This access model will be used by a lot of drivers */
struct PIC32MX_CLR_SET_INV {
    volatile uint32_t REG;
    volatile uint32_t CLR;
    volatile uint32_t SET;
    volatile uint32_t INV;
};

/* System lock/unlock */
void pic32mx_unlock(void);
void pic32mx_lock(void);

/* MMU */
#define KVA_M              0x1fffffffu
#define KVA_TO_PA(x)       ((uint32_t)(x) & KVA_M)
#define KVA_BASE(x)        ((uint32_t)(x) & ~KVA_M)
#define PA_TO_KVA(x, base) ((x) | (base))

#endif
