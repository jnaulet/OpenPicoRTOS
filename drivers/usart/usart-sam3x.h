#ifndef USART_SAM3X_H
#define USART_SAM3X_H

#include "usart.h"
#include "clock.h"

struct USART_SAM3X;

struct usart {
    /*@shared@*/ struct USART_SAM3X *base;
    clock_id_t clkid;
};

int usart_sam3x_init(/*@out@*/ struct usart *ctx, struct USART_SAM3X *base, clock_id_t clkid);

/* Runtime calls:
 * usart_setup(struct usart *ctx, struct usart_settings *settings);
 *
 * usart_read();
 * usart_write();
 */

#endif
