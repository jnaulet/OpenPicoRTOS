# Build file for PIC32Mx
ELF_RAM := curiosity-2.0-pic32mz-ef_ram.elf
ELF_FLASH := curiosity-2.0-pic32mz-ef_flash.elf

CC := gcc
CROSS := xc32-
BUILD := build

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/microchip/pic32mz-ef/Makefile.in

# static anaylyzer
SPLINTFLAGS := -D__PIC32MZ_EF__ -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

INCLUDE += -I.

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mprocessor=32MZ2048EFM144 -fno-common -MP -MMD -MF -DXPRJ_default=default

SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -mprocessor=32MZ2048EFM144  -MMD -MF -DXPRJ_default=default

LDFLAGS += -nostartfiles -nostdlib

# IPCs
INCLUDE += -I$(PICORTOS_DIR)/ipc
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# Drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/can
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/misc
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# Drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/can/can-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/misc/misc-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-pic32mx.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-pic32mx.c

# demo
SRC_S += config.S
SRC_C += curiosity-2.0-pic32mz-ef.c
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

LDRAM := $(DEVICE_DIR)/gcc/pic32mz-efm_2048_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/pic32mz-efm_2048_rom.ld

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

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f *.elf

.PHONY: clean splint
