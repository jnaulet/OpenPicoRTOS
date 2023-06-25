ifneq ($(SMP),)
POSTFIX := -SMP
SUPPORT_FOR_SMP := -DSUPPORT_FOR_SMP
endif

# MPC5775E SMP test
ELF_RAM := mpc5775e-evb$(POSTFIX)_ram.elf
ELF_FLASH := mpc5775e-evb$(POSTFIX)_flash.elf

CC := gcc
CROSS := powerpc-eabivle-
BUILD := build

# picoRTOS
PICORTOS_DIR := $(abspath ../../..)
-include $(PICORTOS_DIR)/staging/devices/nxp/mpc5775e/Makefile$(POSTFIX).in

# static anaylzers
SPLINTFLAGS := -checks +unixlib
CPPCHECKFLAGS += --enable=all --inline-suppr -DNDEBUG
CPPCHECKFLAGS += --suppress=missingIncludeSystem
CPPCHECKFLAGS += --suppress=unusedFunction
CPPCHECKFLAGS += --suppress=unusedStructMember

INCLUDE += -I.

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -fmessage-length=0 -msdata=eabi -mlra -funsigned-bitfields
CFLAGS += -ffunction-sections -fdata-sections -fno-common -mcpu=e200z7
CFLAGS += -mbig -mvle -mregnames -mhard-float $(SUPPORT_FOR_SMP)

SFLAGS += -x assembler-with-cpp -g3 $(DEFINE) $(INCLUDE)
SFLAGS += -g3 -mcpu=e200z7 -mbig -mvle -mregnames

LDRAM :=  $(DEVICE_DIR)/gcc/mpc5775e_ram.ld
LDFLASH := $(DEVICE_DIR)/gcc/mpc5775e_rom.ld

LDFLAGS += -Xlinker --gc-sections -mcpu=e200z7 -mhard-float -nostdlib

# drivers
INCLUDE += -I$(PICORTOS_DIR)/drivers/include
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/adc
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/can
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/clock
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/dma
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/flash
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/gpio
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/misc
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/mux
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/pwm
INCLUDE += -I$(PICORTOS_DIR)/staging/drivers/spi

SRC_C += $(PICORTOS_DIR)/staging/drivers/adc/adc-nxp_eqadc.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/can/can-nxp_flexcan.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/clock/clock-nxp_siu.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/dma/dma-nxp_edma.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/flash/flash-nxp_c55fmc.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/gpio/gpio-nxp_siu.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/misc/misc-nxp_igf.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/mux/mux-nxp_siu.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/pwm/pwm-nxp_emios.c
SRC_C += $(PICORTOS_DIR)/staging/drivers/spi/spi-nxp_dspi.c

# IPCs 
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c

# demo
SRC_C += mpc5775e-evb.c
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
