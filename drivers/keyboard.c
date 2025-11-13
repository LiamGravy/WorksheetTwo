#include "keyboard.h"
#include "io.h"

#define KEYBOARD_BUFFER_SIZE 256
static unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static unsigned char* buffer_head_pointer = 0;
static unsigned char* buffer_tail_pointer = 0;


unsigned char keyboard_read_scancode(void)
{
    return inb(KEYBOARD_DATA_PORT);
}

unsigned char map_keyboard_scan_to_ascii(unsigned char scan_code)
{
    unsigned char ascii[256] =
    {0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', 
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0, 
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0};
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

unsigned char keyboard_get_char()
{

    unsigned char c = keyboard_buffer[buffer_head_pointer];
    buffer_head_pointer = (buffer_head_pointer + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_interrupt()
{
        unsigned char scancode = keyboard_read_scancode();
        unsigned char ascii = map_keyboard_scan_to_ascii(scancode);
        
        if (ascii != 0)
        {
            if (ascii == '\b')
            {
                int pos = get_cursor_position();
                
                if (pos > 1)
                { 
      if (pos % 80 == 1)
                    {
                        pos = pos - 2;
                        
                        while (pos > 1 && framebuffer_read_cell(pos) == ' ')
                        {
                            pos--;
                            if (pos % 80 == 0) 
                            {
                                pos--;
                            }
                        }
                        
                        framebuffer_move_cursor(pos + 1);
                    }
                    else
                    {
                    set_cursor_position((pos - 1) % 80, (pos - 1) / 80);
                    framebuffer_write_cell(pos - 1, ' ', FB_LIGHT_GREY, BG_BLACK);
                    framebuffer_move_cursor(pos - 1);
                    }
                }
                return;
            }
            else if (ascii == '\n')
            {
                //Jump to the shell file to check if it is a command
                newline();
                return;
            }
            else if (ascii == 27)
            {
                outb(0xf4, 0x00);  // Exit QEMU
                return;
            }
            {
            char str[2] = {ascii, '\0'};
            printf(str, 1);
            }
        }
        return;
}