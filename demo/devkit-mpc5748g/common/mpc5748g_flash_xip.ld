/*
 * MPC5748G memory mapping for ROM
 */

MEMORY
{
    flash(rx) : ORIGIN = 0x01000000, LENGTH = 5568k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 768k
    boot(r)   : ORIGIN = 0x00fa0000, LENGTH = 24
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
/* REGION_ALIAS("REGION_TEXT", ram); */

/* ECC RAM */
PROVIDE(__ram_start__ = ORIGIN(ram));
PROVIDE(__ram_end__ = ORIGIN(ram) + LENGTH(ram) - 1);

SECTIONS
{
	.boot :
	{
		__boot_start__ = .;
		KEEP(*(.boot))
		__boot_end__ = .;
	} > boot
}
