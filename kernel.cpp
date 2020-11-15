// kernel.cpp - Main kernel source file.

void printf(char* str); 

extern "C" void kernelMain(void* multiboot_structure, unsigned int magicnumber) {
    printf("This is a test kernel!");

    while(1);
}

void printf(char* str) {
    unsigned short* VideoMemory = (unsigned short*)0xb8000;

    for(int i = 0; str[i] != '\0'; i++) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}
