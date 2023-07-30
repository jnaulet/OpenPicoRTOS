/*
 * STM32H750 memory mapping for ROM (XIP)
 */
MEMORY
{
    flash1_2(rx)        : ORIGIN = 0x08000000, LENGTH = 128k
    sysmem1(rx)         : ORIGIN = 0x1ff00000, LENGTH = 128k
    sysmem2(rx)         : ORIGIN = 0x1ff40000, LENGTH = 128k
    sram0(rxw)          : ORIGIN = 0x24000000, LENGTH = 512k
    sram1_3(rxw)        : ORIGIN = 0x30000000, LENGTH = 384k
    sram4(rxw)          : ORIGIN = 0x38000000, LENGTH = 64k
    bsram(rxw)          : ORIGIN = 0x38800000, LENGTH = 4k
}

PROVIDE(__relocate_text = 0x0);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash1_2);
REGION_ALIAS("RAM", sram0);

/* common sections definition */
INCLUDE sections_flash_xip.ld
/* user-defined sections definitions */
SECTIONS
{
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        __stack_start__ = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        __stack_end__ = .;
    } > sram0

    __StackTop = __stack_end__;
    __StackBottom = __stack_start__;
}
