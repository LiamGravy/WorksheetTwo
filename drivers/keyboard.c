#include "keyboard.h"
#include "io.h"
#include "shell.h"
#include "../idt/idt.h"

#define KEYBOARD_BUFFER_SIZE 256 // Size of the keyboard input buffer
static unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE]; // Buffer to store keyboard input
static unsigned char* buffer_head_pointer = keyboard_buffer; // Pointer to the head of the buffer
static unsigned char* buffer_tail_pointer = keyboard_buffer; // Pointer to the tail of the buffer


unsigned char keyboard_read_scancode(void) //Reads a scancode from the keyboard data port
{
    return inb(KEYBOARD_DATA_PORT); //Reads a byte from the keyboard data port
}

unsigned char map_keyboard_scan_to_ascii(unsigned char scan_code) //Maps scan codes to ASCII characters
{
    unsigned char ascii[256] = 
    {0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', 
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0, 
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0}; // Maps scan codes to ASCII characters
    return ascii[scan_code];
}

void buffer_keyboard_input(unsigned char ascii_char)
{
    unsigned char* max_size = keyboard_buffer + KEYBOARD_BUFFER_SIZE; //pointer to the end of the buffer
    unsigned char* new_tail_pointer = buffer_tail_pointer + 1; // Pointer to the next position to write to

    if (new_tail_pointer == max_size) //  Checks to see if the buffer is at max size
    {
        new_tail_pointer = keyboard_buffer; // Wraps around to the start of the buffer
    }

    if (new_tail_pointer == buffer_head_pointer) // If the buffer is full of characters
    {
        buffer_head_pointer++; // Advances the read pointer to make space - This overwrites the oldest character
        if (buffer_head_pointer == max_size) // Wraps around if needed
        {
            buffer_head_pointer = keyboard_buffer; 
        }
    }
    *buffer_tail_pointer = ascii_char; // Writes the character to the buffer
    buffer_tail_pointer = new_tail_pointer; // Advances the write pointer
}

unsigned char keyboard_pop_head_char()
{
    if (buffer_head_pointer == buffer_tail_pointer) // Buffer is empty
    {
        return 0; // No character to read
    }
    unsigned char c = *buffer_head_pointer; // Read the character at the head pointer
    buffer_head_pointer++; // Advance the head pointer
    if (buffer_head_pointer == keyboard_buffer + KEYBOARD_BUFFER_SIZE) // Checks if the head pointer is at the end of the buffer
    {
        buffer_head_pointer = keyboard_buffer; //Wraps around to the start of the buffer
    }
    return c; // Return the character read
}

void keyboard_interrupt() 
{
        unsigned char scancode = keyboard_read_scancode(); //Reads the scancode from the data port
        unsigned char ascii = map_keyboard_scan_to_ascii(scancode); //Maps the scancode to an ASCII character
        
        if (ascii != 0) //Checks if the character is valid
        {
            if (ascii == '\b') //Checks if the character is a backspace
            {
                int pos = get_cursor_position(); //Gets the current cursor position

                if (buffer_head_pointer != buffer_tail_pointer) 
                {
                   
                    if (buffer_tail_pointer == keyboard_buffer)  // Move the tail pointer back by one
                    {
                        
                        buffer_tail_pointer = keyboard_buffer + KEYBOARD_BUFFER_SIZE; // Handle wrap-around if we're at the start
                    }
                    buffer_tail_pointer--;
                }
                if (pos > 1) //Makes sure the cursor is not at cell 0 or 1
                { 
                    if (pos % 80 == 1) //Checks if the cursor is at the start of a line (> )
                    {
                        pos = pos - 2; //Moves the cursor to the end of the previous line. Back two spaces to move past the (>)
                        
                        while (pos > 1 && framebuffer_read_cell(pos) == ' ') //Moves backwards to find the last non-space character
                        {
                            pos--; //Moves back one cell
                            if (pos % 80 == 0)  //Checks if we have moved to the start of a line
                            {
                                pos--; //Moves back one more to move to previous line
                            }
                        }
                        
                        framebuffer_move_cursor(pos + 1); //Moves the cursor to the position after the last non-space character (+1 so it doesnt land on the character)
                    }
                    else
                    {
                    set_cursor_position((pos - 1) % 80, (pos - 1) / 80); //Moves the cursor back one position
                    framebuffer_write_cell(pos - 1, ' ', FB_LIGHT_GREY, BG_BLACK); //Writes a space to the current cursor position to erase/overwrite the character
                    framebuffer_move_cursor(pos - 1); //Moves the cursor back one position
                    }
                }
                return;
            }
            else if (ascii == '\n') //Checks if the character is a newline
            {
                //Jump to the shell file to check if it is a command
                newline(); //Writes a newline 
                EnterPressed = 1;
                // process_input(); //Processes the input from the keyboard buffer
                return;
            }
            else if (ascii == ESC) //Checks if the character is the escape key
            {
                outb(EXIT_PORT, EXIT_CODE);  // Exit QEMU
                return;
            }
            else                  //Prints the input character
            {
            char str[2] = {ascii, '\0'}; //Creates a string with the character and null terminator
            printf(str, 1); //Prints the character to the framebuffer (well and the null terminator)
            buffer_keyboard_input(ascii); //Buffers the character
            }
        }
        return;
}