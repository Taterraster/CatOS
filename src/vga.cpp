
#include "vga.hpp"


namespace catos {


static constexpr uint16_t VGA_WIDTH = 80;
static constexpr uint16_t VGA_HEIGHT = 25;
static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;


static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;


static inline uint16_t make_entry(char c, uint8_t attr) {
return (uint16_t)c | (uint16_t)attr << 8;
}


void VGA::clear() {
const uint16_t blank = make_entry(' ', 0x07);
for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
VGA_BUFFER[i] = blank;
}
cursor_x = cursor_y = 0;
}


void VGA::putchar(char c) {
if (c == '\n') {
cursor_x = 0;
cursor_y++;
} else if (c == '\r') {
cursor_x = 0;
} else {
const uint16_t idx = cursor_y * VGA_WIDTH + cursor_x;
VGA_BUFFER[idx] = make_entry(c, 0x07);
cursor_x++;
if (cursor_x >= VGA_WIDTH) {
cursor_x = 0;
cursor_y++;
}
}
if (cursor_y >= VGA_HEIGHT) {

for (uint32_t row = 1; row < VGA_HEIGHT; ++row) {
for (uint32_t col = 0; col < VGA_WIDTH; ++col) {
VGA_BUFFER[(row - 1) * VGA_WIDTH + col] = VGA_BUFFER[row * VGA_WIDTH + col];
}
}

const uint16_t blank = make_entry(' ', 0x07);
for (uint32_t col = 0; col < VGA_WIDTH; ++col) {
VGA_BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
}
cursor_y = VGA_HEIGHT - 1;
}
}


void VGA::write(const char* s) {
for (const char* p = s; *p; ++p) putchar(*p);
}


} // namespace catos