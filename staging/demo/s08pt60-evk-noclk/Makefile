target := s08pt60-evk.elf
debug := $(target:%.elf=debug/%.elf)

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

else
#
# extend build to debug
#
ifeq ($(CC),sdcc)
$(target): $(debug)

# more included files to add to debug
a_dep-y += $(patsubst -I%/include, %/include/macros.S, \
            $(filter %/include,$(asflags-y)))

$(debug): $(a_src-y) $(a_dep-y) $(c_src-y)
	install -D -m 0644 -t $(dir $@) $^ $(sort $(call cmd_dep_h_c,$(c_src-y)))
	mkdir -p $(dir $@)/generated
	cp -f Makefile.debug $(dir $@)/Makefile
	cp -fr include/generated $(dir $@)
	rm $(dir $@)/autoconf.h
	make -C $(dir $@)

.PHONY: $(debug)

endif
endif
