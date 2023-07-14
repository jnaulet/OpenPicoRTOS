# Raspberry pico board
ELF_RAM := raspberry-pico-lcd_ram.elf
ELF_FLASH := raspberry-pico-lcd_flash-xip.elf

CC := gcc
CROSS := arm-none-eabi-
BUILD := build/raspberry-pico
ELF2UF2 := $(BUILD)/elf2uf2

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/raspberry/rp2040/Makefile-SMP.in

# static anaylzers
SPLINTFLAGS := -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

INCLUDE += -I. -Iboard

# config
DEFINE += -DCONFIG_SYSCLK_HZ=125000000 -DCONFIG_SMP_CORES=2
DEFINE += -DLCD_FPS=25

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O3 -g3 $(DEFINE)
CFLAGS += $(MCPU_MFLOAT)

SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += $(MCPU_MFLOAT)

LDFLAGS += $(MCPU_MFLOAT)

# drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/misc
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/dma/dma-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-rp2040_sio.c
SRC_C += $(PICORTOS_DIR)/drivers/misc/misc-rp2040_reset.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-rp2040.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-arm_pl022.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-rp2040.c

# demo
SRC_C += board/raspberry-pico.c
SRC_C += amigaball.c
SRC_C += lcd.c
SRC_C += main.c

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

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f $(ELF_RAM) $(ELF_FLASH) *.uf2 *.map

.PHONY: clean splint
