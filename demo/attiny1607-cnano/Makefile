# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/attiny1607/Makefile.in

# static anaylyzer
SPLINTFLAGS := -D__ATtiny1607__ -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction

INCLUDE += -I.

# Drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# Drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-tinyavr_tca.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-tinyavr.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-tinyavr_usart.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-tinyavr.c

# demo
SRC_C += attiny1607-cnano.c
SRC_C += main.c

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

.PHONY: splint
