target := zx-spectrum+2.ihx
C2TZX  := c2tzx-ng

c_src-y += main.c

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

tzx: zx-spectrum+2.ihx
	objcopy -I ihex -O binary $< $(<:%.ihx=%.bin)
	$(C2TZX) -i $(<:%.ihx=%.bin) -o $(<:%.ihx=%.tzx) 

zx-spectrum+2.ihx: FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

endif
