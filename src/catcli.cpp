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
        vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        vga_println("Commands:\n help,\n about,\n clear,\n version,\n kernelinfo");
        vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    } else if (!strcmp(cmd, "version")|| !strcmp(cmd, "about")) {
        vga_println("CatOS Alpha Pre v1.0.0 Gamma");
    } else if (!strcmp(cmd, "clear")) {
        vga_clear();
    }else if (!strcmp(cmd, "")) {

    }else if (!strcmp(cmd, "kernelinfo")) {
        vga_println("CatOS Kernel made by Taterr");
    }
    else {
        vga_print("Unknown command: ");
        vga_println(cmd);
    }
}

// --- Main CLI loop ---
#define HISTORY_SIZE 5

static char history[HISTORY_SIZE][128]; 
static int history_count = 0;           
static int history_index = 0;           
extern "C" void catcli_start() {
    running = true;
    vga_clear();
    vga_set_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BLUE);
    vga_println("Welcome to CatCLI!");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    print_prompt();

    char buffer[128];
    int index = 0;

    while (running) {
    char c = keyboard_getchar();

    if (c == '\x01') { // Up arrow
        if (history_count == 0) continue;

        // Move history index up
        if (history_index > 0) history_index--;
        else history_index = 0;

        // Clear current input
        while (index > 0) {
            vga_backspace();
            index--;
        }

        // Copy history to buffer and print
        strcpy(buffer, history[history_index]);
        index = strlen(buffer);
        vga_print(buffer);
        continue;
    }

    if (c == '\x02') { // Down arrow
        if (history_count == 0) continue;

        // Move history index down
        if (history_index < history_count - 1) history_index++;
        else {
            // Clear input if past last history
            index = 0;
            buffer[0] = '\0';
            vga_print(""); // optional: may clear line visually
            continue;
        }

        // Clear current input
        while (index > 0) {
            vga_backspace();
            index--;
        }

        // Copy history to buffer and print
        strcpy(buffer, history[history_index]);
        index = strlen(buffer);
        vga_print(buffer);
        continue;
    }

    if (c == '\n') { // Enter
        buffer[index] = '\0';
        vga_println("");
        handle_command(buffer);

        // Save to history
        if (history_count < HISTORY_SIZE) {
            strcpy(history[history_count], buffer);
            history_count++;
        } else {
            // Shift history up if full
            for (int i = 1; i < HISTORY_SIZE; i++) {
                strcpy(history[i - 1], history[i]);
            }
            strcpy(history[HISTORY_SIZE - 1], buffer);
        }
        history_index = history_count; // reset browsing index

        index = 0;
        print_prompt();
    } else if (c == '\b') { // Backspace
        if (index > 0) {
            index--;
            vga_backspace();
        }
    } else if (c >= 32 && c <= 126) { // Printable characters
        if (index < 127) {
            buffer[index++] = c;
            vga_putc(c);
        }
    }
}
}

