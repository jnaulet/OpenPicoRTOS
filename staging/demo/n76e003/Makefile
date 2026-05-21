target := n76e003.ihx

c_src-y += main.c
c_src-y += n76e003.c

#ccflags-y += --out-fmt-elf
#ldflags-y += --out-fmt-elf
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
	$(NUVOPROG) --program $<

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

endif
