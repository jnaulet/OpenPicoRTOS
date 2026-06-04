/*
 * SAM3X8E memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00080000, LENGTH = 512k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 96k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
