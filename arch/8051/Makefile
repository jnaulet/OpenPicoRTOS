# 8051 platforms makefile
arch-8051 := $(srctree)/arch/8051

# mcu
mcu-$(CONFIG_8051) := -mmcs51 --model-large --stack-auto --int-long-reent

# required
include-$(CONFIG_8051) += -I$(arch-8051)/include

# machines
a_src-$(CONFIG_MACH_N76E003) += $(arch-8051)/mach-n76e003/startup.S
c_src-$(CONFIG_MACH_N76E003) += $(arch-8051)/mach-n76e003/timer-t2.c
c_src-$(CONFIG_MACH_N76E003) += $(arch-8051)/mach-n76e003/interrupt.c
a_src-$(CONFIG_MACH_UCSIM51) += $(arch-8051)/mach-ucsim_51/startup.S
c_src-$(CONFIG_MACH_UCSIM51) += $(arch-8051)/mach-mcs51/timer-t2.c
c_src-$(CONFIG_MACH_UCSIM51) += $(arch-8051)/mach-mcs51/interrupt.c
a_src-$(CONFIG_MACH_CY7C6801xA) += $(arch-8051)/mach-cy7c6801xa/startup.S
c_src-$(CONFIG_MACH_CY7C6801xA) += $(arch-8051)/mach-cy7c6801xa/timer-t2.c
c_src-$(CONFIG_MACH_CY7C6801xA) += $(arch-8051)/mach-cy7c6801xa/interrupt.c

# N76E003 memory map
mcu-$(CONFIG_MACH_N76E003) += --xram-size 768
mcu-$(CONFIG_MACH_N76E003) += --code-size 18432
# CY7C6801xA memory map (shared CODE/XDATA RAM)
ifeq ($(CONFIG_MACH_CY7C6801xA),y)
code-size := $(shell expr 16384 - $(CONFIG_XDATA_SIZE))
mcu-$(CONFIG_MACH_CY7C6801xA) += --xram-loc $(code-size)
mcu-$(CONFIG_MACH_CY7C6801xA) += --xram-size $(CONFIG_XDATA_SIZE)
mcu-$(CONFIG_MACH_CY7C6801xA) += --code-size $(code-size)
endif

c_src-$(CONFIG_8051) += $(arch-8051)/picoRTOS_port.c
a_src-$(CONFIG_8051) += $(arch-8051)/picoRTOS_portasm.S

# assembler
sdas-$(CONFIG_8051) := sdas8051

ccflags-$(CONFIG_8051) += $(cinclude-y) $(include-y) $(mcu-y)
asflags-$(CONFIG_8051) += $(include-y)
ldflags-$(CONFIG_8051) += $(mcu-y)
