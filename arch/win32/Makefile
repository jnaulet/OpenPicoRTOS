#
# WIN32 simulation makefile
#
arch-win32 := $(srctree)/arch/win32

c_src-$(CONFIG_WIN32) += $(arch-win32)/picoRTOS_port.c

# static analysis improvements
cppcheckflags-$(CONFIG_WIN32) += --platform=win32A
