target := devebox-stc-c51.hex

c_src-y += main.c
c_src-y += devebox-stc-c51.c

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

upload: $(target)
	stcgal -P stc12 $(target)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE upload
FORCE:

.PHONY: $(PHONY)

endif
