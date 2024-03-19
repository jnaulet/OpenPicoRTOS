#
# Staging uart drivers
#
staging-drivers-uart := $(srctree)/staging/drivers/uart

c_src-$(CONFIG_UART_N76E003) += $(staging-drivers-uart)/uart-n76e003.c
c_src-$(CONFIG_UART_8051) += $(staging-drivers-uart)/uart-8051$(CONFIG_UART_8051_IRQDRIVEN:y=_irqdriven).c
