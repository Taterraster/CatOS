
#include "vga.hpp"


extern "C" void kernel_main();


namespace {


    class Kitty {
        public:
            Kitty() {
                catos::VGA::write("[catOS] Kitty constructor running\n");
            }
    };



    static Kitty global_kitty;


} 


extern "C" void kernel_main() {
    catos::VGA::clear();
    catos::VGA::write("Welcome to catOS (C++ kernel)\n");
    catos::VGA::write("Type: minimal kernel no Linux involved.\n\n");


    catos::VGA::write("Hello from kernel_main!\n");

    catos::VGA::write("Made by Taterr\n");

    for (;;) {
        asm volatile ("hlt");
    }
}