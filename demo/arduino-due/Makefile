# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += arduino-due.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..
TTY ?= /dev/ttyACM0

TARGETS := arduino-due_ram.bin
TARGETS += arduino-due_flash.bin
# prevent make from removing the elfs
TARGETS += $(TARGETS:%.bin=%.elf)

defconfig = $(@:%.elf=%_defconfig)

all: $(TARGETS)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(defconfig)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@

upload: $(TARGETS)
	arduino-cli upload -b arduino:sam:arduino_due_x -p $(TTY) \
	  -l serial -i arduino-due_ram.bin

clean: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) clean
	rm -f $(TARGETS) *.map

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
