target := ch552.ihx

c_src-y += main.c
c_src-y += ch552.c

OBJCOPY := objcopy
VNPROCH55X := vnproch55x

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

upload: $(target)
	$(OBJCOPY) -Iihex -Obinary  $(target) $(target:%.ihx=%.bin)
	$(VNPROCH55X) $(target:%.ihx=%.bin)

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

endif
