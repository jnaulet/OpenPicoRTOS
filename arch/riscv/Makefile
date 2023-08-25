#
# RISC-V
#
arch-riscv := $(srctree)/arch/riscv

c_src-$(CONFIG_RV32IMAC) += $(arch-riscv)/rv32imac/picoRTOS_port.c
a_src-$(CONFIG_RV32IMAC) += $(arch-riscv)/rv32imac/picoRTOS_portasm.S

# march
arch-$(CONFIG_RV32IMAC) := -march=rv32imac
abi-$(CONFIG_RV32IMAC) := -mabi=ilp32
cmodel-$(CONFIG_RV32IMAC) := -mcmodel=medlow

# required for riscv64-unknown-elf-
include-$(CONFIG_RISCV) += -I$(arch-riscv)/include \
			   -include $(arch-riscv)/include/errno.h

# machines
a_src-$(CONFIG_MACH_GD32VF103) += $(arch-riscv)/rv32imac/mach-gd32vf103/startup.S
c_src-$(CONFIG_MACH_GD32VF103) += $(arch-riscv)/rv32imac/mach-gd32vf103/intc.c

ccflags-$(CONFIG_RISCV) += $(include-y) $(arch-y) $(abi-y) $(cmodel-y) -ffreestanding
asflags-$(CONFIG_RISCV) += $(include-y) $(arch-y) $(abi-y) $(cmodel-y)

ldflags-$(CONFIG_RISCV) += $(arch-y) $(abi-y) $(cmodel-y)
ldflags-$(CONFIG_RISCV) += -nostdlib -nostartfiles
