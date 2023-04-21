# Longan Nano eval board
ELF_RAM := longan-nano_ram.elf
ELF_FLASH := longan-nano_rom.elf

CC := gcc
CROSS := riscv64-unknown-elf-
BUILD := build

# picoRTOS
PICORTOS_DIR := $(abspath ../../..)
-include $(PICORTOS_DIR)/staging/devices/gigadevice/gd32vf103/Makefile.in

# static anaylzers
SPLINTFLAGS := -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

INCLUDE += -I.

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -ffreestanding -fmessage-length=0
CFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medlow

SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -fmessage-length=0 -march=rv32imac -mabi=ilp32 -mcmodel=medlow

LDRAM := $(DEVICE_DIR)/gcc/gd32vf103cbt6_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/gd32vf103cbt6_rom.ld

LDFLAGS += -Xlinker --gc-sections -nostdlib -nostartfiles
LDFLAGS += -march=rv32imac -mabi=ilp32 -mcmodel=medlow

# drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/can
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/flash
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/wd

SRC_C += $(PICORTOS_DIR)/staging/drivers/adc/adc-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/can/can-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/clock/clock-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/dma/dma-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/flash/flash-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/gpio/gpio-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/mux/mux-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/pwm/pwm-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/spi/spi-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/twi/twi-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/uart/uart-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/wd/wd-gd32vf103_fwdgt.c

# ipcs
INCLUDE += -I$(PICORTOS_DIR)/ipc
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# demo
SRC_C += longan-nano.c
SRC_C += main.c

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

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f $(ELF_RAM) $(ELF_FLASH)

.PHONY: clean splint
