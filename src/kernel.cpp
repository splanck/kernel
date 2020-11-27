// kernel.cpp - Main kernel source file.
#include "types.h"
#include "gdt.h"
#include "kernel.h"

// Execution starting point for the kernel.
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
{
    kprint("This is a test kernel! OH OK!\n");
    kprint("This is another test!");

    GlobalDescriptorTable gdt;

    while(1);
}

// Call all constructors for initialization.
extern "C" void callConstructors() 
{
    for (constructor* x = &start_ctors; x != &end_ctors; x++) 
    {
        (*x)();
    }
}

// Basic text print function.
void kprint(char* s) 
{
    uint16_t* vmem = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;
    uint8_t c = 80 * y + x;

    for(int i = 0; s[i] != '\0'; i++) 
    {
        if(s[i] == '\n')
        {
            y++;
            x = 0;
        }
        else
        {
            c = 80 * y + x;
            vmem[c] = (vmem[c] & 0xFF00) | s[i];

            x++;
        }

        if(x >= 80) 
        {
            y++;
            x = 0;
        }

        if(y >= 25) 
        {
            for(y = 0; y < 25; y++)
            {
                for(x = 0; x < 80; x++)
                {
                    c = 80 * y + x;
                    vmem[c] = (vmem[c] & 0xFF00) | ' ';
                }
            }

            x = 0;
            y = 0;
        }
    }
}
