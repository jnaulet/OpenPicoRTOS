# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atmega328p/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

# directories
SKETCH := arduino-uno
SKETCH_DIR := sketch/$(SKETCH)

# Required for sketch
SRC_H += $(PICORTOS_DIR)/picoRTOS.h
SRC_H += $(PICORTOS_DIR)/arch/include/picoRTOS_port.h
SRC_H += $(ARCH_DIR)/picoRTOS_types.h
SRC_H += $(DEVICE_DIR)/picoRTOS_device.h
SRC_H += picoRTOSConfig.h

# demo
SRC_C += blink.c

all:
	mkdir -p $(SKETCH_DIR)
	cp $(SKETCH).ino $(SRC_C) $(SRC_S) $(SRC_H) $(SKETCH_DIR)
	arduino-cli compile -b arduino:avr:uno -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

upload:
	arduino-cli upload -b arduino:avr:uno -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf sketch

.PHONY: clean splint
