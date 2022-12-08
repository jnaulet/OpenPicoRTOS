#ifndef USART_SAM_H
#define USART_SAM_H

#include "usart.h"
#include "clock.h"

struct USART_SAM;

struct usart {
    /*@shared@*/ struct USART_SAM *base;
    clock_id_t clkid;
};

int usart_sam_init(/*@out@*/ struct usart *ctx, struct USART_SAM *base, clock_id_t clkid);

/* Runtime calls:
 * usart_setup(struct usart *ctx, struct usart_settings *settings);
 *
 * usart_read();
 * usart_write();
 */

#endif
