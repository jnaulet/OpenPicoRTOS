#ifndef CH552_H
#define CH552_H

#include "gpio-8051.h"

struct ch552 {
    struct gpio D1;
};

int ch552_init(/*@out@*/ struct ch552 *ctx);

#endif
