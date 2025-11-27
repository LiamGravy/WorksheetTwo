
#include "./drivers/io.h"
#include "./gdt/gdt.h"
#include "./idt/idt.h"


/* Three simple functions as required by the Worksheet */

/* Returns the sum of three integers */


/* Returns the multiplication of three integers */


/* Returns the maximum of three integers */


/* Kernel main function - the entry point for the kernel */

void kmain()
{
    init_gdt(); // Initialize the Global Descriptor Table
    init_idt(); // Initialize the Interrupt Descriptor Table
    clear_screen(); //Clear the screen of all the initial junk
    set_text_colour(FB_GREEN, BG_BLACK); // Set the fg and bg colours for text output (colours found in io.h)
    enable_interrupts(); //Enables interrupts 
}