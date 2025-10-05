#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_init();
char keyboard_getchar(); // Returns next character, blocking
void keyboard_read_line(char* buffer, int max);
char keyboard_read_char();
#ifdef __cplusplus
}
#endif