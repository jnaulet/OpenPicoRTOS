PICORTOS_DIR := $(abspath ../../..)

DEVICES := $(shell find $(PICORTOS_DIR)/devices -name "Makefile.in")
DEVICES += $(shell find $(PICORTOS_DIR)/staging/devices -name "Makefile.in")

# recursion...
-include $(DEVICE)

INCLUDE += -I.

SPLINT := splint
SPLINTFLAGS := -checks -unixlib

CPPCHECK := cppcheck
CPPCHECKFLAGS := --enable=all --inline-suppr --error-exitcode=1
CPPCHECKFLAGS += -DNDEBUG --suppress=missingIncludeSystem

SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_futex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_mutex.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_cond.c
SRC_C += $(PICORTOS_DIR)/ipc/picoRTOS_queue.c
SRC_C += test.c

all:
	for d in $(DEVICES); do \
	$(MAKE) check DEVICE=$$d || exit; \
	done

check:
	$(SPLINT) $(SPLINTFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)
	$(CPPCHECK) $(CPPCHECKFLAGS) $(DEFINE) $(INCLUDE) $(SRC_C)
