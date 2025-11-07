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
extern void enable_interrupts(void);




extern void interrupt_handler_0(void);
extern void interrupt_handler_1(void);
extern void interrupt_handler_2(void);
extern void interrupt_handler_3(void);
extern void interrupt_handler_4(void);
extern void interrupt_handler_5(void);
extern void interrupt_handler_6(void);  
extern void interrupt_handler_7(void);
extern void interrupt_handler_8(void);
extern void interrupt_handler_9(void);
extern void interrupt_handler_10(void);
extern void interrupt_handler_11(void);
extern void interrupt_handler_12(void);
extern void interrupt_handler_13(void);
extern void interrupt_handler_14(void);
extern void interrupt_handler_15(void);
extern void interrupt_handler_16(void);
extern void interrupt_handler_17(void);
extern void interrupt_handler_18(void);
extern void interrupt_handler_19(void);
extern void interrupt_handler_20(void);
extern void interrupt_handler_21(void);
extern void interrupt_handler_22(void);
extern void interrupt_handler_23(void);
extern void interrupt_handler_24(void);
extern void interrupt_handler_25(void);
extern void interrupt_handler_26(void);
extern void interrupt_handler_27(void);
extern void interrupt_handler_28(void);
extern void interrupt_handler_29(void);
extern void interrupt_handler_30(void);
extern void interrupt_handler_31(void);
extern void interrupt_handler_32(void);
extern void interrupt_handler_33(void);
#endif