#include "keyboard.hpp"
#include "vga.hpp"
#include <stdint.h>

// I/O port access
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// US QWERTY scancode set 1
static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
    'z','x','c','v','b','n','m',',','.','/',  0, '*',  0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// Initialize PS/2 keyboard (optional)
extern "C" void keyboard_init() {
    // Flush any pending data
    while (inb(0x64) & 1) inb(0x60);
}

// Wait for a key press and return it as ASCII
extern "C" char keyboard_getchar() {
    uint8_t scancode = 0;
    do {
        // Wait for a scancode
        while (!(inb(0x64) & 1));

        scancode = inb(0x60);

        // Ignore break codes (key release)
        if (scancode & 0x80) continue;

        if (scancode < 128)
            return scancode_table[scancode];

    } while (1);
}
