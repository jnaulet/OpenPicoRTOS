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

---
int **can_setup**(**struct can** \*<ins>ctx</ins>, 
**const struct can_settings** \*<ins>settings</ins>);
> Configures a CAN interface according to <ins>settings</ins>
### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **can_accept**(**struct can** \*<ins>ctx</ins>, 
**can_id_t** <ins>id</ins>, **can_id_t** <ins>accept_mask</ins>);
> Accepts messages according to <ins>id</ins> & <ins>accept_mask</ins> on
> can interface <ins>ctx</ins>

### NOTES
> CAN_ACCEPT_STRICT(_EXT) as acceptance mask means only the specified ID 
> will be accepted.<br>
> Acceptance masks act as a logical AND between the received ID, the
> acceptance mask and the specified id

> - CAN_ACCEPT_STRICT(_EXT) will only accept the specified ID
> - CAN_ACCEPT_ANY will accept any message (mostly for debug purposes)

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **can_write**(**struct can** \*<ins>ctx</ins>, 
**can_id_t** <ins>id</ins>, **const void** \*<ins>buf</ins>, 
**size_t** <ins>n</ins>);
> Sends a message on the CAN interface

### RETURN
> Returns the number of bytes sent in case of success, 
> -errno otherwise

---
int **can_read**(**struct can** \*<ins>ctx</ins>, 
**can_id_t** \*<ins>id</ins>, **void** \*<ins>buf</ins>,
**size_t** <ins>n</ins>);
> Receives a message from the CAN interface

### RETURN
> Returns the number of bytes received in case of success, 
> -errno otherwise

---
int **can_request_frame**(**struct can** \*<ins>ctx</ins>, **can_id_t** <ins>id</ins>);
> Send a RTR frame with the specified <ins>id</ins>

### RETURN
> Returns 0 if success, -errno otherwise


### CAN + safety

---
struct can \***can_claim**(**struct can** \*<ins>ctx</ins>);
> Claims the can for this process/thread (MPU-related feature)
### NOTES
> This feature is related to MPU & will only have an effect if
> `CONFIG_MPU` is active.


## Clocks

---
clock_freq_t **clock_get_freq**(**clock_id_t** <ins>clkid</ins>);
> Gets the frequency of clock <ins>clkid</ins> (in hz)
### RETURN
> Returns the clock frequency if success, -errno otherwise


## DMA (simple)

---
int **dma_setup**(**struct dma** \*<ins>ctx</ins>, const **struct dma_xfer** \*<ins>xfer</ins>);
> Configures a DMA channel <ins>ctx</ins> for external trigger

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **dma_xfer**(**struct dma** \*<ins>ctx</ins>, const **struct dma_xfer** \*<ins>xfer</ins>);
> Manually starts a DMA transfer <ins>xfer</ins>

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **dma_xfer_done**(**struct dma** \*<ins>ctx</ins>);
> Polls a DMA channel for end of transfer

### RETURN
Returns 0 in case of xfer success, -errno otherwise


### DMA + safety

---
struct dma \***dma_claim**(**struct dma** \*<ins>ctx</ins>);
> Claims the dma for this process/thread (opt. MPU-related feature)


## DMA Scatter-Gather

---
int **dmasg_xfer**(**struct dmasg** \*<ins>ctx</ins>, 
const **struct dmasg_xfer** \*<ins>xfer<ins>);
> Starts a DMA scatter-gather transfer <ins>xfer</ins>

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **dmasg_xfer_done**(**struct dmasg** \*<ins>ctx</ins>);
> Polls a DMA-SG for end of transfer

### RETURN
> Returns 0 in case of successful xfer, -errno otherwise


## EEPROM

---
int **eeprom_read**(**struct eeprom** \*<ins>ctx</ins>, 
**size_t** <ins>addr</ins>, **void** \*<ins>buf</ins>, 
**size_t** <ins>n</ins>);
> Reads value from EEPROM

### RETURN
> Returns the number of bytes read if success, -errno otherwise

---
int **eeprom_erase**(**struct eeprom** \*<ins>ctx</ins>, 
**size_t** <ins>addr</ins>, **size_t** <ins>n</ins>);
> Erase data from EEPROM

### RETURN
> Returns the number of bytes erased if success, -errno otherwise

---
int **eeprom_write**(**struct eeprom** \*<ins>ctx</ins>, 
**size_t** <ins>addr</ins>, const **void** \*<ins>buf</ins>,
**size_t** <ins>n</ins>);
> Write values to EEPROM

### RETURN
> Returns the number of bytes written if success, -errno otherwise


## FLASH (External)

---
int **flash_ext_probe**(**struct flash_ext** \*<ins>ctx</ins>);
> Probe the flash

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_ext_get_attributes**(**struct flash_ext** \*<ins>ctx</ins>,
**struct flash_attributes** \*<ins>attr</ins>);
> Gets the flash's attributes

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_ext_erase**(**struct flash_ext** \*<ins>ctx</ins>, 
**size_t** <ins>offset</ins>);
> Erases a sector from flash

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **flash_ext_read**(**struct flash_ext** \*<ins>ctx</ins>, 
**size_t** <ins>offset</ins>, const **void** \*<ins>data</ins>,
**size_t** <ins>n</ins>);
> Reads data from flash

### RETURN
> Returns the number of bytes read if success, -errno otherwise

---
int **flash_ext_write**(**struct flash_ext** \*<ins>ctx</ins>, 
**size_t** <ins>offset</ins>, const **void** \*<ins>data</ins>, 
**size_t** <ins>n</ins>);
> Writes data to flash

### RETURN
> Returns the number of bytes written if success, -errno otherwise

---
int **flash_ext_lock**(**struct flash_ext** \*<ins>ctx</ins>,
**size_t** <ins>offset</ins>);
> Protects a block in flash

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_ext_unlock**(**struct flash_ext** \*<ins>ctx</ins>,
**size_t** <ins>offset</ins>);
> Unprotects a flash block

### RETURN
> Returns 0 if success, -errno otherwise


## FLASH (Internal)

---
int **flash_probe**(**struct flash** \*<ins>ctx</ins>);
> Probe the flash

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_get_attributes**(**struct flash** \*<ins>ctx</ins>,
**struct flash_attributes** \*<ins>attr</ins>);
> Gets the flash's attributes

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_erase**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
> Erases a sector from flash

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **flash_write**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>,
const **void** \*<ins>data</ins>, **size_t** <ins>n</ins>);
> Writes data to flash

### RETURN
> Returns the number of bytes written if success, -errno otherwise

---
int **flash_lock**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
> Protects a block in flash

### RETURN
> Returns 0 if success, -errno otherwise

---
int **flash_unlock**(**struct flash** \*<ins>ctx</ins>, **size_t** <ins>offset</ins>);
> Unprotects a flash block

### RETURN
> Returns 0 if success, -errno otherwise


## GPIO

---
int **gpio_setup**(**struct gpio** \*<ins>ctx</ins>,
const **struct gpio_settings** \*<ins>settings</ins>);
> Configures a gpio

### RETURN
Returns always 0

---
void **gpio_write**(**struct gpio** \*<ins>ctx/<ins>, **bool** <ins>value</ins>);
> Sets gpio value

---
bool **gpio_read**(**struct gpio** \*<ins>ctx</ins>);
> Read gpio value

### RETURN
> Returns true if the gpio is high, false otherwise

---
void **gpio_toggle**(**struct gpio** \*<ins>ctx</ins>);
> Toggles a gpio value


### GPIO + safety

---
struct gpio \***gpio_claim**(**struct gpio** \*<ins>ctx</ins>);
> Claims the gpio for this process/thread (opt. MPU-related feature)


## IPWM

---
int **ipwm_get_period**(**struct ipwm** \*<ins>ctx</ins>,
**pwm_period_us_t** \*<ins>period</ins>);
> Measures the <ins>period</ins> of a train of pulses

### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **ipwm_get_duty_cycle**(**struct ipwm** \*<ins>ctx</ins>, 
**pwm_duty_cycle_t** \*<ins>duty_cycle</ins>);
> Measures the <ins>duty cycle</ins> of a PWM

### RETURN
> Returns 0 in case of success, -errno otherwise


## LIN

---
int **lin_setup**(**struct lin** \*<ins>ctx</ins>, 
const **struct lin_settings** \*<ins>settings</ins>);
> Configures a LIN interface

### RETURN
Returns 0 if success, -errno otherwise

---
int **lin_write**(**struct lin** \*<ins>ctx</ins>, **uint8_t** <ins>id</ins>,
const **void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
> Writes data to the LIN interface

### RETURN
> Returns the number of bytes written if success, -errno otherwise

---
int **lin_read**(**struct lin** \*<ins>ctx</ins>, **uint8_t** \*<ins>id</ins>,
**void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
> Reads data from a LIN interface

### RETURN
Returns the number of bytes read if success, -errno otherwise


## PWM

---
int **pwm_set_period**(**struct pwm** \*<ins>ctx</ins>, **pwm_period_us_t** <ins>period</ins>);
> Defines a PWM period
### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **pwm_set_duty_cycle**(**struct pwm** \*<ins>ctx</ins>, **pwm_duty_cycle_t** <ins>duty_cycle</ins>);
> Defines a PWM duty cycle
### RETURN
> Returns 0 in case of success, -errno otherwise

---
void **pwm_start**(**struct pwm** \*<ins>ctx</ins>);
> Starts the PWM

---
void **pwm_stop**(**struct pwm** \*<ins>ctx</ins>);
> Stops the PWM


### PWM + safety

---
struct pwm \***pwm_claim**(**struct pwm** \*<ins>ctx</ins>);
> Claims the pwm channel <ins>ctx</ins> & its dependencies
### NOTES
> This feature is related to MPU & will only have an effect if
> `CONFIG_MPU` is active.


## RNG

---
int **rng_read**(**struct rng** \*<ins>ctx</ins>, **void** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
> Read an arbitrary number of bytes from a RNG
### RETURN
Returns the number of bytes read if success, -errno otherwise

---
struct rng \***rng_claim**(**struct rng** \*<ins>ctx</ins>);
> Claims the rng for this process/thread (MPU-related feature)


## SPI

---
int **spi_setup**(**struct spi** \*<ins>ctx</ins>, 
const **struct spi_settings** \*<ins>settings</ins>);
> Configure a SPI interface
### RETURN
Returns 0 in case of success, -errno otherwise

---
int **spi_xfer**(**struct spi** \*<ins>ctx</ins>, **void** \*<ins>rx</ins>, 
const **void** \*<ins>tx</ins>, **size_t** <ins>n</ins>);
> Transfers data over a SPI interface
### RETURN
Returns the number of bytes transfered if success, -errno otherwise


### SPI + safety

---
struct spi \***spi_claim**(**struct spi** \*<ins>ctx</ins>);
> Claims the spi channel <ins>ctx</ins> & its dependencies
### NOTES
> This feature is related to MPU & will only have an effect if
> `CONFIG_MPU` is active.


## TWI

---
int **twi_setup**(**struct twi** \*<ins>ctx</ins>, 
const **struct twi_settings** \*<ins>settings</ins>);
> Configures a TWI interface
### RETURN
Returns 0 in case of success, -errno otherwise

---
int **twi_poll**(**struct twi** \*<ins>ctx<ins>);
> Polls TWI interface for transfer (slave mode)
### RETURN
> Returns:
> - TWI_READ if a read is recv
> - TWI_WRITE if write is recv
> - -errno otherwise

---
int **twi_write**(**struct twi** \*<ins>ctx</ins>, const **void** \*<ins>buf</ins>, 
**size_t** <ins>n</ins>, **int** <ins>flags</ins>);
> Write data to a TWI interface
### RETURN
Returns the number of byte written if success, -errno otherwise

---
int **twi_read**(**struct twi** \*<ins>ctx</ins>, **void** \*<ins>buf</ins>, 
**size_t** <ins>n</ins>, **int** <ins>flags</ins>);
> Read data from a TWI interface
### RETURN
> Returns the number of byte read if success, -errno otherwise


## UART

---
int **uart_setup**(**struct uart** \*<ins>ctx</ins>, 
const **struct uart_settings** \*<ins>settings</ins>);
> Configure a UART interface
### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **uart_write**(**struct uart** \*<ins>ctx</ins>, 
const **char** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
> Write data to a UART interface
### RETURN
> Returns the number of bytes written if success, -errno otherwise

---
int **uart_read**(**struct uart** \*<ins>ctx</ins>, 
**char** \*<ins>buf</ins>, **size_t** <ins>n</ins>);
> Read data from a UART interface
### RETURN
> Returns the number of bytes read if success, -errno otherwise


### UART + safety

---
struct uart \***uart_claim**(**struct uart** \*<ins>ctx</ins>);
> Claims the uart channel <ins>ctx</ins> & its dependencies
### NOTES
> This feature is related to MPU & will only have an effect if
> `CONFIG_MPU` is active.


## Watchdog

---
int **wd_start**(**struct wd** \*<ins>ctx</ins>);
> Starts the watchdog
### RETURN
> Returns 0 in case of success, -errno otherwise

---
int **wd_stop**(**struct wd** \*<ins>ctx</ins>);
> Stops the watchdog
### RETURN
> Returns 0 in case of success, -errno otherwise

---
void **wd_refresh**(**struct wd** \*<ins>ctx</ins>);
> Refreshes the watchdog

