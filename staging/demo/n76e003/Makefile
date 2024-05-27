target := n76e003.ihx
debug := $(target:%.ihx=debug/%.hex)

c_src-y += main.c
c_src-y += n76e003.c

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

# Copy sources to ./debug/
local-y := $(patsubst %,./debug/%, \
	     $(notdir $(a_src-y) $(c_src-y)))

# more included files to add to debug
a_dep-y += $(patsubst -I%/include, %/include/macros.S, \
            $(filter %/include,$(asflags-y)))

$(local-y): $(a_src-y) $(a_dep-y) $(c_src-y)
	install -D -m 0644 -t $(dir $@) $^ $(sort $(call cmd_dep_h_c,$(c_src-y)))

$(debug): $(local-y)
	mkdir -p $(dir $@)/generated
	cp -f Makefile.debug $(dir $@)/Makefile
	cp -fr include/* $(dir $@)
	rm -f $(dir $@)/autoconf.h
	make -C $(dir $@)

.PHONY: $(local-y)

endif
endif
