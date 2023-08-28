# 1st: sources & targets
target := dummy
c_src-y += test.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
KBUILD_SRC := ../../..

TARGETS := $(notdir $(wildcard configs/*_defconfig))

all: $(TARGETS)
	@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

$(TARGETS): FORCE
	@cat configs/$@ configs/base.config > .config
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) oldnoconfig
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) staticcheck

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
