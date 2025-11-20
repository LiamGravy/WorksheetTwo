


extern interrupt_handler                   ;Declares the interrupt_handler function as external (called from C code [idt.c])

%macro no_error_code_interrupt_handler 1   ;Macro to create interrupt handlers for interrupts without error codes: These push 0 as error code
global interrupt_handler_%1                ;Makes the handler globally accessible

interrupt_handler_%1:                      ;Push interrupt number AND !error code! then jumps to common handler
    push dword 0                           ;push 0 as an error code (double word - 4 bytes)
    push dword %1                          ;push the interrupt number 
    jmp common_interrupt_handler           ;jump to the common interrupt_handler
%endmacro                                  ;The end point of the macro

%macro error_code_interrupt_handler 1      ;Macro to create interrupt handlers for interrupts with error codes: These do not automatically push an error code
global interrupt_handler_%1                ;Makes the handler globally accessible

interrupt_handler_%1:                      ;Pushes interrupt number then jumps to common handler !no error code!
    push dword %1                          ;Push the interrupt number
    jmp common_interrupt_handler           ;jump to the common interrupt_handler
%endmacro                                  ;The end point of the macro

common_interrupt_handler:                  ;The common parts of the interrupt handlers (non and with codes)
                                           ;Save the registers: This order matters.
    push eax 
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ebp
    push esp 
    

    call interrupt_handler                 ;Call the C interrupt handler function (in idt.c)
                                           ;Restore the registers: The reverse of the push
    pop esp
    pop ebp
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    add esp, 8                             ;remove the pushed esp and interrupt number from the stack.
    iret                                   ;Return to the code that got interrupted


global load_idt                            ;Makes the load_idt function globally accessible 

load_idt:                                  ;Loads the IDT from the given address (Interrupt Descriptor Table)
    mov eax, [esp+4]                       ;Loads the address of the IDT to the register eax
    lidt [eax]                             ;Load the IDT - This is a special CPU instruction in x86 
    ret                                    ;Return to the calling function 

global enable_interrupts                   ;Makes the enable_interrupts function globally accessible

enable_interrupts:                         ;Enables CPU interrupts
    sti                                    ;Set the interrupt flag to enable interrupts
    ret                                    ;Return to the calling function


                                           ;Certain interrupts push an error code onto the stack, others do not. (8,10,11,12,13,14,17,21,29,30 push error codes)
no_error_code_interrupt_handler 0          ;Creates interrupt handler for interrupt 0 (no error code)
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler 8              ;Creates interrupt handler for interrupt 8 (with error code)
no_error_code_interrupt_handler 9
error_code_interrupt_handler 10             ;Creates interrupt handler for interrupt 10 (with error code)
error_code_interrupt_handler 11             ;Creates interrupt handler for interrupt 11 (with error code)
error_code_interrupt_handler 12             ;Creates interrupt handler for interrupt 12 (with error code)
error_code_interrupt_handler 13             ;Creates interrupt handler for interrupt 13 (with error code)
error_code_interrupt_handler 14             ;Creates interrupt handler for interrupt 14 (with error code)
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
error_code_interrupt_handler 17             ;Creates interrupt handler for interrupt 17 (with error code)
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
error_code_interrupt_handler 21             ;Creates interrupt handler for interrupt 21 (with error code)
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
error_code_interrupt_handler 29             ;Creates interrupt handler for interrupt 29 (with error code)
error_code_interrupt_handler 30             ;Creates interrupt handler for interrupt 30 (with error code)
no_error_code_interrupt_handler 31
no_error_code_interrupt_handler 32
no_error_code_interrupt_handler 33

global interrupt_handlers                   ;Makes the interrupt_handlers array globally accessible 
section .data                               ;Data section to store the interrupt handler addresses
interrupt_handlers:                         ;Array of pointers to the interrupt handlers
    dd interrupt_handler_0                  ;Each entry is a double word (4 bytes) pointing to the respective interrupt handler     
    dd interrupt_handler_1
    dd interrupt_handler_2
    dd interrupt_handler_3
    dd interrupt_handler_4
    dd interrupt_handler_5
    dd interrupt_handler_6
    dd interrupt_handler_7
    dd interrupt_handler_8
    dd interrupt_handler_9
    dd interrupt_handler_10
    dd interrupt_handler_11
    dd interrupt_handler_12
    dd interrupt_handler_13
    dd interrupt_handler_14
    dd interrupt_handler_15
    dd interrupt_handler_16
    dd interrupt_handler_17
    dd interrupt_handler_18
    dd interrupt_handler_19
    dd interrupt_handler_20
    dd interrupt_handler_21
    dd interrupt_handler_22
    dd interrupt_handler_23
    dd interrupt_handler_24
    dd interrupt_handler_25
    dd interrupt_handler_26
    dd interrupt_handler_27
    dd interrupt_handler_28
    dd interrupt_handler_29
    dd interrupt_handler_30
    dd interrupt_handler_31
    dd interrupt_handler_32
    dd interrupt_handler_33


