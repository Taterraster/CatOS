#include "vga.hpp"
#include "keyboard.hpp"
#include "catcli.hpp"

extern "C" void kernel_main() {
    vga_clear();
    vga_println("catOS Kernel starting...");
    keyboard_init();

    catcli_start();  // Start the CLI

    // CLI exited, halt CPU
    vga_println("CLI exited. Halting system...");
    asm volatile("cli; hlt");  // CPU stops here
}
