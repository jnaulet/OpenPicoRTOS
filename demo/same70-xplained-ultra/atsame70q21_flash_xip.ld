/*
 * ATSAME70Q21 memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00400000, LENGTH = 2048k
    ram(rwx)     : ORIGIN = 0x20400000, LENGTH = 384k
    sram(rwx)    : ORIGIN = 0x40074000, LENGTH = 1k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
