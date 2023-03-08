# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsame5x/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

# directories
SKETCH := adafruit-itsybitsy-m4
SKETCH_DIR := sketch/$(SKETCH)

# Required for sketch
SRC_H += $(PICORTOS_DIR)/picoRTOS.h
SRC_H += $(PICORTOS_DIR)/arch/include/picoRTOS_port.h
SRC_H += $(ARCH_DIR)/picoRTOS_types.h
SRC_H += $(DEVICE_DIR)/picoRTOS_device.h
SRC_H += picoRTOSConfig.h

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

# sketch files
SRC_H += $(PICORTOS_DIR)/drivers/include/adc.h
SRC_H += $(PICORTOS_DIR)/drivers/adc/adc-same5x.h
SRC_H += $(PICORTOS_DIR)/drivers/include/clock.h
SRC_H += $(PICORTOS_DIR)/drivers/clock/clock-same5x.h
SRC_H += $(PICORTOS_DIR)/drivers/include/gpio.h
SRC_H += $(PICORTOS_DIR)/drivers/gpio/gpio-same5x.h
SRC_H += $(PICORTOS_DIR)/drivers/include/mux.h
SRC_H += $(PICORTOS_DIR)/drivers/mux/mux-same5x.h
SRC_H += $(PICORTOS_DIR)/drivers/include/pwm.h
SRC_H += $(PICORTOS_DIR)/drivers/pwm/pwm-same5x_tc.h
SRC_H += $(PICORTOS_DIR)/drivers/include/spi.h
SRC_H += $(PICORTOS_DIR)/drivers/spi/spi-sercom.h
SRC_H += $(PICORTOS_DIR)/drivers/include/uart.h
SRC_H += $(PICORTOS_DIR)/drivers/uart/uart-sercom.h

# demo
SRC_C += main.c
SRC_C += adafruit-itsybitsy-m4.c
SRC_H += adafruit-itsybitsy-m4.h

SFLAGS += --build-property "compiler.S.extra_flags=-mcpu={build.mcu}"

all:
	mkdir -p $(SKETCH_DIR)
	cp $(SKETCH).ino $(SRC_C) $(SRC_S) $(SRC_H) $(SKETCH_DIR)
	arduino-cli compile -b adafruit:samd:adafruit_itsybitsy_m4 \
	 $(SFLAGS) --export-binaries $(SKETCH_DIR)

upload:
	arduino-cli upload -b adafruit:samd:adafruit_itsybitsy_m4 -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf sketch

.PHONY: clean splint
