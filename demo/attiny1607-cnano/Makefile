# 1st: sources & targets
target := attiny1607-cnano.elf

c_src-y += main.c
c_src-y += attiny1607-cnano.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

all: $(target)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) CC=xc8-cc

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
