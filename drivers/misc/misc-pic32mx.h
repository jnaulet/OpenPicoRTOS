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

#endif
