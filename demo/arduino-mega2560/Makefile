# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atmega2560/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

# directories
SKETCH := arduino-mega2560
SKETCH_DIR := sketch/$(SKETCH)

# Required for sketch
SRC_H += $(PICORTOS_DIR)/picoRTOS.h
SRC_H += $(PICORTOS_DIR)/arch/include/picoRTOS_port.h
SRC_H += $(ARCH_DIR)/picoRTOS_types.h
SRC_H += $(DEVICE_DIR)/picoRTOS_device.h
SRC_H += picoRTOSConfig.h

INCLUDE += -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/devices
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/eeprom
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-atmega2560.c
SRC_C += $(PICORTOS_DIR)/drivers/eeprom/eeprom-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-avr.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-avr.c

# sketch files
SRC_H += $(PICORTOS_DIR)/drivers/include/adc.h
SRC_H += $(PICORTOS_DIR)/drivers/adc/adc-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/clock.h
SRC_H += $(PICORTOS_DIR)/drivers/clock/clock-atmega2560.h
SRC_H += $(PICORTOS_DIR)/drivers/include/eeprom.h
SRC_H += $(PICORTOS_DIR)/drivers/eeprom/eeprom-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/gpio.h
SRC_H += $(PICORTOS_DIR)/drivers/gpio/gpio-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/mux.h
SRC_H += $(PICORTOS_DIR)/drivers/mux/mux-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/pwm.h
SRC_H += $(PICORTOS_DIR)/drivers/pwm/pwm-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/uart.h
SRC_H += $(PICORTOS_DIR)/drivers/uart/uart-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/spi.h
SRC_H += $(PICORTOS_DIR)/drivers/spi/spi-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/twi.h
SRC_H += $(PICORTOS_DIR)/drivers/twi/twi-avr.h
SRC_H += $(PICORTOS_DIR)/drivers/include/wd.h
SRC_H += $(PICORTOS_DIR)/drivers/wd/wd-avr.h

# demo
SRC_C += main.c
SRC_C += arduino-mega2560.c
SRC_H += arduino-mega2560.h

CFLAGS := --build-property "compiler.c.flags=-c -g -O0 {compiler.warning_flags} -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects"

all:
	mkdir -p $(SKETCH_DIR)
	cp $(SKETCH).ino $(SRC_C) $(SRC_S) $(SRC_H) $(SKETCH_DIR)
	arduino-cli compile -b arduino:avr:mega $(CFLAGS) \
	  --export-binaries $(SKETCH_DIR)

upload:
	arduino-cli upload -b arduino:avr:mega -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf sketch

.PHONY: clean splint
