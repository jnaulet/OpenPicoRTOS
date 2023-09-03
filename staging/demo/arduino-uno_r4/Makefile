# OpenPicoRTOS basic makefile template
# 'target' is the binary OpenPicoRTOS build system will create
target := arduino-uno_r4_flash.elf

# All your sources can be put insinde 'c_src-y' for c and 'a_src-y' for assembly
c_src-y += main.c
c_src-y += arduino-uno_r4.c

# This makefile will be included by OpenPicoRTOS during build, it is safer to use
# the 'skip-makefile' keyword to avoid local targets inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
# 'KBUILD_SRC' is the directory containing the OpenPicoRTOS original sources
KBUILD_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

upload:
	arm-none-eabi-objcopy -O binary $(target) $(target).bin
	dfu-util --device 0x2341:0x0069,:0x0369 -D $(target).bin -a0

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
