#ifndef MISC_ATSAMX7X_MATRIX_H
#define MISC_ATSAMX7X_MATRIX_H

#include <errno.h>
#include <stdint.h>
#include <stddef.h>

struct ATSAMX7X_MATRIX;

struct atsamx7x_matrix {
    /*@temp@*/ struct ATSAMX7X_MATRIX *base;
};

int atsamx7x_matrix_init(/*@out@*/ struct atsamx7x_matrix *ctx, int base);

typedef enum {
    ATSAMX7X_MATRIX_ULBT_UNLTD_LENGTH   = 0,
    ATSAMX7X_MATRIX_ULBT_SINGLE_ACCESS  = 1,
    ATSAMX7X_MATRIX_ULBT_4BEAT_BURST    = 2,
    ATSAMX7X_MATRIX_ULBT_8BEAT_BURST    = 3,
    ATSAMX7X_MATRIX_ULBT_16BEAT_BURST   = 4,
    ATSAMX7X_MATRIX_ULBT_32BEAT_BURST   = 5,
    ATSAMX7X_MATRIX_ULBT_64BEAT_BURST   = 6,
    ATSAMX7X_MATRIX_ULBT_128BEAT_BURST  = 7,
    ATSAMX7X_MATRIX_ULBT_COUNT
} atsamx7x_matrix_ulbt_t;

int atsamx7x_matrix_set_mcfg(struct atsamx7x_matrix *ctx, size_t index,
                             atsamx7x_matrix_ulbt_t ulbt);

typedef enum {
    ATSAMX7X_MATRIX_DEFMSTR_NONE    = 0,
    ATSAMX7X_MATRIX_DEFMSTR_LAST    = 1,
    ATSAMX7X_MATRIX_DEFMSTR_FIXED   = 2,
    ATSAMX7X_MATRIX_DEFMSTR_COUNT
} atsamx7x_matrix_defmstr_t;

struct atsamx7x_matrix_scfg {
    size_t fixed_defmstr;
    atsamx7x_matrix_defmstr_t defmstr_type;
    size_t slot_cycle;
};

int atsamx7x_matrix_set_scfg(struct atsamx7x_matrix *ctx, size_t index,
                             const struct atsamx7x_matrix_scfg *settings);

/* TODO: priorities */

int atsamx7x_matrix_set_can0dmaba(struct atsamx7x_matrix *ctx, uint16_t addr_msb);
int atsamx7x_matrix_set_can1dmaba(struct atsamx7x_matrix *ctx, uint16_t addr_msb);

#endif

