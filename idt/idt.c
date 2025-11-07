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

    if(interrupt == 32)
    {
        pic_acknowledge_interrupt(interrupt);
        return;
    }


    if(interrupt == KEYBOARD_INTERRUPT)
    {
        
        unsigned char scancode = keyboard_read_scancode();
        unsigned char ascii = map_keyboard_scan_to_ascii(scancode);
        
        
        if (ascii != 0)
        {
            char str[2] = {ascii, '\0'};
            printf(str, 1);
        }
        
        
        pic_acknowledge_interrupt(interrupt);
    }
}

void init_idt(void)
{
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt;

    set_idt_entry(0, (unsigned int)interrupt_handler_0);
    set_idt_entry(1, (unsigned int)interrupt_handler_1);
    set_idt_entry(2, (unsigned int)interrupt_handler_2);
    set_idt_entry(3, (unsigned int)interrupt_handler_3);
    set_idt_entry(4, (unsigned int)interrupt_handler_4);
    set_idt_entry(5, (unsigned int)interrupt_handler_5);
    set_idt_entry(6, (unsigned int)interrupt_handler_6);  
    set_idt_entry(7, (unsigned int)interrupt_handler_7);
    set_idt_entry(8, (unsigned int)interrupt_handler_8);
    set_idt_entry(9, (unsigned int)interrupt_handler_9);
    set_idt_entry(10, (unsigned int)interrupt_handler_10);
    set_idt_entry(11, (unsigned int)interrupt_handler_11);
    set_idt_entry(12, (unsigned int)interrupt_handler_12);
    set_idt_entry(13, (unsigned int)interrupt_handler_13);
    set_idt_entry(14, (unsigned int)interrupt_handler_14);
    set_idt_entry(15, (unsigned int)interrupt_handler_15);
    set_idt_entry(16, (unsigned int)interrupt_handler_16);
    set_idt_entry(17, (unsigned int)interrupt_handler_17);
    set_idt_entry(18, (unsigned int)interrupt_handler_18);
    set_idt_entry(19, (unsigned int)interrupt_handler_19);
    set_idt_entry(20, (unsigned int)interrupt_handler_20);
    set_idt_entry(21, (unsigned int)interrupt_handler_21);
    set_idt_entry(22, (unsigned int)interrupt_handler_22);
    set_idt_entry(23, (unsigned int)interrupt_handler_23);
    set_idt_entry(24, (unsigned int)interrupt_handler_24);
    set_idt_entry(25, (unsigned int)interrupt_handler_25);
    set_idt_entry(26, (unsigned int)interrupt_handler_26);
    set_idt_entry(27, (unsigned int)interrupt_handler_27);
    set_idt_entry(28, (unsigned int)interrupt_handler_28);
    set_idt_entry(29, (unsigned int)interrupt_handler_29);
    set_idt_entry(30, (unsigned int)interrupt_handler_30);
    set_idt_entry(31, (unsigned int)interrupt_handler_31);
    set_idt_entry(32, (unsigned int)interrupt_handler_32);
    set_idt_entry(KEYBOARD_INTERRUPT, (unsigned int)interrupt_handler_33);
    pic_remap();
    load_idt((unsigned int)&idt_ptr);
    outb(MASTER_PIC_DATA_PORT, 0xFC);
}