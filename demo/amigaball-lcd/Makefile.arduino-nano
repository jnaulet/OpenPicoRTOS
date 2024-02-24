# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += lcd.c
c_src-y += amigaball.c
c_src-y += board/$(TARGET:%.elf=%.c)

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

BOARDS := adafruit-itsybitsy-m4
BOARDS += devebox-stm32h7xx_m
BOARDS += launchxl-f28379d
BOARDS += raspberry-pico
BOARDS += sipeed-longan-nano

TARGETS := $(BOARDS:%=%.elf)

# RP2040 UF2
ELF2UF2 := elf2uf2/elf2uf2
TARGETS += raspberry-pico.uf2

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

clean: FORCE
	rm -f $(TARGETS) *.o *.map *.d_raw

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
