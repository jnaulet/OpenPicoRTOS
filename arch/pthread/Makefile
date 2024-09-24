#
# PTHREAD simulation makefile
#
arch-pthread := $(srctree)/arch/pthread

c_src-$(CONFIG_PTHREAD_LINUX) += $(arch-pthread)/linux/picoRTOS_port.c

# static analysis improvements
cppcheckflags-$(CONFIG_PTHREAD_LINUX) += --platform=native
