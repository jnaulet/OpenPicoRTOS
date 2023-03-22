TESTS := test_picoRTOS test_picoRTOS-SMP

CC := gcc

PICORTOS_DIR :=  $(abspath ../..)
BUILD_DIR = build

CFLAGS := -I.  -Istub -I$(PICORTOS_DIR) -g3 -O0
CFLAGS += -I$(PICORTOS_DIR)/arch/include

LDFLAGS :=

SRC += stub/picoRTOS_port.c
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

all: $(TESTS)

test_picoRTOS: $(OBJ)
	$(CC) $(CFLAGS) -c test.c -o $@.o
	$(CC) $^ $@.o -o $@ $(LDFLAGS)

test_picoRTOS-SMP: $(OBJ)
	$(CC) $(CFLAGS) -DTEST_PICORTOS_SMP -c test.c -o $@.o
	$(CC) $^ $@.o -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -rf $(BUILD_DIR)
	-rm -f $(TESTS) *.o

.PHONY: $(TESTS)
