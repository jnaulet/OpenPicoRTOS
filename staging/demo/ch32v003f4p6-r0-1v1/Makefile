# OpenPicoRTOS basic makefile template
# 'target' is the binary OpenPicoRTOS build system will create
target := ch32v003f4p6-r0-1v1.elf
bin    := $(target:%.elf=%.bin)

# All your sources can be put insinde 'c_src-y' for c and 'a_src-y' for assembly
c_src-y += main.c
c_src-y += ch32v003f4p6-r0-1v1.c

# This makefile will be included by OpenPicoRTOS during build, it is safer to use
# the 'skip-makefile' keyword to avoid local targets inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
# 'PICORTOS_SRC' is the directory containing the OpenPicoRTOS original sources
PICORTOS_SRC := ../../..

all: $(bin)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

$(bin): $(target)
	riscv64-unknown-elf-objcopy -O binary $< $@

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
