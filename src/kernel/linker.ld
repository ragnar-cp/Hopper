ENTRY(_start)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x1000; /* Define the load address for the kernel */

    .text : ALIGN(4)
    {
        *(.text)
    }

    .data : ALIGN(4)
    {
        *(.data)
    }

    .rodata : ALIGN(4)
    {
        *(.rodata)
    }

    .bss : ALIGN(4)
    {
        *(.bss)
        *(COMMON)
    }
}
