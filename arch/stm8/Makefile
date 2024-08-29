# STM8 platforms makefile
arch-stm8 := $(srctree)/arch/stm8

# mcu
mcu-$(CONFIG_STM8) := -mstm8 --int-long-reent --out-fmt-elf

# required
include-$(CONFIG_STM8) += -I$(arch-stm8)/include

# machines
a_src-$(CONFIG_MACH_STM8S) += $(arch-stm8)/mach-stm8s/startup.S
c_src-$(CONFIG_MACH_STM8S) += $(arch-stm8)/mach-stm8s/timer-awu.c

# STM8S memory map
mcu-$(CONFIG_MACH_STM8S) += --xram-size 1024

c_src-$(CONFIG_STM8) += $(arch-stm8)/picoRTOS_port.c
a_src-$(CONFIG_STM8) += $(arch-stm8)/picoRTOS_portasm.S

# assembler
sdas-$(CONFIG_STM8) := sdasstm8

ccflags-$(CONFIG_STM8) += $(cinclude-y) $(include-y) $(mcu-y)
asflags-$(CONFIG_STM8) += $(include-y)
ldflags-$(CONFIG_STM8) += $(mcu-y)
