# OpenPicoRTOS basic makefile template
# 'target' is the binary OpenPicoRTOS build system will create
target := mh-tiny.elf
debug  := debug/mh-tiny.elf
hex    := $(target:%.elf=%.hex)

# All your sources can be put insinde 'c_src-y' for c and 'a_src-y' for assembly
c_src-y += main.c
c_src-y += mh-tiny.c

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

$(hex): $(target)
	avr-objcopy -O ihex -R .eeprom $< $@

upload: $(hex)
	avrdude -p attiny88 -c atmelice_isp -D \
	  -Uflash:w:$<:i

fuses:
	avrdude -p attiny88 -c atmelice_isp -D \
	  -Ulfuse:w:0xec:m -Uhfuse:w:0x9f:m -Uefuse:w:0xff:m

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

else
#
# extend build for debug
#
#$(target): $(debug)

# Copy sources to debug/
#$(debug): $(a_src-y) $(a_dep-y) $(c_src-y)
#	install -D -m 0644 -t $(dir $@) $^ $(sort $(call cmd_dep_h_c,$(c_src-y)))
#	mkdir -p $(dir $@)/generated
#	cp -f $(dir $@)/autoconf.h $(dir $@)/generated
#	rm -f $(dir $@)/autoconf.h
#	cp -f Makefile.debug $(dir $@)/Makefile
#	Some cflags
#	@echo "== Additional options =="
#	@echo '$(ccattributes)'
#	@echo "== =="

#PHONY += $(local-y)

endif

.PHONY: $(PHONY)
