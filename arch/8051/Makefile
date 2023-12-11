# 8051 platforms makefile
arch-8051 := $(srctree)/arch/8051

c_src-$(CONFIG_8051) += $(arch-8051)/picoRTOS_port.c
a_src-$(CONFIG_8051) += $(arch-8051)/picoRTOS_portasm.S

# mcu
mcu-$(CONFIG_8051) := -mmcs51 --model-large --stack-auto --int-long-reent

# required
cinclude-$(CONFIG_8051) += -I$(arch-8051)/include \
			   --include $(arch-8051)/include/errno.h

# machines
a_src-$(CONFIG_MACH_N76E003) += $(arch-8051)/mach-n76e003/startup.S
a_src-$(CONFIG_MACH_UCSIM51) += $(arch-8051)/mach-ucsim_51/startup.S
a_src-$(CONFIG_MACH_EZUSBFX2) += $(arch-8051)/mach-ezusbfx2/startup.S

# assembler
sdas-$(CONFIG_8051) := sdas8051

ccflags-$(CONFIG_8051) += $(cinclude-y) $(include-y) $(mcu-y)
asflags-$(CONFIG_8051) += $(include-y)
ldflags-$(CONFIG_8051) += $(mcu-y)
