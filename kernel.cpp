// kernel.cpp - Main kernel source file.

void kprint(char* s); 

// Execution starting point for the kernel.
extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
    kprint("This is a test kernel!");

    while(1);
}

// Basic text print function.
void kprint(char* s) {
    unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int i = 0; s[i] != '\0'; i++) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | s[i];
    }
}
