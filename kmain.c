
#include "./drivers/io.h"
#include "./gdt/gdt.h"
#include "./idt/idt.h"
#include "./drivers/shell.h"
#include "./drivers/timerInterrupt.h"
#include "./snake/randomNumberGenerator.h"


int sum_of_threes(int arg1, int arg2, int arg3)
{
    int sum = arg1 + arg2 + arg3;
    char tempstring[16];
    integer_to_char(sum, tempstring);
    printf("Sum result: ", 1);
    printf(tempstring, 1);  
    newline();
return sum;
}

int mult_of_threes(int arg1, int arg2, int arg3)
{
    int mult = arg1 * arg2 * arg3;
    char tempstring[16];
    integer_to_char(mult, tempstring);
    printf("Multiplication result: ", 1);
    printf(tempstring, 1);
    newline();
return mult;
}

int max_of_threes(int arg1, int arg2, int arg3) 
{
    int max;
    max = arg1;
    if (arg2 > max) 
        max = arg2;
    if (arg3 > max) 
        max = arg3;
    char tempstring[16];
    integer_to_char(max, tempstring);
    printf("Maximum result: ", 1);
    printf(tempstring, 1);
    newline();
    return max;
}


/* Kernel main function - the entry point for the kernel */

void kmain()
{
    init_gdt(); // Initialize the Global Descriptor Table
    init_idt(); // Initialize the Interrupt Descriptor Table
    clear_screen(); //Clear the screen of all the initial junk
    set_text_colour(FB_GREEN, BG_BLACK); // Set the fg and bg colours for text output (colours found in io.h)
    enable_interrupts(); //Enables interrupts 
    get_timer_ticks();

    // sum_of_threes(5, 10, 15); //Test sum_of_three function
    // mult_of_threes(2, 3, 4); //Test mult_of_three
    // max_of_threes(10, 25, 15); //Test max_of_three function

    //clear_screen();
    // set_text_colour(FB_CYAN, BG_BLACK);
    // fb_write_string("This is testing the framebuffer write string");
    // newline();
    // set_text_colour(FB_RED, BG_BLACK);
    // fb_write_string("This is testing the newline function");
    // set_text_colour(FB_GREEN, BG_BLACK);
    // newline();
    // fb_write_string("This is testing the set text colour function");


    // init_serial(3); 
    // serial_write_string("Testing the serial port write string function");
    // serial_newline();

    // serial_write_string("Testing writing an integer to serial: ");
    // serial_write_integer(123);
    // serial_newline();

    // printf("Printing to Serial Only", 0);
    // serial_newline();

    // printf("Printing to Framebuffer Only", 1);
    // newline();

    // printf("Writing integer to Framebuffer: ", 1);
    // fb_write_integer(123);
    // newline();

    // printf("Printing to Both Serial and Framebuffer", 2);
    // newline();

    // set_cursor_position(79, 24);

}