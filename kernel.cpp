// kernel.cpp - Main kernel source file.

#include "types.h"
#include "mykernel.h"

// Execution starting point for the kernel.
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    kprint("This is a test kernel! OH OK!");

    while(1);
}

// Call all constructors for initialization.
extern "C" void callConstructors() {
    for (constructor* x = &start_ctors; x != &end_ctors; x++)
        (*x)();
}

// Basic text print function.
void kprint(char* s) {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for(int x = 0; s[x] != '\0'; x++) {
        VideoMemory[x] = (VideoMemory[x] & 0xFF00) | s[x];
    }
}
