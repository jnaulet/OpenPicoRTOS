# AVR platforms makefile
arch-avr := $(srctree)/arch/avr

c_src-$(CONFIG_ATMEGA) := $(arch-avr)/atmega/picoRTOS_port.c
a_src-$(CONFIG_ATMEGA) := $(arch-avr)/atmega/picoRTOS_portasm.S

c_src-$(CONFIG_ATTINY) := $(arch-avr)/attiny/picoRTOS_port.c
a_src-$(CONFIG_ATTINY) := $(arch-avr)/attiny/picoRTOS_portasm.S

# mcu
mcu-$(CONFIG_MACH_ATMEGA2560) := -mmcu=atmega2560 -D__ATmega2560__
mcu-$(CONFIG_MACH_ATMEGA328P) := -mmcu=atmega328p -D__ATmega328p__
mcu-$(CONFIG_MACH_ATMEGA32U4) := -mmcu=atmega32u4 -D__ATmega32u4__
mcu-$(CONFIG_MACH_ATTINY817)  := -mmcu=avrxmega3 -D__ATTiny817__
mcu-$(CONFIG_MACH_ATTINY1607) := -mmcu=avrxmega3 -D__ATTiny1607__

# required
include-$(CONFIG_AVR) += -I$(arch-avr)/include \
			 -include $(arch-avr)/include/errno.h

# include atmega (needed cause no file is compiled from these directories)
include-$(CONFIG_MACH_ATMEGA2560) += -I$(arch-avr)/atmega/mach-atmega2560
include-$(CONFIG_MACH_ATMEGA328P) += -I$(arch-avr)/atmega/mach-atmega328p
include-$(CONFIG_MACH_ATMEGA32U4) += -I$(arch-avr)/atmega/mach-atmega32u4

# include attiny (needed cause no file is compiled from these directories)
include-$(CONFIG_MACH_ATTINY817) += -I$(arch-avr)/attiny/mach-attiny817
include-$(CONFIG_MACH_ATTINY1607) += -I$(arch-avr)/attiny/mach-attiny1607

ccflags-$(CONFIG_AVR) += $(include-y) -ffunction-sections -fdata-sections \
			 -flto -fno-fat-lto-objects $(mcu-y)
asflags-$(CONFIG_AVR) += $(include-y) -x assembler-with-cpp -flto $(mcu-y)
ldflags-$(CONFIG_AVR) += -flto -fuse-linker-plugin -Wl,--gc-sections $(mcu-y) -lm
