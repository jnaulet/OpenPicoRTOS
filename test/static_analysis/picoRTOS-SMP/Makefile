PICORTOS_DIR := $(abspath ../../..)

CHECK += arch/arm/cm0+/rp2040

SPLINT := splint
SPLINTFLAGS := -checks -I. -unixlib -I$(PICORTOS_DIR)
SPLINTFLAGS += -I$(PICORTOS_DIR)/arch/include

SRC += $(PICORTOS_DIR)/picoRTOS-SMP.c
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
	  $(PICORTOS_DIR)/$@/picoRTOS_port.c \
	  $(PICORTOS_DIR)/$@/picoRTOS-SMP_port.c

# special case unfortunately
arch/arm/cm0+/rp2040:
	$(SPLINT) $(SPLINTFLAGS) $(SRC) \
	  -I$(PICORTOS_DIR)/$@ \
	  -I$(PICORTOS_DIR)/$@/.. \
	  -I$(PICORTOS_DIR)/$@/samples \
	  $(PICORTOS_DIR)/$@/../picoRTOS_port.c \
	  $(PICORTOS_DIR)/$@/picoRTOS-SMP_port.c
