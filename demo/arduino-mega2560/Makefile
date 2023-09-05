# 1st: sources & targets
target := arduino-mega2560.elf

c_src-y += main.c
c_src-y += arduino-mega2560.c

# avoid double-inclusion
ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
KBUILD_SRC := ../..
TTY ?= /dev/ttyACM0

hex := $(target:%.elf=%.hex)

all: $(hex)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

$(hex): $(target)
	avr-objcopy -O ihex -R .eeprom $< $@

upload: $(hex)
	avrdude -v -patmega2560 -cwiring -P$(TTY) \
	-b115200 -D -Uflash:w:$<:i

clean: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@
	@rm -f $(target) $(hex) *.map

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
