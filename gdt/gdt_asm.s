global gdt_flush        ;Allows C code to call the gdt_flush()

gdt_flush:
    mov eax, [esp+4]    ; Gets the pointer to the GPT
    lgdt[eax]           ; Loads the new GDT poitner

    mov ax, 0x10    ; 0x10 is the offset for the Kernel Data segment      
    mov ds, ax      ; Loads the offset for each of the data segment registers
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax


    ; Below is the far jump. It loads cs (code segment register) and eip (instruction pointer)
    jmp 0x08:flush_cs ;Flushes the code segment register

    flush_cs: 
        ; After jumping here the cs now equals 0x08
        ret
