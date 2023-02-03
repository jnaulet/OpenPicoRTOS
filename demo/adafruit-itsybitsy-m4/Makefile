# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsame5x/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

INCLUDE += -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/usart

# drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-same5x_tc.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-sercom.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-sercom.c
SRC_C += $(PICORTOS_DIR)/drivers/usart/usart-sercom.c

# demo
SRC_C += main.c
SRC_C += adafruit-itsybitsy-m4.c

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

.PHONY: splint
