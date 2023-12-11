# 1st: sources & targets
target := $(TARGET)
c_src-y += main.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

ARCHS := armv7e-m armv7-m atmega attiny c28x rp2040 e200z4 e200z7
TARGETS := $(ARCHS:%=%.out)

all: $(TARGETS)

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

target: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) \
	  EXTRA_CFLAGS=-DDEVICE_INTERRUPT_COUNT=0 \
	  EXTRA_AFLAGS=-DDEVICE_INTERRUPT_COUNT=0 \
	  TARGET=$@

%.out: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(@:%.out=%)_defconfig
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) \
	  EXTRA_CFLAGS=-DDEVICE_INTERRUPT_COUNT=0 \
	  EXTRA_AFLAGS=-DDEVICE_INTERRUPT_COUNT=0 \
	  TARGET=$@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
