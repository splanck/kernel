.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv 

# Macro function to handle exceptions.
.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

# Macro function to pass interrupt number to cpp event handler.
.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

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

    movl %eax, %esp

    # Pop previous context from the stack to restore values for previous process.
    popl %gs
    popl %es
    popl %fs
    popl %ds
    popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0
    