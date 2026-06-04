# 1st: sources & targets
target :=  devkit-mpc5748g.elf

c_src-y += main.c
c_src-y += devkit-mpc5748g.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../..

all: $(TARGETS)
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) TARGET=$@

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
