# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += launchxl-f28379d.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

TARGETS := launchxl-f28379d_ram.elf
TARGETS += launchxl-f28379d_flash.elf

defconfig = $(@:%.elf=%_defconfig)

all: $(TARGETS)
	@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(defconfig)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
