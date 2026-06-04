# 1st: sources & targets
target := sipeed-longan-nano_flash.elf

c_src-y += main.c
c_src-y += sipeed-longan-nano.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

all: $(TARGETS)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
