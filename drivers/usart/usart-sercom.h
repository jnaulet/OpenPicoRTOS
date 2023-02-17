#ifndef USART_SERCOM_H
#define USART_SERCOM_H

#include "usart.h"
#include "clock.h"

#define USART_SERCOM_FRAME_SIZE_MIN 8
#define USART_SERCOM_FRAME_SIZE_MAX 32

struct USART_SERCOM;

struct usart {
    /*@temp@*/ struct USART_SERCOM *base;
    clock_id_t clkid;
    int balance;
    size_t frame_size;
};

/* FIXME: quick and dirty */
struct usart_sercom_settings {
    unsigned long bitrate;
    bool cpol;
    unsigned long txpo;
    size_t frame_size;
};

int usart_sercom_init(/*@out@*/ struct usart *ctx, struct USART_SERCOM *base, clock_id_t clkid);
int usart_sercom_setup(struct usart *ctx, const struct usart_sercom_settings *settings);

/* Runtime calls:
 * usart_read(struct usart *ctx, void *buf, size_t n);
 * usart_write(struct usart *ctx, const void *buf, size_t n);
 */

#endif
