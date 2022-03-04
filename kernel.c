/*
 * Stivale bare bone kernel
 * author: 0xlilith
 * date:   3-3-2022
 * reference - SRIVALE2 boot protocol specification (https://github.com/stivale/stivale/blob/master/STIVALE2.md)
*/

#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>

typedef struct stivale2_struct STIVALE2_STRUCT;

// uint8_t = unsigned char
// allocating stack as an array in .bss
static uint8_t stack[8192]; 

/* 
 * TERMINAL HEADER TAG 
 * - enables terminal 
*/
static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0, // end of linkedlist of header tag
    },
    .flags = 0
};

/* 
 * FRAME BUFFEER HEADER TAG
 * - tells bootloadeer to enable graphical framebuffer 
*/
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        
        // uint64_t = unsigned long long
        .next = (uint64_t)&terminal_hdr_tag // pointer to previous header tag (terminal_hdr_tag)
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

// __attribute__ allows you to specify special properties of variables, function parameters, or structure, union
// This "header structure" structure needs to reside in the .stivale2hdr ELF section in order
// for the bootloader to find it.
__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    // entry point of kernel. If 0 - ELF entry point will be used.
    .entry_point = 0,

    // uintptr_t = unsigned integer pointer which has same size of pointer
    // stack address (esp/rsp). 
    .stack = (uintptr_t)stack + sizeof(stack), 

    // https://github.com/stivale/stivale/blob/master/STIVALE2.md#stivale2-header for flags
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),

    // this header structure is root of the linkedlist of header tags
    .tags = (uintptr_t)&framebuffer_hdr_tag // pointer to first header in linked list (framebuffer_hdr_tag)
};

// Helper function than will help scan for tags
// that we want from the bootloader (structure tags)
// https://github.com/stivale/stivale/blob/master/STIVALE2.md#stivale2-structure
void *stivale2_get_tag(STIVALE2_STRUCT *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for(;;) {
        // if tag pointer is NULL (end of linkedlist)
        // return NULL
        if (current_tag == NULL) {
            return NULL;
        }

        // check if identifier matches to id
        // return pointer to the matching tag
        if (current_tag->identifier == id) {
            return current_tag;
        }

        // get a pointer to the next linked list and repeat
        current_tag = (void *)current_tag->next;
    }
}

// Kernel entry point
void _start(STIVALE2_STRUCT *stivale2_struct) {
    // get terminal structure tag from bootloader
    struct stivale2_struct_tag_terminal *term_str_tag;
    term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    // check if tag was found
    if (term_str_tag == NULL) {
        for(;;) {
            asm("hlt");
        }
    }

    // get address of terminal write function
    void *term_write_ptr = (void *)term_str_tag->term_write;
    
    // size_t = unsigned type used to represent size of object.
    void (*term_write)(const char *string, size_t length) = term_write_ptr;

    term_write("Lilith is God", 13);

    for(;;) {
        asm("hlt");
    }
}