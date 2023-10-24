# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += devkit-mpc5748g.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
KBUILD_SRC := ../../..

TARGETS := devkit-mpc5748g_ram.elf
TARGETS += devkit-mpc5748g_flash.elf
TARGETS += devkit-mpc5748g-SMP_ram.elf
TARGETS += devkit-mpc5748g-SMP_flash.elf

defconfig = $(@:%.elf=%_defconfig)

all: $(TARGETS)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(defconfig)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
