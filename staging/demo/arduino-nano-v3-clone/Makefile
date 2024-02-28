# 1st: sources & targets
target := arduino-nano-v3-clone.elf

c_src-y += main.c
c_src-y += arduino-nano-v3-clone.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..
TTY ?= /dev/ttyUSB0

hex := $(target:%.elf=%.hex)

all: $(hex)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

$(hex): $(target)
	avr-objcopy -O ihex -R .eeprom $< $@

upload: $(hex)
	avrdude -v -patmega328p -carduino -P$(TTY) \
	-b57600 -D -Uflash:w:$<:i

clean: FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@
	@rm -f $(target) $(hex) *.map

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
