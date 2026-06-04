# 1st: sources & targets
target := arduino-due.elf

c_src-y += main.c
c_src-y += arduino-due.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..
TTY ?= /dev/ttyACM0

all:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD)

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@

upload: arduino-due.bin
	arduino-cli upload -b arduino:sam:arduino_due_x -p $(TTY) \
	  -l serial -i $<

clean: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) clean
	rm -f $(TARGETS) *.map

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
