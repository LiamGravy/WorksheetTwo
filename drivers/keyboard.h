#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60

unsigned char keyboard_read_scancode(void);
unsigned char map_keyboard_scan_to_ascii(unsigned char scan_code);

#endif