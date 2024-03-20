# AVR platforms makefile
arch-avr := $(srctree)/arch/avr

c_src-$(CONFIG_AVR) += $(arch-avr)/picoRTOS_port.c
a_src-$(CONFIG_AVR) += $(arch-avr)/picoRTOS_portasm.S

c_src-$(CONFIG_MACH_ATMEGA2560) += $(arch-avr)/timer-tc0.c
c_src-$(CONFIG_MACH_ATMEGA328P) += $(arch-avr)/timer-tc0.c
c_src-$(CONFIG_MACH_ATMEGA32U4) += $(arch-avr)/timer-tc0.c
c_src-$(CONFIG_MACH_ATTINY88)   += $(arch-avr)/mach-attiny88/timer-tc0.c
c_src-$(CONFIG_MACH_ATTINY817)  += $(arch-avr)/timer-rtc.c
c_src-$(CONFIG_MACH_ATTINY1607) += $(arch-avr)/timer-rtc.c
c_src-$(CONFIG_MACH_LGT8FX8P)   += $(arch-avr)/timer-tc0.c

# mcu
mcu-$(CONFIG_MACH_ATMEGA2560) := -mmcu=atmega2560 -D__ATmega2560__
mcu-$(CONFIG_MACH_ATMEGA328P) := -mmcu=atmega328p -D__ATmega328p__
mcu-$(CONFIG_MACH_ATMEGA32U4) := -mmcu=atmega32u4 -D__ATmega32u4__
mcu-$(CONFIG_MACH_ATTINY88)   := -mmcu=attiny88 -D__ATTiny88__
mcu-$(CONFIG_MACH_ATTINY817)  := -mmcu=avrxmega3 -D__ATTiny817__
mcu-$(CONFIG_MACH_ATTINY1607) := -mmcu=avrxmega3 -D__ATTiny1607__
mcu-$(CONFIG_MACH_LGT8FX8P)   := -mmcu=atmega328p -D__ATmega328p__

# required
include-$(CONFIG_AVR) += -I$(arch-avr)/include \
			 -include $(arch-avr)/include/errno.h

include-$(CONFIG_MACH_ATMEGA2560) += -I$(arch-avr)/mach-atmega2560
include-$(CONFIG_MACH_ATMEGA328P) += -I$(arch-avr)/mach-atmega328p
include-$(CONFIG_MACH_ATMEGA32U4) += -I$(arch-avr)/mach-atmega32u4
include-$(CONFIG_MACH_ATTINY88)   += -I$(arch-avr)/mach-attiny88
include-$(CONFIG_MACH_ATTINY817)  += -I$(arch-avr)/mach-attiny817
include-$(CONFIG_MACH_ATTINY1607) += -I$(arch-avr)/mach-attiny1607
include-$(CONFIG_MACH_LGT8FX8P)   += -I$(arch-avr)/mach-lgt8fx8p

ccflags-$(CONFIG_AVR)    += $(include-y) -ffunction-sections -fdata-sections
ccflags-$(CONFIG_AVR)    += -fpack-struct -fshort-enums -Wno-address $(mcu-y)
ccflags-$(CONFIG_ATMEGA) += -flto -fno-fat-lto-objects
asflags-$(CONFIG_AVR)    += $(include-y) -x assembler-with-cpp $(mcu-y)
asflags-$(CONFIG_ATMEGA) += -flto
ldflags-$(CONFIG_AVR)    += -fuse-linker-plugin -Wl,--gc-sections $(mcu-y) -lm
ldflags-$(CONFIG_ATMEGA) += -flto
