#include "idt.h"
#include "pic.h"
#include "../drivers/io.h"
#include "../drivers/keyboard.h"

#define IDT_SIZE 256
#define KEYBOARD_INTERRUPT 33 //The keyboard interrupt is number 33

static idt_entry_t idt[IDT_SIZE]; //Creates an IDT with 256 entries
static idt_ptr_t idt_ptr; //Creates the IDT pointer structure

void set_idt_entry(unsigned int index, unsigned int isr_address) //Sets an entry in the IDT
{
    idt[index].isr_low = (isr_address & 0xFFFF); //Sets the lower 16 bits of the ISR address
    idt[index].isr_high = (isr_address >> 16) & 0xFFFF; //Sets the higher 16 bits of the ISR address
    idt[index].kernel_cs = 0x08; //Sets the kernel code segment selector
    idt[index].reserved = 0x00; //Sets the reserved byte to 0
    idt[index].attributes = (0x01 << 7) | (0x00 << 6) | (0x00 <<5) | 0xe; //Sets the attributes: Present, DPL 0, Storage Segment 0, 32-bit Interrupt Gate
}

void interrupt_handler(cpu_state_t cpus, unsigned int interrupt, stack_state_t stack) //Builds the main interrupt handler
{
    (void)cpus; //Doesnt break the OS if cpus is unused
    (void)stack; //Doesnt break the OS if stack is unused

    if(interrupt == 32) //Timer interrupt (broke without this)
    {
        
        pic_acknowledge_interrupt(interrupt); //Acknowledge the PIC
        return; 
    }

    if(interrupt == KEYBOARD_INTERRUPT) //Keyboard interrupt (code 33)
    {
        keyboard_interrupt();  //Handles keyboard interrupts
        pic_acknowledge_interrupt(interrupt); //Acknowledge the PIC that the interrupt has been handled
        return;
    }
}

void init_idt(void) //Initialises the IDT
{
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1; //Sets the limit of the IDT
    idt_ptr.base = (unsigned int)&idt; //Sets the base address of the IDT

    for (unsigned int i = 0; i < 33; i++) //Sets the first 33 entries in the IDT
    {
        set_idt_entry(i, interrupt_handlers[i]); //Sets the IDT entry to the corresponding interrupt handler (from idt.h and idt_asm.s)
    }

    set_idt_entry(KEYBOARD_INTERRUPT, (unsigned int)interrupt_handlers[33]); //Sets the keyboard interrupt handler
    pic_remap(); //Remaps the PIC to avoid conflicts with CPU exceptions
    load_idt((unsigned int)&idt_ptr); //Loads the IDT
    outb(MASTER_PIC_DATA_PORT, 0xFC); //Masks all interrupts except the timer and keyboard 0xFC = 11111100
}

