#
# PowerPC platforms
#
arch-powerpc := $(srctree)/arch/powerpc

c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_port.c
a_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_portasm.S
c_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS_port.c
a_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS_portasm.S

ifeq ($(CONFIG_SMP),y)
c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS-SMP_port.c
a_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS-SMP_portasm.S
c_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS-SMP_port.c
a_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS-SMP_portasm.S
endif

# machines
a_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/startup.S
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/intc-siul2.c
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/timer-stm.c
a_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/startup.S
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/intc-siu.c

# mcpu
cpu-$(CONFIG_E200Z4) := -mcpu=e200z4
cpu-$(CONFIG_E200Z7) := -mcpu=e200z7
# mfloat (for now)
fpu-$(CONFIG_POWERPC) := -mhard-float

ifeq ($(CONFIG_SMP),y)
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/core-mc_me.c
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/spinlock-sema42.c
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/core-siu.c
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/spinlock-sema4.c
endif

ccflags-$(CONFIG_POWERPC) += -fmessage-length=0 -msdata=eabi -mlra -funsigned-bitfields
ccflags-$(CONFIG_POWERPC) += -ffunction-sections -fdata-sections -fno-common $(cpu-y)
ccflags-$(CONFIG_POWERPC) += -mbig -mvle -mregnames $(fpu-y) -std=c99
asflags-$(CONFIG_POWERPC) += -x assembler-with-cpp $(cpu-y) $(fpu-y)
asflags-$(CONFIG_POWERPC) += -mbig -mvle -mregnames
ldflags-$(CONFIG_POWERPC) += -Xlinker --gc-sections $(cpu-y) $(fpu-y)
ldflags-$(CONFIG_POWERPC) += -nostdlib -nostartfiles -nodefaultlibs
