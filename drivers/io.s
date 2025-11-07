segment .text
global outb

    outb:
        mov al, [esp+8]
        mov dx, [esp+4]
        out dx, al
        ret

global inb

    inb:                  ;Returns a byte from the given I/O port
        mov dx, [esp + 4] ;Moves the address from the I/o port to the dx register
        in al, dx         ;Read a byte from the I/O port and stores it in the al register
        ret               ;Returns the read byte


