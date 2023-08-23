# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += raspberry-pico.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

ELF2UF2 := elf2uf2/elf2uf2

TARGETS := raspberry-pico_ram.uf2
TARGETS += raspberry-pico_flash.uf2
TARGETS += raspberry-pico-SMP_ram.uf2
TARGETS += raspberry-pico-SMP_flash.uf2
# Needed to tell make not to remove the ELF files
TARGETS += $(TARGETS:%.uf2=%.elf)

defconfig = $(@:%.elf=%_defconfig)

all: $(TARGETS)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(defconfig)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%.uf2: %.elf $(ELF2UF2)
	$(ELF2UF2) $< $@

$(ELF2UF2):
	g++ elf2uf2/main.cpp -o $@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
