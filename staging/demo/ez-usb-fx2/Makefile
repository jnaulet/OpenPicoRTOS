target := build/ez-usb-fx2.ihx

c_src-y += main.c

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

run: $(target)
	cycfx2prog prg:$<
	sleep 1
	cycfx2prog run
#	delay:10 dbulk:6,-512,5
#	cycfx2prog dbulk:6,-512

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
