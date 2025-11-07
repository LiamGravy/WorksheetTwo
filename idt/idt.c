#include "idt.h"
#include "pic.h"
#include "../drivers/io.h"
#include "../drivers/keyboard.h"

#define IDT_SIZE 256
#define KEYBOARD_INTERRUPT 33

static idt_entry_t idt[IDT_SIZE];
static idt_ptr_t idt_ptr;

void set_idt_entry(unsigned int index, unsigned int isr_address)
{
    idt[index].isr_low = (isr_address & 0xFFFF);
    idt[index].isr_high = (isr_address >> 16) & 0xFFFF;
    idt[index].kernel_cs = 0x08;
    idt[index].reserved = 0x00;
    idt[index].attributes = (0x01 << 7) | (0x00 << 6) | (0x00 <<5) | 0xe;
}

void interrupt_handler(cpu_state_t cpus, stack_state_t stack, unsigned int interrupt)
{
    (void)cpus;
    (void)stack;

    if(interrupt == KEYBOARD_INTERRUPT)
    {
        /*
        unsigned char scancode = keyboard_read_scancode();
        unsigned char ascii = map_keyboard_scan_to_ascii(scancode);
        */
        /*
        if (ascii != 0)
        {
            char str[2] = {ascii, '\0'};
            printf(str, 1);
        }
        */
        
        pic_acknowledge_interrupt(interrupt);
    }
}

void init_idt(void)
{
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt;

    set_idt_entry(KEYBOARD_INTERRUPT, (unsigned int)interrupt_handler_33);
    pic_remap();
    load_idt((unsigned int)&idt_ptr);
    outb(MASTER_PIC_COMMAND_PORT, 0xFD);
}