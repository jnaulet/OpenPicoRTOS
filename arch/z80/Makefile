# z80 platforms makefile
arch-z80 := $(srctree)/arch/z80

# mcu
mcu-$(CONFIG_Z80) := -mz80 --stack-auto --int-long-reent --no-std-crt0

# required
include-$(CONFIG_Z80) += -I$(arch-z80)/include

# machines
a_src-$(CONFIG_MACH_ZXSPECTRUM) += $(arch-z80)/mach-zxspectrum/startup.S
c_src-$(CONFIG_MACH_ZXSPECTRUM) += $(arch-z80)/mach-zxspectrum/timer-gatearray.c

# ZX Spectrum memory map (EXPERIMENTAL)
mcu-$(CONFIG_MACH_ZXSPECTRUM) += --code-loc 32768
mcu-$(CONFIG_MACH_ZXSPECTRUM) += --data-loc 0
mcu-$(CONFIG_MACH_ZXSPECTRUM) += --iram-size 32768

c_src-$(CONFIG_Z80) += $(arch-z80)/picoRTOS_port.c
a_src-$(CONFIG_Z80) += $(arch-z80)/picoRTOS_portasm.S

# assembler
sdas-$(CONFIG_Z80) := sdasz80

ccflags-$(CONFIG_Z80) += $(cinclude-y) $(include-y) $(mcu-y)
asflags-$(CONFIG_Z80) += $(include-y)
ldflags-$(CONFIG_Z80) += $(mcu-y)
