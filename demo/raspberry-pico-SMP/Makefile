ELF_RAM := raspberry-pico-SMP_ram.elf
ELF_FLASH := raspberry-pico-SMP_flash.elf

CC := gcc
CROSS := arm-none-eabi-
BUILD := build

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/raspberry/rp2040/Makefile-SMP.in

SPLINTFLAGS := -checks +unixlib

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE) -mcpu=cortex-m0plus
SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE) -mcpu=cortex-m0plus

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

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

all: $(ELF_RAM) $(ELF_FLASH)

$(ELF_RAM): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ -T$(LDRAM) $(LDFLAGS) -o $@

$(ELF_FLASH): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ -T$(LDFLASH) $(LDFLAGS) -o $@

$(BUILD)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(SFLAGS) -c $< -o $@

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -fR $(BUILD)
	-rm -f *.elf

.PHONY: splint
