segment .text
global outb

    outb:
        mov al, [esp+8] ;Moves the data to be set to the al register
        mov dx, [esp+4] ;Moves the address from the I/O port to the dx register
        out dx, al  ;Writes a byte to the given I/O port
        ret

global inb

    inb:                  ;Returns a byte from the given I/O port
        mov dx, [esp + 4] ;Moves the address from the I/o port to the dx register
        in al, dx         ;Read a byte from the I/O port and stores it in the al register
        ret               ;Returns the read byte

