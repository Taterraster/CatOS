
// Minimal freestanding string helpers + trivial heap for new/delete
// Put this in src/string_ext.cpp

#include <stddef.h>
#include <stdint.h>
extern "C" {
// string functions used by the kernel
size_t strlen(const char* s) {
    size_t n = 0;
    while (s && s[n]) ++n;
    return n;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++)) {}
    return dest;
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) { ++a; ++b; }
    return (unsigned char)*a - (unsigned char)*b;
}

char* strcat(char* dest, const char* src) {
    size_t dlen = strlen(dest);
    char* d = dest + dlen;
    while ((*d++ = *src++)) {}
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; ++i) dest[i] = src[i];
    for (; i < n; ++i) dest[i] = '\0';
    return dest;
}

// Simple strtok (non-reentrant) - safe for single-threaded CLI use
char* strtok(char* s, const char* delim) {
    static char* save = nullptr;
    if (s) save = s;
    if (!save) return nullptr;

    // skip leading delimiters
    char* start = save;
    while (*start) {
        bool isdel = false;
        for (const char* d = delim; *d; ++d) if (*start == *d) { isdel = true; break; }
        if (!isdel) break;
        ++start;
    }
    if (!*start) { save = nullptr; return nullptr; }

    // find end
    char* p = start;
    while (*p) {
        bool isdel = false;
        for (const char* d = delim; *d; ++d) if (*p == *d) { isdel = true; break; }
        if (isdel) break;
        ++p;
    }

    if (*p) {
        *p = '\0';
        save = p + 1;
    } else {
        save = nullptr;
    }
    return start;
}
} // extern "C"

// ------------------------------
// Tiny bump allocator for operator new/delete
// ------------------------------
static uint8_t _kernel_heap[64 * 1024]; // 64KB heap for small objects
static size_t  _kernel_heap_ptr = 0;

extern "C++" void* operator new(size_t size) noexcept {
    // Align to 8
    size_t align = 8;
    size_t p = (_kernel_heap_ptr + (align - 1)) & ~(align - 1);
    if (p + size > sizeof(_kernel_heap)) {
        // out of memory — return nullptr (nothrow new)
        return nullptr;
    }
    void* mem = &_kernel_heap[p];
    _kernel_heap_ptr = p + size;
    return mem;
}

extern "C++" void operator delete(void* ptr) noexcept {
    // no-op in this tiny bump allocator
    (void)ptr;
}

// sized delete (C++14)
extern "C++" void operator delete(void* ptr, size_t) noexcept {
    (void)ptr;
}
