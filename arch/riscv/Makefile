#
# RISC-V
#
arch-riscv := $(srctree)/arch/riscv

c_src-$(CONFIG_RV32IMAC) += $(arch-riscv)/rv32imac/picoRTOS_port.c
a_src-$(CONFIG_RV32IMAC) += $(arch-riscv)/rv32imac/picoRTOS_portasm.S
c_src-$(CONFIG_RV32EC)   += $(arch-riscv)/rv32ec/picoRTOS_port.c
a_src-$(CONFIG_RV32EC)   += $(arch-riscv)/rv32ec/picoRTOS_portasm.S

# march
arch-$(CONFIG_RV32IMAC) := -march=rv32imac
abi-$(CONFIG_RV32IMAC) := -mabi=ilp32
cmodel-$(CONFIG_RV32IMAC) := -mcmodel=medlow

arch-$(CONFIG_RV32EC) := -march=rv32ec
abi-$(CONFIG_RV32EC) := -mabi=ilp32e
cmodel-$(CONFIG_RV32EC) := -mcmodel=medlow

# required for riscv64-unknown-elf-
include-$(CONFIG_RISCV) += -I$(arch-riscv)/include \
			   -include $(arch-riscv)/include/errno.h

# machines
a_src-$(CONFIG_MACH_GD32VF103) += $(arch-riscv)/rv32imac/mach-gd32vf103/startup.S
c_src-$(CONFIG_MACH_GD32VF103) += $(arch-riscv)/rv32imac/mach-gd32vf103/intc.c
a_src-$(CONFIG_MACH_CH32V003) += $(arch-riscv)/rv32ec/mach-ch32v003rm/startup.S
c_src-$(CONFIG_MACH_CH32V003) += $(arch-riscv)/rv32ec/mach-ch32v003rm/intc-pfic.c
c_src-$(CONFIG_MACH_CH32V003) += $(arch-riscv)/rv32ec/mach-ch32v003rm/timer-stk.c
# FIXME: libgcc is supposed to implement this
a_src-$(CONFIG_RV32EC_LIBGCC_DIV_MULT) += $(arch-riscv)/rv32ec/mach-ch32v003rm/gcc/div.S
a_src-$(CONFIG_RV32EC_LIBGCC_DIV_MULT) += $(arch-riscv)/rv32ec/mach-ch32v003rm/gcc/muldi3.S

ccflags-$(CONFIG_RISCV) += $(include-y) $(arch-y) $(abi-y) $(cmodel-y) -ffreestanding
asflags-$(CONFIG_RISCV) += $(include-y) $(arch-y) $(abi-y) $(cmodel-y)

ldflags-$(CONFIG_RISCV) += $(arch-y) $(abi-y) $(cmodel-y)
ldflags-$(CONFIG_RISCV) += -nostdlib -nostartfiles
