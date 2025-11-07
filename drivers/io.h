#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

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

// Defines the mapping for foreground colours    
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


void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void framebuffer_move_cursor(unsigned short pos);
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
void serial_configure_FIFO(unsigned short com);
void serial_configure_modem(unsigned short com);
int serial_is_transmit_empty(unsigned short com);
void serial_write_character(unsigned short com, unsigned char data);
void serial_write_integer(unsigned short com, unsigned int data);
int serial_write_string(unsigned short com, const char* str);
int fb_write_string(const char* str);
void framebuffer_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void clear_screen();
void printf(const char* str, unsigned int option);
void set_cursor_position(unsigned int x, unsigned int y);
void set_text_colour(unsigned int fg, unsigned int bg);
void newline();
void io_wait(void);
#endif