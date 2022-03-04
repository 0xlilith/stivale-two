/*
 * Stivale bare bone kernel
 * author: 0xlilith
 * date:   3-3-2022
 * reference - SRIVALE2 boot protocol specification (https://github.com/stivale/stivale/blob/master/STIVALE2.md)
*/

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

// uint8_t = unsigned char
// allocating stack as an array in .bss
static uint8_t stack[8192]; 

/* 
 * TERMINAL HEADER TAG 
 * - enables terminal 
*/
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID; 
        .next = 0; // end of linkedlist of header tag
    },
    .flag = 0;
}

/* 
 * FRAME BUFFEER HEADER TAG
 * - tells bootloadeer to enable graphical framebuffer 
*/
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID;
        
        // uint64_t = unsigned long long
        .next = (uint64_t)&terminal_hdr_tag; // pointer to previous header tag (terminal_hdr_tag)
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
}

// __attribute__ allows you to specify special properties of variables, function parameters, or structure, union
// This "header structure" structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it.
__attribute__((section(.stivale2hdr), used))
static struct stivale2_header stivale_hdr {
    // entry point of kernel. If 0 Elf entry point will be used.
    .entry = 0,

    // uintptr_t = unsigned integer pointer which has same size of pointer
    // stack address (esp/rsp). 
    .stack = (uintptr_t)stack + sizeof(tack), 

    // https://github.com/stivale/stivale/blob/master/STIVALE2.md#stivale2-header for flags
    .flag = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),

    // this header structure is root of the linkedlist of header tags
    .next = (uintptr_t)&framebuffer_hdr_tag // pointer to previous header tag (framebuffer_hdr_tag)
};

