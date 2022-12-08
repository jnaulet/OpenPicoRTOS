EXE := win32-blink.exe

CC := gcc
CROSS := i686-w64-mingw32-

PICORTOS_DIR := $(abspath ../..)
BUILD_DIR = build

CFLAGS += -Wall -Wextra -I. -I$(PICORTOS_DIR) -g
CFLAGS += -I$(PICORTOS_DIR)/arch/include
CFLAGS += -I$(PICORTOS_DIR)/arch/win32

LDFLAGS += -g

# picoRTOS
SRC_C_FILES += $(PICORTOS_DIR)/picoRTOS.c
SRC_C_FILES += $(PICORTOS_DIR)/arch/win32/picoRTOS_port.c

# demo
SRC_C_FILES += blink.c

OBJ_FILES := $(SRC_C_FILES:%.c=%.o)
OBJ_FILES += $(SRC_S_FILES:%.S=%.o)

# fix paths
OBJ_FILES := $(addprefix $(BUILD_DIR)/,$(OBJ_FILES))

all: $(EXE)

$(EXE): $(OBJ_FILES)
	-mkdir -p $(@D)
	$(CROSS)$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(EXE)
