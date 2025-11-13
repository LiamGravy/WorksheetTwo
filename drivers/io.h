#ifndef INCLUDE_IO_H 
#define INCLUDE_IO_H

#define FB_COMMAND_PORT 0x3D4 //This is the command port for FrameBuffer
#define FB_DATA_PORT 0x3D5 //This the data port for FrameBuffer

#define FB_HIGH_BYTE_COMMAND 14 //Command that is sent to the command port to tell it to expect the high byte of the cursor position in the data port 
#define FB_LOW_BYTE_COMMAND 15 //Command that is sent to the command port to tell it to expect the low byte of the cursor position in the data port

#define SERIAL_CON1_BASE 0x3F8 //Base address for COM1 serial port 
#define SERIAL_DATA_PORT(base) (base) //Sets the data port address to 0x3F8 
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2) //Sets the FIFO command port address to 0x3FA (0x3F8 + 2 = 0x3FA)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3) //Sets the line command port address to 0x3FB (0x3F8 + 3 = 0x3FB)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4) //Sets the modem command port address to 0x3FC (0x3F8 + 4 = 0x3FC)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5) //Sets the line status port address to 0x3FD (0x3F8 + 5 = 0x3FD)
#define SERIAL_LINE_ENABLE_DLAB 0x80 //Command for to enable DLAB (Divisor Latch Access Bit) basically telss the serial port to expect the first 8 bits (high) and then the last 8 bits (low) of the divisor

// Defines the mapping for foreground colours - Should be FG not FB but dont particularly want to change it
    #define FB_BLACK 0
    #define FB_BLUE 1  
    #define FB_GREEN 2 
    #define FB_CYAN 3
    #define FB_RED 4
    #define FB_MAGENTA 5
    #define FB_BROWN 6
    #define FB_LIGHT_GREY 7
    #define FB_DARK_GREY 8
    #define FB_LIGHT_BLUE 9
    #define FB_LIGHT_GREEN 10
    #define FB_LIGHT_CYAN 11
    #define FB_LIGHT_RED 12
    #define FB_LIGHT_MAGENTA 13
    #define FB_LIGHT_BROWN 14
    #define FB_WHITE 15

//Defines the mapping for background colours
    #define BG_BLACK 0
    #define BG_BLUE 1
    #define BG_GREEN 2
    #define BG_CYAN 3
    #define BG_RED 4
    #define BG_MAGENTA 5
    #define BG_BROWN 6
    #define BG_LIGHT_GREY 7
    #define BG_DARK_GREY 8
    #define BG_LIGHT_BLUE 9
    #define BG_LIGHT_GREEN 10
    #define BG_LIGHT_CYAN 11
    #define BG_LIGHT_RED 12
    #define BG_LIGHT_MAGENTA 13
    #define BG_LIGHT_BROWN 14
    #define BG_WHITE 15

/* I/O port functions */
void outb(unsigned short port, unsigned char data); //Writes a byte to a given port
unsigned char inb(unsigned short port); //Reads a byte from a given port


/*Framebuffer related functions*/
void framebuffer_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg); //Writes a character to the framebuffer (pos i, fg, bg)
char framebuffer_read_cell(unsigned int i); //Reads a character from the framebuffer at position i
int fb_write_string(const char* str); //Writes a string to the framebuffer
void set_text_colour(unsigned int fg, unsigned int bg); //Sets the text colour for framebuffer output (fg and bg)

void newline(); //Moves the cursor to the start of the next line
void clear_screen(); //Clears the framebuffer screen (by setting all cells to ' ')

void framebuffer_move_cursor(unsigned short pos); //Moves the cursor to a specific position
int get_cursor_position(); //Gets the current cursor position
void set_cursor_position(unsigned int x, unsigned int y); //Sets the cursor position to (x, y)

/*Serial Port related functions*/
void serial_configure_baud_rate(unsigned short com, unsigned short divisor); //Configures the baud rate of the serial port (com is the base address, the divisor sets the baud rate)

/*The serial port has a clock speed of 115200 bits per second. The divisor is used to divide this clock speed to achieve the desired baud rate. 
This is needed because different devices may require different baud rates for communication. */

void serial_configure_line(unsigned short com); //Configures the serial line command port (com is the base address) Configure line sets the data bits, stop bits and parity.
void serial_configure_FIFO(unsigned short com); //Configures the FIFO buffer of the serial port (com is the base address). (First in First Out)
void serial_configure_modem(unsigned short com); //Configures the modem command port (com is the base address). This sets up the modem control signals like RTS (Request to Send) and DSR (Data Set Ready).
int serial_is_transmit_empty(unsigned short com); //Checks if the transmit FIFO queue is empty (com is the base address)

void serial_write_character(unsigned short com, unsigned char data); //Writes a character to the serial port 
void serial_write_integer(unsigned short com, unsigned int data); //Writes an integer to the serial port
int serial_write_string(unsigned short com, const char* str); //Writes a string to the serial port

/* Function used by both serial and framebuffer */
void printf(const char* str, unsigned int option); //Custon Printf function to print to either serial or framebuffer based on option

/* Function used for PIC */
void io_wait(void); //Just a function to create a small delay for the PIC to process commands


#endif