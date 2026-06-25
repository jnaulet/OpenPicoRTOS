TESTS := picoRTOS

CC := gcc

PICORTOS_DIR :=  $(abspath ../..)
BUILD_DIR = build

CFLAGS := -I.  -Istub -I$(PICORTOS_DIR) -g3 -O0
CFLAGS += -I$(PICORTOS_DIR)/arch/include

LDFLAGS :=

SRC += stub/picoRTOS_port.c
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

all: $(TESTS)

picoRTOS: $(OBJ)
	$(CC) $(CFLAGS) -c $@.c -o $@.o
	$(CC) $^ $@.o -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

indent:
	uncrustify -c ../../etc/uncrustify.cfg $(SRC) picoRTOS.c \
	  --replace --no-backup

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(TESTS) *.o

.PHONY: $(TESTS)
