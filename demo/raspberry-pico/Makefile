# splint test
# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/raspberry/rp2040/Makefile.in

SPLINTFLAGS := -checks +unixlib

INCLUDE += -I.

# drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/dma/dma-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-rp2040_sio.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-arm_pl022.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-dw_apb_i2c.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-arm_pl011.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-rp2040.c

# IPCs 
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# demo
SRC_C += main.c
SRC_C += raspberry-pico.c

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

.PHONY: splint
