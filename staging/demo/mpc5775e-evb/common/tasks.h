#ifndef TASKS_H
#define TASKS_H

#include "mpc5775e-evb.h"

void tick_main(void *priv);
void led0_main(void *priv);
void led1_main(void *priv);
void adc_main(void *priv);
void pwm_main(void *priv);
void spi_main(void *priv);
void can_main(void *priv);

#endif
