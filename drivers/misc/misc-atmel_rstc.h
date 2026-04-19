#ifndef MISC_ATMEL_RSTC_H
#define MISC_ATMEL_RSTC_H

#include <errno.h>

struct ATMEL_RSTC;

struct atmel_rstc {
    /*@temp@*/ struct ATMEL_RSTC *base;
};

int atmel_rstc_init(/*@out@*/ struct atmel_rstc *ctx, int base);

typedef enum {
    ATMEL_RSTC_RSTTYP_GENERAL_RST   = 0,
    ATMEL_RSTC_RSTTYP_BACKUP_RST    = 1,
    ATMEL_RSTC_RSTTYP_WDT_RST       = 2,
    ATMEL_RSTC_RSTTYP_SOFT_RST      = 3,
    ATMEL_RSTC_RSTTYP_USER_RST      = 4,
    ATMEL_RSTC_RSTTYP_COUNT
} atmel_rstc_rsttyp_t;

int atmel_rstc_reset(struct atmel_rstc *ctx);
int atmel_rstc_get_rsttyp(struct atmel_rstc *ctx, /*@out@*/ atmel_rstc_rsttyp_t *rsttyp);

#endif
