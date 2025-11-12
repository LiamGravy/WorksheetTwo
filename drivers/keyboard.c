#include "keyboard.h"
#include "io.h"

#define KEYBOARD_BUFFER_SIZE 256
static unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static unsigned int buffer_read_pointer = 0;
static unsigned int buffer_write_pointer = 0;


unsigned char keyboard_read_scancode(void)
{
    return inb(KEYBOARD_DATA_PORT);
}

unsigned char map_keyboard_scan_to_ascii(unsigned char scan_code)
{
    unsigned char ascii[256] =
    {0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', //The escape key character is
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0, 
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0};
    return ascii[scan_code];
}

void buffer_keyboard_input(unsigned char scancode)
{
    unsigned char ascii_char = map_keyboard_scan_to_ascii(scancode);
    if (ascii_char != 0)
    {
        if ((buffer_write_pointer + 1) % KEYBOARD_BUFFER_SIZE == buffer_read_pointer)
        {
            // Buffer is full, discard the character
            return;
        }
        keyboard_buffer[buffer_write_pointer] = ascii_char;
        buffer_write_pointer = (buffer_write_pointer + 1) % KEYBOARD_BUFFER_SIZE;
    }

}

unsigned char keyboard_get_char(void)
{
    if (buffer_read_pointer == buffer_write_pointer)
    {
        return 0;
    }
    unsigned char c = keyboard_buffer[buffer_read_pointer];
    buffer_read_pointer = (buffer_read_pointer + 1) % KEYBOARD_BUFFER_SIZE;
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