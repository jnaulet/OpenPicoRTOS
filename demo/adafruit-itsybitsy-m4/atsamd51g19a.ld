/*
 * SAMD51G19A memory mapping for ROM (Arduino bootloader)
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00004000, LENGTH = 496k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 192k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
