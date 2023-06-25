# Longan Nano eval board
ELF_FLASH := sipeed-longan-nano-lcd_rom.elf

CC := gcc
OBJCOPY := objcopy
CROSS := riscv64-unknown-elf-
BUILD := build/sipeed-longan-nano

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/gigadevice/gd32vf103/Makefile.in

# static anaylzers
SPLINTFLAGS := -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

INCLUDE += -I. -Iboard

# config
DEFINE += -DCONFIG_SYSCLK_HZ=27000000
DEFINE += -DLCD_FPS=25

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O3 -g3 $(DEFINE)
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
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/drivers/dma/dma-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-gd32vf103.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-gd32vf103_fwdgt.c

# demo
SRC_C += board/sipeed-longan-nano.c
SRC_C += amigaball.c
SRC_C += lcd.c
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

all: $(ELF_RAM) $(ELF_FLASH)

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

upload:
	$(CROSS)$(OBJCOPY) -O binary $(ELF_FLASH) $(ELF_FLASH).bin
	dfu-util -d 28e9:0189 -a 0 -D $(ELF_FLASH).bin

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f $(ELF_RAM) $(ELF_FLASH) *.bin *.map

.PHONY: clean splint