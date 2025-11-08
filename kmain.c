
#include "./drivers/io.h"
#include "./gdt/gdt.h"
#include "./idt/idt.h"


/* The C function */
int sum_of_three(int arg1, int arg2, int arg3)
{
return arg1 + arg2 + arg3;
}

int mult_of_three(int arg1, int arg2, int arg3)
{
return arg1 * arg2 * arg3;
}

int max_of_three(int arg1, int arg2, int arg3)
{
    int max;
    if (arg1 >= arg2 && arg2 >= arg3)
        max = arg1;
    else if (arg2 >= arg1 && arg2 >= arg3)
        max = arg2;
    else
        max = arg3;
    //serial_write_integer(0x3F8, max);
    return max;
}


void kmain()
{
    init_gdt();
    init_idt();

    clear_screen();
    serial_write_string(0x3F8, "This is the serial port fyi...\n");
    //printf("print('Hello World!')", 1);
    //newline();
    set_text_colour(FB_GREEN, BG_BLUE);
    //printf("Hello World!", 1);

    enable_interrupts();
}