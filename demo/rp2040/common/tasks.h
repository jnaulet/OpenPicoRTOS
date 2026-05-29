#ifndef TASKS_H
#define TASKS_H

void tick_main(void *priv);
void led0_main(void *priv);
void led1_main(void *priv);
void spi_main(void *priv);
void adc_main(void *priv);
void twi_master_main(void *priv);
void twi_slave_main(void *priv);
void pwm_main(void *priv);
void ipwm_main(void *priv);
void wd_main(void *priv);

#endif
