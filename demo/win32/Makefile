# 1st: sources & targets
target := win32.exe

c_src-y += main.c

# avoid double-inclusion
ifeq ($(skip-makefile),)
PWD := $(shell pwd)
KBUILD_SRC := ../..

all: $(target)
	@

%.exe: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif
