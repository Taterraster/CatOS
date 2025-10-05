#include <stdint.h>
#include <cstddef>  
#include "vga.hpp"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// VGA memory buffer
static uint16_t* const vga_buffer = (uint16_t*)VGA_MEMORY;
static size_t cursor_row = 0;
static size_t cursor_col = 0;
static uint8_t vga_color = (VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));

static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_color = fg | (bg << 4);
}
// Current cursor position
static uint8_t vga_row = 0;
static uint8_t vga_col = 0;

// Default color: white on black

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
void vga_putchar(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(' ', vga_color);
        }
    } else {
        vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = vga_entry(c, vga_color);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT)
        cursor_row = 0; // Simple wraparound; you can add scrolling later
}

// Print a string (no newline)
extern "C" void vga_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

// Print a string + newline
extern "C" void vga_println(const char* str) {
    vga_print(str);
    vga_putchar('\n');
}

// Move cursor back one space (for backspace)
extern "C" void vga_backspace() {
    if (vga_col > 0) {
        vga_col--;
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        update_cursor();
    }
}
