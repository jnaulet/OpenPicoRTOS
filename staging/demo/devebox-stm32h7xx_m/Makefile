# Build file
ELF_RAM := devebox-stm32h7xx_m_ram.elf
ELF_FLASH := devebox-stm32h7xx_m_flash.elf

CC := gcc
OBJCOPY := objcopy
CROSS := arm-none-eabi-
BUILD := build

# Configure system
PICORTOS_DIR := $(abspath ../../..)
-include $(PICORTOS_DIR)/staging/devices/stm/stm32h743/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16
SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16

INCLUDE += -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/uart

# drivers
# staging
SRC_C += $(PICORTOS_DIR)/staging/drivers/clock/clock-stm32h7xx.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/dma/dma-stm32h7xx.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/gpio/gpio-stm32h7xx.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/mux/mux-stm32h7xx.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/pwm/pwm-stm32h7xx_tim.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/spi/spi-stm32h7xx.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/uart/uart-stm32h7xx.c

# IPCs
INCLUDE += -I$(PICORTOS_DIR)/ipc
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# demo
SRC_C += main.c
SRC_C += stm32h7xx_m.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

LDRAM := $(DEVICE_DIR)/gcc/stm32h743_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/stm32h743_flash_xip.ld

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

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f *.elf *.map

.PHONY: clean splint
