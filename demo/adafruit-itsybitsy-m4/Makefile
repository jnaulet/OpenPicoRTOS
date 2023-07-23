# Build file for Arduino (no probe required)
ELF_RAM := adafruit-itsybitsy_ram.elf
ELF_FLASH := adafruit-itsybitsy_flash.elf

CC := gcc
OBJCOPY := objcopy
CROSS := arm-none-eabi-
BUILD := build

# Configure system
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/atmel/atsamd51/Makefile.in

INCLUDE += -I.
SPLINTFLAGS += -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16
SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16

INCLUDE += -I$(PICORTOS_DIR)/arch/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/drivers/flash
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/uart
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# drivers
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/dma/dma-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/flash/flash-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-same5x.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-same5x_tc.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-atmel_sercom.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-atmel_sercom.c
SRC_C += $(PICORTOS_DIR)/drivers/uart/uart-atmel_sercom.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-same5x.c

# staging
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/can

SRC_C += $(PICORTOS_DIR)/staging/drivers/can/can-same5x.c

# demo
SRC_C += main.c
SRC_C += adafruit-itsybitsy-m4.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD)/%.o)

# custom linker scripts for arduino-cli (bootloader)
# Original files are in devices/atmel/atsame5x/gcc/samd51g_xxx.ld
LDRAM := $(DEVICE_DIR)/gcc/atsamd51g19a_arduino-boot_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/atsamd51g19a_arduino-boot_flash_xip.ld

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
	arduino-cli upload -b adafruit:samd:adafruit_itsybitsy_m4 -p /dev/ttyACM0 \
	  -l serial -i $(ELF_RAM).bin

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD)
	-rm -f *.elf

.PHONY: clean splint
