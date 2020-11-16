// kernel.cpp - Main kernel source file.

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors();
void kprint(char* s); 

// Execution starting point for the kernel.
extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
    kprint("This is a test kernel!");

    while(1);
}

// Call all constructors for initialization.
extern "C" void callConstructors() {
    for (constructor* x = &start_ctors; x != &end_ctors; x++)
        (*x)();
}

// Basic text print function.
void kprint(char* s) {
    unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int x = 0; s[x] != '\0'; x++) {
        VideoMemory[x] = (VideoMemory[x] & 0xFF00) | s[x];
    }
}
