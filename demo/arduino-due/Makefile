ELF_RAM := arduino-due_ram.elf
ELF_FLASH := arduino-due_flash.elf

CC := gcc
OBJCOPY := objcopy
CROSS := arm-none-eabi-
BUILD := build

# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsam3x8e/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mcpu=cortex-m3 -mthumb
SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -mcpu=cortex-m3 -mthumb

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

# demo
SRC_C += main.c
SRC_C += arduino-due.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

# custom linker scripts for arduino-cli (bootloader)
LDRAM := $(DEVICE_DIR)/gcc/atsam3x8e_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/atsam3x8e_rom.ld

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

upload:
	$(CROSS)$(OBJCOPY) -O binary $(ELF_RAM) $(ELF_RAM).bin
	arduino-cli upload -b arduino:sam:arduino_due_x -p /dev/ttyACM0 \
	  -l serial -i $(ELF_RAM).bin

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f *.elf

.PHONY: clean splint
