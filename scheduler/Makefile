# scheduler makefile
scheduler := $(srctree)/scheduler

c_src-$(CONFIG_SINGLE_CORE) += $(scheduler)/picoRTOS.c $(scheduler)/picoRTOS_u.c
c_src-$(CONFIG_SMP) += $(scheduler)/picoRTOS-SMP.c $(scheduler)/picoRTOS_u.c
c_src-$(CONFIG_LITE) += $(scheduler)/picoRTOS-lite.c
