#ifndef MISC_NXP_IGF_H
#define MISC_NXP_IGF_H

#include <errno.h>
#include <stddef.h>
#include <stdbool.h>

#define NXP_IGF_CHANNEL_COUNT 46

struct NXP_IGF;

struct nxp_igf {
    /*@temp@*/ struct NXP_IGF *base;
    size_t channel;
};

struct nxp_igf_settings {
    bool invert;
};

int nxp_igf_init(struct nxp_igf *ctx, int base, size_t channel);
int nxp_igf_setup(struct nxp_igf *ctx, struct nxp_igf_settings *settings);

#endif
