# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += stm32h7xx_m.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

TARGETS := devebox-stm32h7xx_m.elf

all: $(TARGETS)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
