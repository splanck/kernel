.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj

# Macro function to handle exceptions.
.macro HandleException num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
    mov $\num, (interruptnumber)
    jmp int_bottom
.endm

# Macro function to pass interrupt number to cpp event handler.
.macro HandleInterruptRequest num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
    mov $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00 # Interrupt request handler for timer interrupt
HandleInterruptRequest 0x01 # Interrupt request handler for keyboard input

int_bottom:
    # Store away current stack
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # Call interrupt handler passing in interrupt number
    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj

    mov %eax, %esp

    # Pop previous context from the stack to restore values for previous process.
    popl %gs
    popl %es
    popl %fs
    popl %ds
    popa

    iret

.data
    interruptnumber: .byte 0
    