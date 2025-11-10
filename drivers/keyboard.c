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
    {0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
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