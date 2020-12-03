#include "interrupts.h"

void kprint(char* s);

InterruptManager::GateDescriptor InterruptManager::InterruptDescriptorTable[256];

static void SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegnentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilageLevel,
    uint8_t DescriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    InterruptDescriptorTable(interruptNumber).handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    InterruptDescriptorTable(interruptNumber).handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
    InterruptDescriptorTable(interruptNumber).gdt_codeSegmentSelector = codeSegnentSelectorOffset;
    InterruptDescriptorTable(interruptNumber).access = IDT_DESC_PRESENT | DescriptorType | 
        ((DescriptorPrivilageLevel&3) << 5);
    InterruptDescriptorTable(interruptNumber).reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
{
    uint16_t CodeSegment = gdt->CodeSegmentSelector;
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    // Create entries for all 256 interrupts and direct them to the interrupt ignore function.
    for(uint16_t x = 0; x <= 255; x++)
    {
        SetInterruptDescriptorTableEntry(x, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    // Setup interrupt handler to handle the interrupt zero timer.
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, 
        IDT_INTERRUPT_GATE);

    // Setup interrupt handle for handle interrupts from the keyboard.
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, 
        IDT_INTERRUPT_GATE);

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
    asm ("sti");
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) 
{
    kprint("Interrupt!\n");

    return esp;
}