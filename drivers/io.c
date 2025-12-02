#include "io.h"

/* Framebuffer cursor position and text colours */
static unsigned int cursor_position = 0; //Initial cursor position at top left of screen, cell 0
static unsigned int fg_colour = FB_WHITE; //Initial foreground colour is white
static unsigned int bg_colour = BG_BLACK; //Initial background colour is black


/*Serial configure baud rate: sets the speed of the data being sent. Clock speed is 115200 bits/s */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),SERIAL_LINE_ENABLE_DLAB); //Enable DLAB. Sends the conmand 0x80 which tells the serial port to expect the high 8 bits then the low 8 bits of the divisor
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF); //Sets the high byte of the divisor. >> 8 shifts the bits right 8 places. & 0x00FF does bitwise AND to make sure only the lower 8 bits can be a 1
    outb(SERIAL_DATA_PORT(com), (divisor & 0x00FF)); //Sets the low byte of the divisor. & 0x00FF makes sure only the lower 8 bits can be a 1
}

/*Serial configure line: configures the serial port to use 8 bits, no parity, one stop bit.*/
void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03); //Sends the command 0x03 to set 8 bits, no parity, one stop bit.
}

/* Serial configure FIFO: enables FIFO, clears the transmit and receive FIFO queues, sets queue length to 14 bytes. */
void serial_configure_FIFO(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7); //Sends the command 0xC7 to enable FIFO, clear both queues, set queue length to 14 bytes.
}

/* Serial configure modem: sets Ready To Transmit and Data Terminal Ready to on. */
void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03); //Sends the command 0x03 to set RTS and DTR to on.
}

/* Checks if the serial port transmit FIFO queue is empty. */
int serial_is_transmit_empty(unsigned short com)
{
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20; //Checks if the 5th bit of the line status register is set. If it is then it is empty
}

void init_serial(unsigned short com, unsigned short baud_divisor)
{
    serial_configure_baud_rate(com, baud_divisor); //Configures the baud rate with the given divisor
    serial_configure_line(com); //Configures the line to 8 bits, no parity, one stop bit
    serial_configure_FIFO(com); //Configures the FIFO
    serial_configure_modem(com); //Configures the modem
}

/* Writes a character to the serial port */
void serial_write_character(unsigned short com, unsigned char data) 
{
    while (serial_is_transmit_empty(com) == 0) //Loops around until it is empty
    {
        //Waits for the transmit FIFO queue to be empty
    }
    outb(SERIAL_DATA_PORT(com), data); //Writes the character to the data port

}

/* Writes an integer to the serial port */
void serial_write_integer(unsigned short com, unsigned int data) 
{
    while (serial_is_transmit_empty(com) == 0) //Loops around until the serial port is clear
    {
        //Waits for the transmit FIFO queue to be empty
    }
    outb(SERIAL_DATA_PORT(com), data); //Writes the integer to the data port
}

/* Writes a string to the serial port */
int serial_write_string(unsigned short com, const char* str) 
{
    for (int i =0; str[i] != '\0'; i++) //Loops through each character in the string until the null terminator is reached
    {
        serial_write_character(com, (unsigned char) str[i]); //Writes each character to the serial port
    }
    return 1; //Returns 1 when finsihed
}

/* Clears the screen by setting all cells to ' ' except the first position of each row which is a > */
void clear_screen() 
{
    for (unsigned int i = 0; i < 80 *25; i++) //Loops through all cells of the framebuffer (80 columns x 25 rows)
    {
        if (i % 80 == 0) //Checks if it is the first position of a row
        {
            framebuffer_write_cell(i, '|', FB_WHITE, BG_BLACK); //Writes a > character at the start of each row
        }
        else 
        {
        framebuffer_write_cell(i, ' ', 0, 0); //Writes a space character with fg black bg black) 
        }
    }
    framebuffer_move_cursor(1); //Moves the cursor to position 1 (after the first >)
}

/*Writes a character to the framebuffer */
void framebuffer_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;  // Address B8000 is used as a char pointer. 
    fb[i*2] = c; // Places character c at the relevant position in the framebuffer. Byte offset is 2 bytes.
    fb[i*2+1] = ((bg & 0x0F) << 4 | (fg & 0x0F)); //Performs a bitwise AND operation to ensure only the lower 4 bits of fg and bg are used. Shifts the background color 4 bits to the left to occupy the higher nibble. Combines the background and foreground colors using a bitwise OR operation. 
}

/*Writes a string to the framebuffer */
int fb_write_string(const char* str) 
{
    unsigned int i = 0;
    while (str[i] != '\0') { //Loops through each character in the string until the null terminator is reached
        if (cursor_position % 80 == 0) //Checks if at the start of a new line
        {
            cursor_position++; //Skips the first position of the new line (which is reserved for the > character)
        }
        framebuffer_write_cell(cursor_position, str[i], fg_colour, bg_colour);  //Writes the character at the current position
        framebuffer_move_cursor(cursor_position + 1); //Moves the cursor to the next position
        i++;
    }
    return 1;
   
}

/*Moves the cursor to a specific position */
void framebuffer_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND); //Sends command to set high byte of cursor position (expect the high 8 bits)
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF)); //Sends the high byte of the position. >> 8 shifts the bits right 8 places. & 0x00FF does bitwise AND to make sure only the lower 8 bits can be a 1
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

void set_background_color(unsigned int bg)
{
    bg_colour = bg;
}

void set_foreground_color(unsigned int fg)
{
    fg_colour = fg;
}

void newline()
{
    unsigned int new_position = ((cursor_position/80) + 1) * 80;
    if (new_position % 80 == 0)
    {
        new_position++;
    }
    framebuffer_move_cursor(new_position);
}

void io_wait(void)
{
    outb(0x80, 0);
}

int get_cursor_position()
{
    return cursor_position;
}

char framebuffer_read_cell(unsigned int i)
{
    char *fb = (char *) 0x000B8000;
    return fb[i * 2];  
}

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
    max = arg1;
    if (arg2 > max) 
        max = arg2;
    if (arg3 > max) 
        max = arg3;
    return max;
}