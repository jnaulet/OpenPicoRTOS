/*
 * MPC5748G memory mapping for RAM
 */

MEMORY
{
    flash(rx) : ORIGIN = 0x01000000, LENGTH = 5568k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 768k
    boot(r)   : ORIGIN = 0x00fa0000, LENGTH = 24
}

ENTRY(Reset_Handler)
/* REGION_ALIAS("REGION_TEXT", flash); */
REGION_ALIAS("REGION_TEXT", ram);

/* ECC RAM */
PROVIDE(__ram_start__ = 0x40000000);
PROVIDE(__ram_end__ = 0x400bfffc);

SECTIONS
{
	.boot :
	{
		__boot_start__ = .;
		KEEP(*(.boot))
		__boot_end__ = .;
	} > boot
}
