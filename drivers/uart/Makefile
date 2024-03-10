#
# UART drivers makefile
#
drivers-uart := $(srctree)/drivers/uart

c_src-$(CONFIG_UART_ARM_PL011) += $(drivers-uart)/uart-arm_pl011.c
c_src-$(CONFIG_UART_ATMEL_SERCOM) += $(drivers-uart)/uart-atmel_sercom.c
c_src-$(CONFIG_UART_AVR) += $(drivers-uart)/uart-avr$(CONFIG_UART_AVR_IRQDRIVEN:y=+irqdriven).c
c_src-$(CONFIG_UART_GD32VF103) += $(drivers-uart)/uart-gd32vf103.c
c_src-$(CONFIG_UART_PIC32MX) += $(drivers-uart)/uart-pic32mx.c
c_src-$(CONFIG_UART_ATSAM3X) += $(drivers-uart)/uart-sam3x.c
c_src-$(CONFIG_UART_STM32H7XX) += $(drivers-uart)/uart-stm32h7xx.c
c_src-$(CONFIG_UART_TINYAVR_USART) += $(drivers-uart)/uart-tinyavr_usart.c
c_src-$(CONFIG_UART_RENESAS_RA4_SCI) += $(drivers-uart)/uart-renesas_ra4_sci.c
c_src-$(CONFIG_UART_NXP_LINFLEXD) += $(drivers-uart)/uart-nxp_linflexd.c
