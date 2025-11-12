
#include "./drivers/io.h"
#include "./gdt/gdt.h"
#include "./idt/idt.h"


/* Three simple functions as required by the Worksheet */

/* Returns the sum of three integers */
int sum_of_three(int arg1, int arg2, int arg3)
{
return arg1 + arg2 + arg3;
}

/* Returns the multiplication of three integers */
int mult_of_three(int arg1, int arg2, int arg3)
{
return arg1 * arg2 * arg3;
}

/* Returns the maximum of three integers */
int max_of_three(int arg1, int arg2, int arg3) 
{
    int max;
    if (arg1 >= arg2 && arg2 >= arg3)
        max = arg1;
    else if (arg2 >= arg1 && arg2 >= arg3)
        max = arg2;
    else
        max = arg3;
    return max;
}

/* Kernel main function - the entry point for the kernel */

void kmain()
{
    init_gdt(); // Initialize the Global Descriptor Table
    init_idt(); // Initialize the Interrupt Descriptor Table
    clear_screen(); //Clear the screen of all the initial junk
    set_text_colour(FB_GREEN, BG_BLUE); // Set the fg and bg colours for text output (colours found in io.h)
    enable_interrupts(); //Enables interrupts 
}