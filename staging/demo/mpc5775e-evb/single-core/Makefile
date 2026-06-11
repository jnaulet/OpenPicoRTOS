# 1st: sources & targets
target := mpc5775e-evb.elf

c_src-y += main.c
c_src-y += ../common/mpc5775e-evb.c
c_src-y += ../common/tasks.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
KBUILD_SRC := ../../../..

all:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
