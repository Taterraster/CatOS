#include "catcli.hpp"
#include "vga.hpp"
#include "keyboard.hpp"
#include "string.hpp"
#include "fs.hpp"
#include <cstring>
void handle_command(const char* cmdline) {
    char buffer[128];
    strcpy(buffer, cmdline);
    char* cmd = strtok(buffer, " ");

    if (!cmd) return;

    if (strcmp(cmd, "help") == 0) {
        vga_println("Commands: help, clear, ls, mkdir, rmdir, cd, pwd, touch, rm, cat, echo, version");
    }
    else if (strcmp(cmd, "clear") == 0) {
        vga_clear();
    }
    else if (strcmp(cmd, "mkdir") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: mkdir <folder>"); return; }
        int res = fs_mkdir(name);
        if (res == 0) vga_println("Folder created.");
        else vga_println("Failed to create folder.");
    }
    else if (strcmp(cmd, "rmdir") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: rmdir <folder>"); return; }
        int res = fs_rmdir(name);
        if (res == 0) vga_println("Folder removed.");
        else if (res == -2) vga_println("Folder not empty!");
        else vga_println("Failed to remove folder.");
    }
    else if (strcmp(cmd, "cd") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: cd <folder>"); return; }
        int res = fs_cd(name);
        if (res == 0) {
            vga_print("Current dir: ");
            vga_println(fs_pwd());
        } else vga_println("Directory not found.");
    }
    else if (strcmp(cmd, "ls") == 0) {
        fs_ls();
    }
    else if (strcmp(cmd, "pwd") == 0) {
        vga_println(fs_pwd());
    }
    else if (strcmp(cmd, "touch") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: touch <file>"); return; }
        if (fs_touch(name) == 0) vga_println("File created.");
        else vga_println("Failed to create file.");
    }
    else if (strcmp(cmd, "rm") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: rm <file>"); return; }
        if (fs_rm(name) == 0) vga_println("File removed.");
        else vga_println("Failed to remove file.");
    }
    else if (strcmp(cmd, "echo") == 0) {
        char* text = strtok(nullptr, ">");
        if (!text) { vga_println("Usage: echo <text> > <file>"); return; }

        // Parse file name after '>'
        char* file = strtok(nullptr, " ");
        if (!file) { vga_println("Usage: echo <text> > <file>"); return; }

        // Trim leading space
        while (*file == ' ') file++;

        if (fs_write(file, text) == 0) vga_println("Written to file.");
        else vga_println("Failed to write file.");
    }
    else if (strcmp(cmd, "cat") == 0) {
        char* name = strtok(nullptr, " ");
        if (!name) { vga_println("Usage: cat <file>"); return; }
        const char* content = fs_read(name);
        if (!content) vga_println("File not found.");
        else vga_println(content);
    }
    else if (strcmp(cmd, "version") == 0){
        vga_println("CatOS Alpha Pre v1.0.0 Delta");
    }
    else if (strcmp(cmd, "") == 0){
        
    }
    else {
        vga_print("Unknown command: ");
        vga_println(cmd);
    }
}

void catcli_start() {
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_println("Welcome To CatOS");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    char input[128];
    while (1) {
        vga_print("cat>");
        keyboard_read_line(input, sizeof(input));
        handle_command(input);
    }
}
