#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60 // I/O port for keyboard data
#define EXIT_PORT 0xf4       // The port to exit QEMU
#define EXIT_CODE 0x00      // The code to send to exit QEMU
#define ESC 27              // ASCII code for the escape key

extern volatile int EnterPressed; // Flag to indicate if a keyboard interrupt has occurred

unsigned char keyboard_read_scancode(void); //Reads a scancode from the keyboard data port
unsigned char map_keyboard_scan_to_ascii(unsigned char scan_code); //Maps scan codes to ASCII characters

void buffer_keyboard_input(unsigned char scancode); //Buffers keyboard input characters
unsigned char keyboard_pop_head_char(void); //Gets a character from the keyboard buffer
void keyboard_interrupt(void); //Handles keyboard interrupts
#endif