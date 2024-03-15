# OpenPicoRTOS basic makefile template
# 'target' is the binary OpenPicoRTOS build system will create
target := yd-stm32l431rc.elf

# All your sources can be put insinde 'c_src-y' for c and 'a_src-y' for assembly
c_src-y += main.c
c_src-y += yd-stm32l431rc.c

# This makefile will be included by OpenPicoRTOS during build, it is safer to use
# the 'skip-makefile' keyword to avoid local targets inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
# 'PICORTOS_SRC' is the directory containing the OpenPicoRTOS original sources
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

endif

.PHONY: $(PHONY)
