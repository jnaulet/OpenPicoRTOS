target := stm8s003f3p3.ihx
debug := $(target:%.elf=debug/%.ihx)

c_src-y += main.c
c_src-y += stm8s003f3p3.c

ifneq ($(skip-makefile),1)
PWD := $(shell pwd)
PICORTOS_SRC := ../../..

all: $(target)
	@

$(target): FORCE
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD)

%:
	$(Q)$(MAKE) -C $(PICORTOS_SRC) O=$(PWD) $@

program:
	stm8flash -p stm8s003f3 -c stlinkv2 -w $(target)

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
