# MSP430 platforms makefile
arch-msp430 := $(srctree)/arch/msp430

c_src-$(CONFIG_MSP430) += $(arch-msp430)/picoRTOS_port.c
a_src-$(CONFIG_MSP430) += $(arch-msp430)/picoRTOS_portasm.S

# machines
a_src-$(CONFIG_MACH_MSP430G2553) += $(arch-msp430)/mach-msp430g2553/startup.S

# mcu
mcu-$(CONFIG_MACH_MSP430G2553) := -mmcu=msp430g2553

# required
include-$(CONFIG_MSP430) += -I$(arch-msp430)/include \
			    -include $(arch-msp430)/include/errno.h

ccflags-$(CONFIG_MSP430) += $(include-y) $(mcu-y) -std=c99
asflags-$(CONFIG_MSP430) += $(include-y) $(mcu-y)
ldflags-$(CONFIG_MSP430) += $(mcu-y) -nostartfiles
ldflags-$(CONFIG_MSP430) += -L$(arch-msp430)/include -Tsections.ld
