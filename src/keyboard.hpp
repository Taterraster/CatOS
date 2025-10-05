#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_init();
char keyboard_getchar(); // Returns next character, blocking

#ifdef __cplusplus
}
#endif