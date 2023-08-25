# scheduler makefile
scheduler := $(srctree)/scheduler

c_src-$(CONFIG_SINGLE_CORE) += $(scheduler)/picoRTOS.c
c_src-$(CONFIG_SMP) += $(scheduler)/picoRTOS-SMP.c
