# HCS08 platforms makefile
arch-hcs08 := $(srctree)/arch/hcs08

# mcu
mcu-$(CONFIG_HCS08) := -ms08 --stack-auto --int-long-reent --out-fmt-elf

# required
include-$(CONFIG_HCS08) += -I$(arch-hcs08)/include

# machines
a_src-$(CONFIG_MACH_MC9S08PTXX) += $(arch-hcs08)/mach-mc9s08ptxx/startup.S

# systick
c_src-$(CONFIG_SYSTICK_MC9S08PTXX_MTIM1) += $(arch-hcs08)/mach-mc9s08ptxx/timer-mtim1.c
c_src-$(CONFIG_SYSTICK_MC9S08PTXX_RTC) += $(arch-hcs08)/mach-mc9s08ptxx/timer-rtc.c

# MC9S08PT60 memory map
mcu-$(CONFIG_MACH_MC9S08PT60) += --xram-loc 0x40 --xram-size 4096 --data-loc 0x40
mcu-$(CONFIG_MACH_MC9S08PT60) += -Wl-bGSINIT=0x1040 -Wl-bCSEG=0x3200
# MC9S08PT32 memory map
mcu-$(CONFIG_MACH_MC9S08PT32) += --xram-loc 0x40 --xram-size 4096 --data-loc 0x40
mcu-$(CONFIG_MACH_MC9S08PT32) += -Wl-bGSINIT=0x8000

c_src-$(CONFIG_HCS08) += $(arch-hcs08)/picoRTOS_port.c
a_src-$(CONFIG_HCS08) += $(arch-hcs08)/picoRTOS_portasm.S

# assembler
sdas-$(CONFIG_HCS08) := sdas6808

ccflags-$(CONFIG_HCS08) += $(cinclude-y) $(include-y) $(mcu-y)
asflags-$(CONFIG_HCS08) += $(include-y)
ldflags-$(CONFIG_HCS08) += $(mcu-y)
