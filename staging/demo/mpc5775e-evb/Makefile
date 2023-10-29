# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += mpc5775e-evb.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
KBUILD_SRC := ../../..

TARGETS := mpc5775e-evb_ram.elf
TARGETS += mpc5775e-evb_flash.elf
TARGETS += mpc5775e-evb-SMP_ram.elf
TARGETS += mpc5775e-evb-SMP_flash.elf

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
