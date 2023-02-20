# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/microchip/pic32mz-ef/Makefile.in

# static anaylyzer
SPLINTFLAGS := -D__PIC32MZ_EF__ -checks +unixlib

INCLUDE += -I.

# IPCs
INCLUDE += -I$(PICORTOS_DIR)/ipc
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# Drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/misc
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# Drivers
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/misc/misc-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-pic32mx.c

# demo
SRC_C += curiosity-2.0-pic32mz-ef.c
SRC_C += main.c

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

.PHONY: splint
