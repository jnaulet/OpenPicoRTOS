PICORTOS_DIR := $(abspath ../../..)

CHECK += arch/arm/cm0+
CHECK += arch/arm/cm3
CHECK += arch/avr/avr5
CHECK += arch/avr/avr6

SPLINT := splint
SPLINTFLAGS := -checks -unixlib -I. -I$(PICORTOS_DIR)
SPLINTFLAGS += -I$(PICORTOS_DIR)/arch/include

SRC += $(PICORTOS_DIR)/picoRTOS.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_futex.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_queue.c
SRC += test.c

all: $(CHECK)

arch/%:
	$(SPLINT) $(SPLINTFLAGS) $(SRC) \
	  -I$(PICORTOS_DIR)/$@ \
	  -I$(PICORTOS_DIR)/$@/samples \
	  $(PICORTOS_DIR)/$@/picoRTOS_port.c
