/*
 * MPC5748G memory mapping for RAM
 */

MEMORY
{
    boot(rx)  : ORIGIN = 0x00fa0000, LENGTH = 24
    flash(rx) : ORIGIN = 0x01000000, LENGTH = 5568k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 768k
}

PROVIDE(__relocate_text = 0x1);
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x100;

ENTRY(Reset_Handler)
REGION_ALIAS("FLASH", flash);
REGION_ALIAS("RAM", ram);
PROVIDE(__interrupt_vector_count = 768);

/* ECC RAM */
PROVIDE(__ram_start__ = 0x40000000);
PROVIDE(__ram_end__ = 0x400bfffc);

/* custom sections first */
SECTIONS
{
    .boot :
    {
        __boot_start__ = .;
        KEEP (*(.boot))
        __boot_end__ = .;
    } > boot
}

/* common sections definition */
INCLUDE ppc-eabivle_ram_sections.ld
