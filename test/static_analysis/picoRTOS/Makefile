PICORTOS_DIR := $(abspath ../../..)

CHECK += arch/arm/cm0+
CHECK += arch/arm/cm3
CHECK += arch/avr/avr5
CHECK += arch/avr/avr6

SPLINT := splint
SPLINTFLAGS := -checks -unixlib -I. -I$(PICORTOS_DIR)
SPLINTFLAGS += -I$(PICORTOS_DIR)/arch/include

CPPCHECK := cppcheck
CPPCHECKFLAGS := --enable=all -DNDEBUG -I. -I$(PICORTOS_DIR)
CPPCHECKFLAGS += -I$(PICORTOS_DIR)/arch/include
CPPCHECKFLAGS += --suppress=missingIncludeSystem

SRC += $(PICORTOS_DIR)/picoRTOS.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_futex.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c
SRC += $(PICORTOS_DIR)/ipc/picoRTOS_queue.c
SRC += test.c

splint: SA := $(SPLINT)
splint: SAFLAGS := $(SPLINTFLAGS)
splint: $(CHECK)

cppcheck: SA := $(CPPCHECK)
cppcheck: SAFLAGS := $(CPPCHECKFLAGS)
cppcheck: $(CHECK)

arch/%:
	$(SA) $(SAFLAGS) $(SRC) \
	  -I$(PICORTOS_DIR)/$@ \
	  -I$(PICORTOS_DIR)/$@/samples \
	  $(PICORTOS_DIR)/$@/picoRTOS_port.c
