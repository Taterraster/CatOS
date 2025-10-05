#pragma once
#include <stddef.h>  

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);

#ifdef __cplusplus
}
#endif