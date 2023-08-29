# 1st: sources & targets
target := $(TARGET)

c_src-y += main.c
c_src-y += curiosity-2.0-pic32mz-ef.c
a_src-y += config.S

ifeq ($(skip-makefile),1)

ld_script := $(abspath $(CONFIG_LDSCRIPT:"%"=%))
c_src := $(abspath $(c_src-y))
a_src := $(abspath $(a_src-y))

linkerscript := <itemPath>$(ld_script)</itemPath>
sourcefiles := $(c_src:%=<itemPath>%</itemPath>)
sourcefiles += $(a_src:%=<itemPath>%</itemPath>)

inctree        = $(patsubst %/,%,$(addprefix -I,$(dir $(1))))
c_flags        = -Wp,-MD,$(depfile) $(opt-y) $(debug-y) $(RTOSINCLUDE) -Wpedantic \
                 $(sort $(call inctree,$(c_src-y)) $(call inctree,$(a_src-y))) \
                 $(ccflags-y) $(subst \",,$(CONFIG_EXTRA_CFLAGS))

includedirs := $(filter -I%,$(c_flags))
includedirs := $(abspath $(includedirs:-I%=%;))

$(info === Useful info for MPLABX config file ===)
$(info Linker script:)
$(info $(linkerscript))
$(info Source files:)
$(info $(sourcefiles))
$(info include directories:)
$(info $(includedirs))
$(info === Normal continuation ===)

# avoid double-inclusion
else # $(skip-makefile)
PWD := $(shell pwd)
KBUILD_SRC := ../..

TARGETS := curiosity-2.0-pic32mz-ef_ram.elf
TARGETS += curiosity-2.0-pic32mz-ef_flash.elf

defconfig = $(@:%.elf=%_defconfig)

all: $(TARGETS)
	@

%.elf: FORCE
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $(defconfig)
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) TARGET=$@

%:
	$(Q)$(MAKE) -C $(KBUILD_SRC) O=$(PWD) $@

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
endif # $(skip-makefile)
