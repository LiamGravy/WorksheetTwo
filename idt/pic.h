#ifndef INCLUDE_PIC_H
#define INCLUDE_PIC_H

#define MASTER_PIC_COMMAND_PORT 0x20 //Base address for Master PIC
#define MASTER_PIC_DATA_PORT 0x21 
#define SLAVE_PIC_COMMAND_PORT 0xA0 //Base address for Slave PIC
#define SLAVE_PIC_DATA_PORT 0xA1

void pic_remap(void); //Remaps the PIC to avoid conflicts with CPU exceptions
void pic_acknowledge_interrupt(unsigned int interrupt); //Acknowledges the PIC that an interrupt has been handled

#endif
