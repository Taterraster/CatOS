#include "keyboard.hpp"
#include "ports.hpp"
#include "vga.hpp"
#include <stdint.h>

static bool shift_pressed = false;

static const char keymap[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

// Shifted characters map
static const char keymap_shift[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0, '*', 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

// Read scancode from keyboard
static uint8_t keyboard_read_scancode() {
    while (!(inb(0x64) & 1));
    return inb(0x60);
}

// Initialize keyboard (currently nothing to init)
void keyboard_init() {
    // Future: enable interrupts if needed
}

// Return next character (blocking)
char keyboard_getchar() {
       static bool shift_pressed = false;
    static bool extended = false; 

    uint8_t scancode = 0;  
    while (1) {
    scancode = keyboard_read_scancode();

    if (scancode == 0xE0) { // start of extended key
        extended = true;
        continue;
    }

    // Shift pressed/released
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = true; continue;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = false; continue;
    }

    // Arrow keys (extended)
    if (extended) {
        extended = false;
        if (scancode == 0x48) return '\x01'; // Up arrow -> special code 1
        if (scancode == 0x50) return '\x02'; // Down arrow -> special code 2 (optional)
        continue;
    }

    if (scancode > 127) continue;

    if (shift_pressed)
        return keymap_shift[scancode];
    else
        return keymap[scancode];
}
}
static char last_char = 0;

char keyboard_read_char() {
    unsigned char scancode = inb(0x60);
    // Simplify: only handle basic ASCII for now
    if (scancode >= 0x02 && scancode <= 0x0D) return '0' + (scancode - 0x02);
    if (scancode == 0x1C) return '\n';
    return 0;
}

extern "C" void keyboard_read_line(char* buffer, int max) {
    if (!buffer || max <= 0) return;

    int i = 0;
    while (1) {
        char c = keyboard_getchar();
        if (!c) continue;

        if (c == '\n') {
            buffer[i] = '\0';
            vga_putchar('\n');
            return;
        } 
        else if (c == '\b') {
            if (i > 0) {
                --i;
                // Erase character visually
                vga_putchar('\b');
                vga_putchar(' ');
                vga_putchar('\b');
            }
        } 
        else if ((unsigned char)c >= 32 && (unsigned char)c <= 126) {
            if (i < max - 1) {
                buffer[i++] = c;
                vga_putchar(c);
            }
        }
    }
}