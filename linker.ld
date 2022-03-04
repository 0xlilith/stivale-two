/*
 * Kernel linker script
 * author: 0xlilith
 * date:   4-3-2022
*/

/* Tell linker that we want x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* symbol _start to be our entry point */
ENTRY(_start)

/* Define the program header we want */
/* MMU permission */
PHDRS
{
    /* 
        segment type:PR_LOAD  value:1 
        0 - execute
        1 - writ
        2 - read
    */
    null    PT_NULL FLAGS(0);                      /* NULL */
    text    PT_LOAD FLAGS((1 << 0) | (1 << 2));    /* Execute + Read */
    rodata  PT_LOAD FLAGS((1 << 2));               /* Read only */
    data    PT_LOAD FLAGS((1 << 1) | (1 << 2));    /* Write + Read */
}

SECTIONS
{
    /* We wanna be placed in the topmost 2GiB of the address space, for optimisations */
    /* and because that is what the stivale2 spec mandates. */
    /* Any address in this region will do, but often 0xffffffff80000000 is chosen as */
    /* that is the beginning of the region. */
    . = 0xffffffff80000000;

    .text : {
        *(.text .text.*)
    } :text

    /* Move to next memory page for .rodata */
    . += CONSTANT(MAXPAGESIZE);

    /* We place the .stivale2hdr section containing the header in its own section, */
    /* and we use the KEEP directive on it to make sure it doesn't get discarded. */
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    } :rodata

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    /* Move to next mem page for .data */
    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data
}