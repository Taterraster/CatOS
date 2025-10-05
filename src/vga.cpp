#include <stdint.h>
#include <cstddef>  
#include "vga.hpp"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// VGA memory buffer
static uint16_t* const vga_buffer = (uint16_t*)VGA_MEMORY;

// Current cursor position
static uint8_t vga_row = 0;
static uint8_t vga_col = 0;

// Default color: white on black
static uint8_t vga_color = 0x0F;

// Low-level port I/O for the cursor
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Update hardware cursor to match vga_row/col
static void update_cursor() {
    uint16_t pos = vga_row * VGA_WIDTH + vga_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Clear the screen and reset cursor
extern "C" void vga_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        }
    }
    vga_row = 0;
    vga_col = 0;
    update_cursor();
}

// Output a single character to the screen
extern "C" void vga_putc(char c) {
    if (c == '\n') {
        vga_row++;
        vga_col = 0;
    } else if (c == '\b') {
        if (vga_col > 0) {
            vga_col--;
            vga_buffer[vga_row * VGA_WIDTH + vga_col] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        }
    } else {
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = (uint16_t)c | ((uint16_t)vga_color << 8);
        vga_col++;
        if (vga_col >= VGA_WIDTH) {
            vga_col = 0;
            vga_row++;
        }
    }

    // Scroll if needed
    if (vga_row >= VGA_HEIGHT) {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
            }
        }
        // Clear last line
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        }
        vga_row = VGA_HEIGHT - 1;
    }

    update_cursor();
}

// Print a string (no newline)
extern "C" void vga_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i]);
    }
}

// Print a string + newline
extern "C" void vga_println(const char* str) {
    vga_print(str);
    vga_putc('\n');
}

// Move cursor back one space (for backspace)
extern "C" void vga_backspace() {
    if (vga_col > 0) {
        vga_col--;
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        update_cursor();
    }
}
