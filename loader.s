
global loader ;Makes the loader function globally accessible (outside of the file)

MAGIC_NUMBER equ 0x1BADB002 ;Magic number to identify the kernel
FLAGS equ 0x0 ;Sets flags as 0. Too keep the kernel simple and small (no extra flags to complicate things)
CHECKSUM equ -MAGIC_NUMBER ;Sets the checksum so the OS kernel can be verified

KERNEL_STACK_SIZE equ 4096 ;Defines the size of the kernel stack

section .bss 
align 4 ;Aligns the kernel stack to a 4-byte boundary
kernel_stack:
    resb KERNEL_STACK_SIZE ;Reserves space for the kernel stack (4096 bytes)

section .text
align 4 ;Makes sure that the Magic number etc is aligned to a 4-byte boundary
    dd MAGIC_NUMBER ;Defines the magic number as a 4 byte double word
    dd FLAGS 
    dd CHECKSUM

loader:
    mov eax, 0xCAFEBABE ;Moves the test value CAFEBABE into the eax register
    mov esp, kernel_stack + KERNEL_STACK_SIZE ;Sets the stack pointer (esp) to the top of the stack (4096)

    extern sum_of_threes ;Tells the assembler that sum_of_threes is an external function
    extern mult_of_threes
    extern max_of_threes
    extern kmain ;Tells the assembler that kmain is an external function
    call kmain ;Calls the kmain function
    push dword 5 ;Pushes the integer 5 onto the stack 
    push dword 10 ;Pushes the integer 10 onto the stack
    push dword 15 ;Pushes the integer 15 onto the stack
    call sum_of_threes ;Calls the sum_of_threes function
    push dword 2
    push dword 3
    push dword 4
    call mult_of_threes
    push dword 10
    push dword 25
    push dword 15
    call max_of_threes

.loop:
    jmp .loop ;Infinite loop to prevent the kernel from exiting