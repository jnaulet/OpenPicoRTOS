/*
 * STM32H743 memory mapping for ROM (XIP)
 */

MEMORY
{
    flash1_2(rx)        : ORIGIN = 0x08000000, LENGTH = 2048k
    sysmem1(rx)         : ORIGIN = 0x1ff00000, LENGTH = 128k
    sysmem2(rx)         : ORIGIN = 0x1ff40000, LENGTH = 128k
    sram0(rxw)          : ORIGIN = 0x24000000, LENGTH = 512k
    sram1_3(rxw)        : ORIGIN = 0x30000000, LENGTH = 384k
    sram4(rxw)          : ORIGIN = 0x38000000, LENGTH = 64k
    bsram(rxw)          : ORIGIN = 0x38800000, LENGTH = 4k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash1_2);
