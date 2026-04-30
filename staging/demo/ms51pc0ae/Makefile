target := ms51pc0ae.ihx

c_src-y += main.c
c_src-y += ms51pc0ae.c

NUVOPROG := nuvoprog-c

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
	$(NUVOPROG) -p $<

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

endif
