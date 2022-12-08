# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsam3x8e/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

# directories
SKETCH := arduino-due
SKETCH_DIR := sketch/$(SKETCH)

# IPCs
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# Required for sketch
SRC_H += $(PICORTOS_DIR)/picoRTOS.h
SRC_H += $(PICORTOS_DIR)/arch/include/picoRTOS_port.h
SRC_H += $(ARCH_DIR)/picoRTOS_types.h
SRC_H += $(DEVICE_DIR)/picoRTOS_device.h
SRC_H += picoRTOSConfig.h
SRC_H += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.h
SRC_H += $(PICORTOS_DIR)/ipc/picoRTOS_cond.h

# demo
SRC_C += blink.c

all:
	mkdir -p $(SKETCH_DIR)
	cp $(SKETCH).ino $(SRC_C) $(SRC_S) $(SRC_H) $(SKETCH_DIR)
	arduino-cli compile -b arduino:sam:arduino_due_x -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

upload:
	arduino-cli upload -b arduino:sam:arduino_due_x -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

debug:
	arduino-cli debug -b arduino:sam:arduino_due_x -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf sketch

.PHONY: clean splint
