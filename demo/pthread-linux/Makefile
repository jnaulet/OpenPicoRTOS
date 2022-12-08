ELF := pthread-linux.elf

CC := gcc

PICORTOS_DIR := $(abspath ../..)
BUILD_DIR = build

CFLAGS += -Wall -Wextra -I. -I$(PICORTOS_DIR) -g
CFLAGS += -I$(PICORTOS_DIR)/arch/include
CFLAGS += -I$(PICORTOS_DIR)/arch/pthread/linux

LDFLAGS += -g -pthread

# picoRTOS
SRC_C_FILES += $(PICORTOS_DIR)/picoRTOS.c
SRC_C_FILES += $(PICORTOS_DIR)/arch/pthread/linux/picoRTOS_port.c

# demo
SRC_C_FILES += blink.c

OBJ_FILES := $(SRC_C_FILES:%.c=$(BUILD_DIR)/%.o)
OBJ_FILES += $(SRC_S_FILES:%.S=$(BUILD_DIR)/%.o)

all: $(ELF)

$(ELF): $(OBJ_FILES)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(ELF)
