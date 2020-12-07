#include "interrupts.h"

void kprint(char* s);

InterruptManager::GateDescriptor InterruptManager::InterruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegnentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    InterruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    InterruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    InterruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegnentSelectorOffset;
    InterruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | 
        ((DescriptorPrivilegeLevel & 3) << 5);
    InterruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picMasterCommand(0x20),
picMasterData(0x21),
picSlaveCommand(0xA0),
picSlaveData(0xA1)
{
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    // Create entries for all 256 interrupts and direct them to the interrupt ignore function.
    for(uint16_t x = 0; x <= 255; x++)
    {
        SetInterruptDescriptorTableEntry(x, CodeSegment, &IgnoreInterruptRequest, 0, 
            IDT_INTERRUPT_GATE);
    }

    // Setup interrupt handler to handle the interrupt zero timer.
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, 
        IDT_INTERRUPT_GATE);

    // Setup interrupt handle for handle interrupts from the keyboard.
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, 
        IDT_INTERRUPT_GATE);

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);
    
    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    // Tell the CPU to use the new interrupt descriptor table.
    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)InterruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager()
{
}

void InterruptManager::Activate()
{
    asm("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) 
{
    kprint("Interrupt!\n");

    return esp;
}
