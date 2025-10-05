#include <stddef.h>

extern "C" int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}
#include "string.hpp"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

char* strcpy(char* dest, const char* src) {
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}
