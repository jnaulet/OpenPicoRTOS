# ESP32DOIT-DEVKIT-V1 bare-metal
ELF := esp32doit-devkit-v1.elf

CC := gcc
CROSS := xtensa-esp32-elf-
BUILD_DIR := build

# picoRTOS
PICORTOS_DIR := $(abspath ../..)
-include $(PICORTOS_DIR)/devices/espressif/esp32/Makefile.in

# static analysis
SPLINTFLAGS := -checks +unixlib

DEFINE += -DESP32
INCLUDE += -I.

CFLAGS += -Wall -Wextra -std=c99 $(INCLUDE) -O0 -g3 $(DEFINE)
CFLAGS += -mlongcalls -Wno-frame-address -ffunction-sections -fdata-sections
CFLAGS += -Wno-unused-parameter -Wno-sign-compare -ggdb
CFLAGS += -freorder-blocks -Wwrite-strings -fstack-protector
CFLAGS += -fstrict-volatile-bitfields -fno-jump-tables -fno-tree-switch-conversion
CFLAGS += -Wno-old-style-declaration -MMD

SFLAGS += $(DEFINE) $(INCLUDE) -mlongcalls -ffunction-sections -fdata-sections
SFALGS += -Wno-unused-parameter
SFLAGS += -Wno-sign-compare -ggdb -freorder-blocks -Wwrite-strings
SFLAGS += -fstack-protector -fstrict-volatile-bitfields
SFLAGS += -fno-jump-tables
SFALGS += -fno-tree-switch-conversion -x assembler-with-cpp -MMD

LDRAM := esp32doit-devkit-v1.ld
#LDFLAGS += -Xlinker --gc-sections

# startup
SRC_S += esp32doit-devkit-v1.S

# demo
SRC_C += main.c

OBJ := $(SRC_C:%.c=$(BUILD_DIR)/%.o)
OBJ += $(SRC_S:%.S=$(BUILD_DIR)/%.o)

all: $(ELF)

$(ELF): $(OBJ)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ -T$(LDRAM) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(SFLAGS) -c $< -o $@

splint:
	splint $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(ELF)

.PHONY: clean splint
