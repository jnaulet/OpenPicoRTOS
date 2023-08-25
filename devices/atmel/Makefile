# Atmel devices makefile
obj-$(DEVICE_ATMEGA2560) := atmega2560/startup.o
obj-$(DEVICE_ATMEGA328P) := atmega328p/startup.o
obj-$(DEVICE_ATMEGA32U4) := atmega32u4/startup.o
obj-$(DEVICE_ATSAM3X8E) := atsam3x8e/startup.o
obj-$(DEVICE_ATSAMD51) := atsamd51/startup.o
obj-$(DEVICE_ATTINY1607) := attiny1607/startup.o
obj-$(DEVICE_ATTINY817) := attiny817/startup.o

ldflags-$(DEVICE_ATSAM3X8E) += -L../lib/arm/cortex-m/ldscripts
ldflags-$(DEVICE_ATSAMD51) += -L../lib/arm/cortex-m/ldscripts
