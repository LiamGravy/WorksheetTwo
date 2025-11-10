#include "io.h"

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define SERIAL_CON1_BASE 0x3F8
#define SERIAL_DATA_PORT(base) (base) 
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5) 

#define SERIAL_LINE_ENABLE_DLAB 0x80 //Tells the serial port to expect the highest 8 bits on the data port, then the lower 8 bits will follow.

static unsigned int cursor_position = 0;
static unsigned int fg_colour = FB_WHITE;
static unsigned int bg_colour = BG_BLACK;

//Serial configure baud rate: sets the speed of the data being sent. Default is 115200 bits/s

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com), (divisor & 0x00FF));
}

//Serial configure line: configures the serial port to use 8 bits, no parity, one stop bit.
void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03); 
}

//Serial configure FIFO: enables FIFO, clears the transmit and receive FIFO queues, sets queue length to 14 bytes.
void serial_configure_FIFO(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

//Serial configure modem: sets Ready To Transmit and Data Terminal Ready to on.
void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

//Checks if the serial port transmit FIFO queue is empty.
int serial_is_transmit_empty(unsigned short com)
{
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write_character(unsigned short com, unsigned char data) //com is the base address of the serial port
{
    while (serial_is_transmit_empty(com) == 0)
    {
        //Waits for the transmit FIFO queue to be empty
    }
    outb(SERIAL_DATA_PORT(com), data);

}

void serial_write_integer(unsigned short com, unsigned int data) //com is the base address of the serial port
{
    while (serial_is_transmit_empty(com) == 0)
    {
        //Waits for the transmit FIFO queue to be empty
    }
    outb(SERIAL_DATA_PORT(com), data);
}

int serial_write_string(unsigned short com, const char* str) {
    for (int i =0; str[i] != '\0'; i++) {
        serial_write_character(com, (unsigned char) str[i]);
    }
    return 1;
}

void clear_screen() 
{
    for (unsigned int i = 0; i < 80 *25; i++) 
    {
        if (i % 80 == 0)
        {
            framebuffer_write_cell(i, '>', FB_WHITE, BG_BLACK);
        }
        else 
        {
        framebuffer_write_cell(i, ' ', 0, 0);
        }
    }
    framebuffer_move_cursor(1);
}

void framebuffer_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;  // Address B8000 is used as a char pointer. 
    fb[i*2] = c; // Places character c at the relevant position in the framebuffer. Byte offset is 2 bytes.
    fb[i*2+1] = ((bg & 0x0F) << 4 | (fg & 0x0F)); //Performs a bitwise AND operation to ensure only the lower 4 bits of fg and bg are used. Shifts the background color 4 bits to the left to occupy the higher nibble. Combines the background and foreground colors using a bitwise OR operation. 
}

int fb_write_string(const char* str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (cursor_position % 80 == 0)
        {
            cursor_position++;
        }
        framebuffer_write_cell(cursor_position, str[i], fg_colour, bg_colour);
        framebuffer_move_cursor(cursor_position + 1); 
        i++;
    }
    return 1;
   
}

void framebuffer_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
    cursor_position = pos;

}

void printf(const char* str, unsigned int option)
{
    if (option == 0) //Serial
    {
        serial_write_string(SERIAL_CON1_BASE, str);
    }
    else if (option == 1) //FrameBuffer
    {
        fb_write_string(str);
    }
}

//grid is 80x25 
void set_cursor_position(unsigned int x, unsigned int y)
{
    framebuffer_move_cursor(y * 80 + x);
}

void set_text_colour(unsigned int fg, unsigned int bg)
{
    fg_colour = fg;
    bg_colour = bg;
}

void newline()
{
    framebuffer_move_cursor(((cursor_position/80) + 1) * 80);
}

void io_wait(void)
{
    outb(0x80, 0);
}