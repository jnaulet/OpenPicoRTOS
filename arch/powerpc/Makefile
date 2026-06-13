#
# PowerPC platforms
#
arch-powerpc := $(srctree)/arch/powerpc

c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_port.c
c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_port_u.c
a_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_portasm.S
a_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS_portasm_u.S

c_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS_port.c
a_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS_portasm.S

ifeq ($(CONFIG_SMP),y)
c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS-SMP_port.c
c_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS-SMP_port_u.c
a_src-$(CONFIG_E200Z4) += $(arch-powerpc)/e200z4/picoRTOS-SMP_portasm.S

c_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS-SMP_port.c
a_src-$(CONFIG_E200Z7) += $(arch-powerpc)/e200z7/picoRTOS-SMP_portasm.S
endif

# machines
a_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/startup.S
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/intc-siul2.c
c_src-$(CONFIG_MACH_MPC574X) += $(arch-powerpc)/e200z4/mach-mpc574x/timer-stm.c
c_src-$(CONFIG_MPU_SMPU)     += $(arch-powerpc)/e200z4/mach-mpc574x/mpu-smpu.c

a_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/startup.S
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/intc-siu.c
ifeq ($(CONFIG_SMP),y)
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/mach-mpc577x/timer-stm.c
else
c_src-$(CONFIG_MACH_MPC577X) += $(arch-powerpc)/e200z7/timer-dec.c
endif

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

ccflags-$(CONFIG_POWERPC) += -save-temps
ccflags-$(CONFIG_POWERPC) += $(cpu-y) -mbig -mvle -mregnames -mlra $(fpu-y) -std=c99
asflags-$(CONFIG_POWERPC) += -x assembler-with-cpp $(cpu-y) $(fpu-y)
asflags-$(CONFIG_POWERPC) += -mbig -mvle -mregnames
ldflags-$(CONFIG_POWERPC) += $(cpu-y) $(fpu-y)
ldflags-$(CONFIG_POWERPC) += -nostdlib -nostartfiles -nodefaultlibs
ldflags-$(CONFIG_POWERPC) += -L$(arch-powerpc)/include -Tsections.ld
