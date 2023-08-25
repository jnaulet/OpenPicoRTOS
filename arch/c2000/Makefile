#
# TI C2000 platforms
#
arch-c2000 := $(srctree)/arch/c2000

c_src-$(CONFIG_C28X) += $(arch-c2000)/c28x/picoRTOS_port.c
a_src-$(CONFIG_C28X) += $(arch-c2000)/c28x/picoRTOS_portasm.asm

a_src-$(CONFIG_MACH_F28379X) += $(arch-c2000)/c28x/mach-f28379x/startup.asm

# opt
abi-$(CONFIG_C28X_EABI) := --abi=eabi
float_support-$(CONFIG_C28X_FPU) := --float_support=fpu64

rts-$(CONFIG_C28X) := -lrts2800_ml.lib
rts-$(CONFIG_C28X_EABI) := -lrts2800_ml_eabi.lib
rts-$(CONFIG_C28X_FPU) := -lrts2800_fpu64_eabi.lib
