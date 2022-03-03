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
        .idntifier = STIVALE2_HEADER_TAG_TERMINAL_ID; 
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
        .idntifier = STIVALE2_HEADER_TAG_TERMINAL_ID;
        // uint64_t = unsigned long long
        .next = (uint64_t)&terminal_hdr_tag; // pointer to previous header tag
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
}