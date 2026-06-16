# picoRTOS HAL API

## Description

 - [ADC](#adc)
 - [CAN](#can)
 - [Clocks](#clocks)
 - [DMA](#dma-simple)
 - [DMA Scatter-Gather](#dma-scatter-gather)
 - [EEPROM](#eeprom)
 - [Flash (External)](#flash-external)
 - [Flash (Internal)](#flash-internal)
 - [GPIO](#gpio)
 - [IPWM](#ipwm)
 - [LIN](#lin)
 - [PWM](#pwm)
 - [RNG](#rng)
 - [SPI](#spi)
 - [TWI](#twi)
 - [UART](#uart)
 - [Watchdog](#watchdog)

## ADC

---
**struct adc_settings**;
> Simple transfer function parameters
### NOTES
> `adc_read()` will apply these to the raw adc value as following:
> - (int)adc_value * <ins>multiplier</ins> / <ins>divider</ins> + <ins>offset</ins>

---
int **adc_setup**(**struct adc** \*<ins>ctx</ins>,
const **struct adc_settings** \*<ins>settings</ins>);
> Configures an ADC channel <ins>ctx</ins>
### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **adc_read**(**struct adc** \*<ins>ctx</ins>, **int** \*<ins>data</ins>);
> Reads a value from the adc channel <ins>ctx</ins> & stores it in 
> \*<ins>data</ins>
### RETURN
> Returns the number of values read (1) if success, -errno otherwise

---
int **adc_read_multiple**(**struct adc** \*<ins>ctx</ins>, 
**int** \*<ins>data</ins>, **size_t** <ins>n</ins>);
> Reads multiple values from the adc channel <ins>ctx</ins> & 
> stores then in <ins>data</ins>[]
### RETURN
> Returns the number of values read if success, -errno otherwise


### ADC + safety

---
struct adc \***adc_claim**(**struct adc** \*<ins>ctx</ins>);
> Claims the adc channel <ins>ctx</ins> & its dependencies
### NOTES
> This feature is related to MPU & will only have an effect if
> `CONFIG_MPU` is active.


## CAN


### CAN + safety


## Clocks

---
clock_freq_t **clock_get_freq**(**clock_id_t** <ins>clkid</ins>);
> Gets the frequency of clock <ins>clkid</ins> (in hz)
### RETURN
> Returns the clock frequency if success, -errno otherwise


## DMA (simple)


### DMA + safety


## DMA Scatter-Gather


## EEPROM


## FLASH (External)


## FLASH (Internal)


## GPIO


### GPIO + safety


## IPWM


## LIN


## PWM


### PWM + safety


## RNG


## SPI


### SPI + safety


## TWI


## UART


### UART + safety


## Watchdog

