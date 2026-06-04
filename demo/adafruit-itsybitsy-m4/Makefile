# 1st: sources & targets
target := adafruit-itsybitsy.elf

c_src-y += main.c
c_src-y += adafruit-itsybitsy-m4.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

all:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
