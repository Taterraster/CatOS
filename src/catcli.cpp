#include "catcli.hpp"
#include "keyboard.hpp"
#include "vga.hpp"
#include <stdint.h>
#include <cstring>

static bool running = true;

// --- Print CLI prompt ---
static void print_prompt() {
    vga_print("cat> ");
}

// --- Handle CLI commands ---
static void handle_command(const char* cmd) {
    if (!strcmp(cmd, "help")) {
        vga_println("Commands: help, about, clear, version, kernelinfo");
    } else if (!strcmp(cmd, "version")|| !strcmp(cmd, "about")) {
        vga_println("catOS Alpha Pre v1.0.0 Beta");
    } else if (!strcmp(cmd, "clear")) {
        vga_clear();
    }else if (!strcmp(cmd, "")) {

    }else if (!strcmp(cmd, "kernelinfo")) {
        vga_println("catOS Kernel made by Taterr");
    }
    else {
        vga_print("Unknown command: ");
        vga_println(cmd);
    }
}

// --- Main CLI loop ---
extern "C" void catcli_start() {
    running = true;
    vga_clear();
    vga_println("Welcome to CatCLI!");
    vga_println("Type 'help' for a list of commands.");
    print_prompt();

    char buffer[128];
    int index = 0;

    while (running) {
        char c = keyboard_getchar(); // Blocking

        if (c == '\n') {  // Enter
            buffer[index] = '\0';
            vga_println("");
            handle_command(buffer);
            index = 0;
            print_prompt();
        } else if (c == '\b') {  // Backspace
            if (index > 0) {
                index--;
                vga_backspace();
            }
        } else if (c >= 32 && c <= 126) {  // Printable characters
            if (index < 127) {
                buffer[index++] = c;
                vga_putc(c);
            }
        }
    }
}
