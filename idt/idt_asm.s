


extern interrupt_handler

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
    push dword 0 ; push 0 as an error code
    push dword %1 ;push the interrupt number
    jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
    push dword %1 ;Push the interrupt number
    jmp common_interrupt_handler ;jump to the common interrupt_handler
%endmacro

common_interrupt_handler: ;The common parts of the generic interrupt interrupt_handler
;Save the registers
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ebp
    push esp
    

    call interrupt_handler
;Restore the registers
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
    add esp, 8 ;remove the pushed esp and interrupt number from the stack
    iret ;Return to the code that got interrupted


global load_idt

load_idt:
    mov eax, [esp+4] ;Loads the address of the IDT to the register eax
    lidt [eax] ;Load the IDT
    ret ;Return to the calling function

global enable_interrupts

enable_interrupts:
    sti ;Set the interrupt flag to enable interrupts
    ret ;Return to the calling function

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler 8
no_error_code_interrupt_handler 9
error_code_interrupt_handler 10
error_code_interrupt_handler 11
error_code_interrupt_handler 12
error_code_interrupt_handler 13
error_code_interrupt_handler 14

no_error_code_interrupt_handler 33



