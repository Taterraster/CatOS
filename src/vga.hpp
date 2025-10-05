#pragma once

extern "C" void vga_putc(char c);
extern "C" void vga_print(const char* str);
extern "C" void vga_println(const char* str);
extern "C" void vga_clear();
extern "C" void vga_backspace();