target := s08pt60-evk.elf

c_src-y += main.c
c_src-y += s08pt60-evk.c

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

endif
