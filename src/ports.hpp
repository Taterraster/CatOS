#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#ifdef __cplusplus
}
#endif
