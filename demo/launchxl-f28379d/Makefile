# LAUNCHXL-F28379D board
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/ti/f28379x/Makefile.in

ELF_RAM := launchxl-f28379d_ram.elf
ELF_FLASH := launchxl-f28379d_flash.elf

CC := cl2000
OPT_LEVEL := -O0

# directories
BUILD := build
TOOLCHAIN_DIR := $(dir $(shell which $(CC)))..

INCLUDE += -I.
INCLUDE_EXTRA += -I$(TOOLCHAIN_DIR)/include

CFLAGS += -v28 -ml -mt $(OPT_LEVEL) $(DEFINE) $(INCLUDE) $(INCLUDE_EXTRA)
CFLAGS += --float_support=fpu64 --preproc_with_compile --abi=eabi
CFLAGS += --preproc_dependency="$(basename $(<F)).d_raw"

SFLAGS := $(CFLAGS)

LDFLAGS += -v28 -ml -mt $(OPT_LEVEL) -g --abi=eabi -z --stack_size=0x100 --float_support=fpu64
LDFLAGS += $(INCLUDE) --warn_sections --reread_libs --priority --disable_auto_rts --rom_model
LDFLAGS += --search_path="$(TOOLCHAIN_DIR)/lib" -lrts2800_fpu64_eabi.lib $(ENTRY_POINT)

LDRAM := --define=_RAM --define=CPU1 -lf28379d_ram.cmd
LDFLASH := --define=_FLASH --define=CPU1 -lf28379d_rom.cmd

SPLINTFLAGS := -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

# drivers
SRC_C += $(PICORTOS_DIR)/drivers/clock/clock-f28379x.c
SRC_C += $(PICORTOS_DIR)/drivers/adc/adc-ti_type4.c
SRC_C += $(PICORTOS_DIR)/drivers/can/can-ti_dcan.c
SRC_C += $(PICORTOS_DIR)/drivers/gpio/gpio-ti_f28x.c
SRC_C += $(PICORTOS_DIR)/drivers/mux/mux-ti_f28x.c
SRC_C += $(PICORTOS_DIR)/drivers/pwm/pwm-ti_epwm.c
SRC_C += $(PICORTOS_DIR)/drivers/spi/spi-ti_f28x.c
SRC_C += $(PICORTOS_DIR)/drivers/twi/twi-ti_f28x.c
SRC_C += $(PICORTOS_DIR)/drivers/wd/wd-ti_f28x.c

INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/drivers/devices
INCLUDE += -I$(PICORTOS_DIR)/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/drivers/can
INCLUDE += -I$(PICORTOS_DIR)/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/drivers/spi
INCLUDE += -I$(PICORTOS_DIR)/drivers/twi
INCLUDE += -I$(PICORTOS_DIR)/drivers/wd

# IPCs
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c
INCLUDE += -I$(PICORTOS_DIR)/ipc

# demo
SRC_C += launchxl-f28379d.c
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD)/%.o)
OBJ += $(SRC_S:%.asm=$(BUILD)/%.o)

all: $(ELF_RAM) $(ELF_FLASH)

asm: CFLAGS += --skip_assembler
asm: $(OUT)

$(ELF_RAM): $(OBJ)
	-mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ $(LDRAM) -o $@ --map_file=$@.map

$(ELF_FLASH): $(OBJ)
	-mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ $(LDFLASH) -o $@ --map_file=$@.map

$(BUILD)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< --output_file=$@

$(BUILD)/%.o: %.asm
	-mkdir -p $(@D)
	$(CC) $(SFLAGS) -c $< --output_file=$@

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -fr $(BUILD)
	-rm -f $(OUT) $(OBJ) *.d_raw *.map

.PHONY: clean splint
