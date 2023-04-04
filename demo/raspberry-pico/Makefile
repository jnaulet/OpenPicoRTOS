ELF_RAM := raspberry-pico_ram.elf
ELF_FLASH := raspberry-pico_flash.elf

CC := gcc
CROSS := arm-none-eabi-
BUILD := build
ELF2UF2 := $(BUILD)/elf2uf2

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/raspberry/rp2040/Makefile.in

SPLINTFLAGS := -checks +unixlib

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mcpu=cortex-m0plus

SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -mcpu=cortex-m0plus

LDFLAGS += -mcpu=cortex-m0plus

INCLUDE += -I.

# drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/misc
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
SRC_C += $(PICORTOS_DIR)/drivers/misc/misc-rp2040_reset.c
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

UF2_RAM := $(ELF_RAM:%.elf=%.uf2)
UF2_FLASH := $(ELF_FLASH:%.elf=%.uf2)

all: $(UF2_RAM) $(UF2_FLASH)

$(ELF_RAM): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ -T$(LDRAM) $(LDFLAGS) -o $@ -Wl,-Map=$@.map

$(ELF_FLASH): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ -T$(LDFLASH) $(LDFLAGS) -o $@ -Wl,-Map=$@.map

$(BUILD)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(SFLAGS) -c $< -o $@

%.uf2: %.elf $(ELF2UF2)
	$(ELF2UF2) $< $@

$(ELF2UF2):
	g++ elf2uf2/main.cpp -o $@

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -fR $(BUILD)
	-rm -f *.elf

.PHONY: splint
