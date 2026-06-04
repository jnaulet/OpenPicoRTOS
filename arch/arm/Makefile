#
# ARM platforms
#
arch-arm := $(srctree)/arch/arm

c_src-$(CONFIG_ARMV6M) += $(arch-arm)/armv6-m/picoRTOS_port.c
c_src-$(CONFIG_ARMV6M) += $(arch-arm)/armv6-m/picoRTOS_port_u.c
a_src-$(CONFIG_ARMV6M) += $(arch-arm)/armv6-m/picoRTOS_portasm.S
a_src-$(CONFIG_ARMV6M) += $(arch-arm)/armv6-m/picoRTOS_portasm_u.S

c_src-$(CONFIG_MPU_PMSAV6) += $(arch-arm)/armv6-m/mpu-pmsa.c

ifeq ($(CONFIG_SMP),y)
c_src-$(CONFIG_MACH_RP2040) += $(arch-arm)/armv6-m/mach-rp2040/picoRTOS-SMP_port.c
c_src-$(CONFIG_MACH_RP2040) += $(arch-arm)/armv6-m/mach-rp2040/picoRTOS-SMP_port_u.c
a_src-$(CONFIG_MACH_RP2040) += $(arch-arm)/armv6-m/mach-rp2040/picoRTOS-SMP_portasm.S
endif

c_src-armv7x-m += $(arch-arm)/armv7e-m/picoRTOS_port.c
c_src-armv7x-m += $(arch-arm)/armv7e-m/picoRTOS_port_u.c
a_src-armv7x-m += $(arch-arm)/armv7e-m/picoRTOS_portasm.S
a_src-armv7x-m += $(arch-arm)/armv7e-m/picoRTOS_portasm_u.S

c_src-$(CONFIG_ARMV7M) += $(c_src-armv7x-m)
a_src-$(CONFIG_ARMV7M) += $(a_src-armv7x-m)

ifeq ($(CONFIG_MACH_ATSAME5X),y)
armv7em-cache-c_src-$(CONFIG_CACHE_MAINTENANCE) += $(arch-arm)/armv7e-m/cache-cmcc.c
else
armv7em-cache-c_src-$(CONFIG_CACHE_MAINTENANCE) += $(arch-arm)/armv7e-m/cache-mvsa.c
endif

c_src-$(CONFIG_ARMV7EM) += $(c_src-armv7x-m)
a_src-$(CONFIG_ARMV7EM) += $(a_src-armv7x-m)
c_src-$(CONFIG_ARMV7EM) += $(armv7em-cache-c_src-y)

c_src-$(CONFIG_MPU_PMSAV7) += $(arch-arm)/armv7e-m/mpu-pmsa.c

# march
arch-$(CONFIG_ARMV6M) := -march=armv6-m -mfloat-abi=soft
arch-$(CONFIG_ARMV7M) := -march=armv7-m -mfloat-abi=soft
arch-$(CONFIG_ARMV7EM) := -march=armv7e-m
# mfpu
fpu-$(CONFIG_FPV4_SP_D16) := -mfpu=fpv4-sp-d16
fpu-$(CONFIG_FPV5_SP_D16) := -mfpu=fpv5-sp-d16

# machines
a_src-$(CONFIG_MACH_RP2040) += $(arch-arm)/armv6-m/mach-rp2040/startup.S
a_src-$(CONFIG_MACH_ATSAM3X8E) += $(arch-arm)/armv7-m/mach-atsam3x8e/startup.S
a_src-$(CONFIG_MACH_STM32F10XXX) += $(arch-arm)/armv7-m/mach-stm32f10xxx/startup.S
a_src-$(CONFIG_MACH_ATSAME5X) += $(arch-arm)/armv7e-m/mach-atsame5x/startup.S
a_src-$(CONFIG_MACH_STM32H7XX) += $(arch-arm)/armv7e-m/mach-stm32h7xx/startup.S
a_src-$(CONFIG_MACH_RENESAS_RA4) += $(arch-arm)/armv7e-m/mach-renesas_ra4/startup.S
a_src-$(CONFIG_MACH_STM32F401X) += $(arch-arm)/armv7e-m/mach-stm32f401x/startup.S
a_src-$(CONFIG_MACH_STM32L4X) += $(arch-arm)/armv7e-m/mach-stm32l4x/startup.S
a_src-$(CONFIG_MACH_ATSAMX7X) += $(arch-arm)/armv7e-m/mach-atsamx7x/startup.S

ccflags-$(CONFIG_ARM) += -mthumb $(arch-y) $(fpu-y)
asflags-$(CONFIG_ARM) += -mthumb $(arch-y) $(fpu-y)
ldflags-$(CONFIG_ARM) += $(arch-y) $(fpu-y) --specs=nosys.specs
ldflags-$(CONFIG_ARM) += -L$(arch-arm)/include -Tsections.ld

# newlib-4.4.0
a_src-$(CONFIG_STUB_NEWLIB_4_4_0) += $(arch-arm)/stub-newlib-4.4.0.S
