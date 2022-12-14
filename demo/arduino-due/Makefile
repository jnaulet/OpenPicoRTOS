ELF_RAM := arduino-due.elf

CC := gcc
CROSS := arm-none-eabi-
BUILD_DIR := build

# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsam3x8e/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

# directories
SKETCH := arduino-due
SKETCH_DIR := sketch/$(SKETCH)

# IPCs
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_futex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

INCLUDE += -I$(PICORTOS_DIR)/ipc
INCLUDE += -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/can
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# Required for sketch
SRC_H += $(PICORTOS_DIR)/picoRTOS.h
SRC_H += $(PICORTOS_DIR)/arch/include/picoRTOS_port.h
SRC_H += $(ARCH_DIR)/picoRTOS_types.h
SRC_H += $(DEVICE_DIR)/picoRTOS_device.h
SRC_H += $(PICORTOS_DIR)/ipc/picoRTOS_futex.h
SRC_H += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.h
SRC_H += $(PICORTOS_DIR)/ipc/picoRTOS_cond.h
SRC_H += picoRTOSConfig.h

# drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-sam.c
SRC_C += $(PICORTOS_DIR)/drivers/can/can-sam.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-sam_pmc.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-sam_pio.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-sam_pio.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-sam_tc.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-sam.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-sam.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-sam.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-sam.c

# sketch files
SRC_H += $(PICORTOS_DIR)/drivers/include/adc.h
SRC_H += $(PICORTOS_DIR)/drivers/adc/adc-sam.h
SRC_H += $(PICORTOS_DIR)/drivers/include/can.h
SRC_H += $(PICORTOS_DIR)/drivers/can/can-sam.h
SRC_H += $(PICORTOS_DIR)/drivers/include/clock.h
SRC_H += $(PICORTOS_DIR)/drivers/clock/clock-sam_pmc.h
SRC_H += $(PICORTOS_DIR)/drivers/include/gpio.h
SRC_H += $(PICORTOS_DIR)/drivers/gpio/gpio-sam_pio.h
SRC_H += $(PICORTOS_DIR)/drivers/include/mux.h
SRC_H += $(PICORTOS_DIR)/drivers/mux/mux-sam_pio.h
SRC_H += $(PICORTOS_DIR)/drivers/include/pwm.h
SRC_H += $(PICORTOS_DIR)/drivers/include/ipwm.h
SRC_H += $(PICORTOS_DIR)/drivers/pwm/pwm-sam_tc.h
SRC_H += $(PICORTOS_DIR)/drivers/include/spi.h
SRC_H += $(PICORTOS_DIR)/drivers/spi/spi-sam.h
SRC_H += $(PICORTOS_DIR)/drivers/include/twi.h
SRC_H += $(PICORTOS_DIR)/drivers/twi/twi-sam.h
SRC_H += $(PICORTOS_DIR)/drivers/include/uart.h
SRC_H += $(PICORTOS_DIR)/drivers/uart/uart-sam.h
SRC_H += $(PICORTOS_DIR)/drivers/include/wd.h
SRC_H += $(PICORTOS_DIR)/drivers/wd/wd-sam.h

# demo
SRC_C += main.c
SRC_C += arduino-due.c
SRC_H += arduino-due.h

CFLAGS := "compiler.c.flags=-c -g -O0 {compiler.warning_flags} -std=gnu11 -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -Dprintf=iprintf -MMD"

all:
	mkdir -p $(SKETCH_DIR)
	cp $(SKETCH).ino $(SRC_C) $(SRC_S) $(SRC_H) $(SKETCH_DIR)
	arduino-cli compile -b arduino:sam:arduino_due_x \
	  --build-property $(CFLAGS) --export-binaries $(SKETCH_DIR)

upload:
	arduino-cli upload -b arduino:sam:arduino_due_x -p /dev/ttyACM0 \
	  -l serial $(SKETCH_DIR)

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf sketch

.PHONY: clean splint
