/*
 * SAMD51G19A memory mapping for RAM (Arduino bootloader)
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00004000, LENGTH = 496k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 192k
}

REGION_ALIAS("REGION_TEXT", ram);
