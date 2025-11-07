#ifndef IDT_H
#define IDT_H

struct idt_entry_struct
{
    unsigned short isr_low; //The lower 16 bits of the ISR's address
    unsigned short kernel_cs; //The GDT segment selector that the CPU will load into CS before calling the ISR
    unsigned char reserved; // Set to zero
    unsigned char attributes; //Type and atrributes of the interrupt
    unsigned short isr_high; //The higher 16 bits of the ISR address
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

struct idt_pointer_struct 
{
    unsigned short limit; //The 16 bit size/limit of the IDT
    unsigned int base; //The 32 bit address of the first element in the IDT
} __attribute__((packed));

typedef struct idt_pointer_struct idt_ptr_t;

struct cpu_state_struct
{
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int esp;
} __attribute__((packed));

typedef struct cpu_state_struct cpu_state_t;

struct stack_state_struct
{
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

typedef struct stack_state_struct stack_state_t;

void interrupt_handler(cpu_state_t cpu, stack_state_t stack, unsigned int interrupt);
void init_idt(void);
extern void load_idt(unsigned int);
extern void interrupt_handler_33(void);
extern void enable_interrupts(void);

#endif