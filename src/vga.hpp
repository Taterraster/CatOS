
#pragma once


#include <cstdint>


namespace catos {


class VGA {
public:
static void clear();
static void putchar(char c);
static void write(const char* s);
};


} 