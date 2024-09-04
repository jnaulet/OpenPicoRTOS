#ifndef STM8S003F3P3_H
#define STM8S003F3P3_H

#include "gpio-stm8.h"

struct stm8s003f3p3 {
  struct gpio TICK;
  struct gpio TEST;
};

int stm8s003f3p3_init(/*@out@*/ struct stm8s003f3p3 *ctx);

#endif
