# MIPS arch makefile
arch-mips := $(srctree)/arch/mips32

c_src-$(CONFIG_M51XX) += $(arch-mips)/m51xx/picoRTOS_port.c
a_src-$(CONFIG_M51XX) += $(arch-mips)/m51xx/picoRTOS_portasm.S

# machines
c_src-$(CONFIG_MACH_PIC32MZ) += $(arch-mips)/m51xx/mach-pic32mz/intc.c
a_src-$(CONFIG_MACH_PIC32MZ) += $(arch-mips)/m51xx/mach-pic32mz/startup.S

# include
include-$(CONFIG_M51XX) += -I$(arch-mips)/m51xx
include-$(CONFIG_MACH_PIC32MZ) += -I$(arch-mips)/m51xx/mach-pic32mz

ccflags-$(CONFIG_MIPS) += $(include-y)
asflags-$(CONFIG_MIPS) += $(include-y)
