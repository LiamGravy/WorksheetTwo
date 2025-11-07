#include "../drivers/io.h"

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

#define ICW1_INIT       0x11    // 0x10 is the initialisation bit. 0x01 indicates that ICW4 will be used. So combined gives 0x11
#define MASTER_OFFSET   0x20    // Remap Master PIC to interrupt vector 0x20 (32)
#define SLAVE_OFFSET    0x28    // Remap Slave PIC to interrupt vector
#define SET_MODE_x86    0x01    // Set to 8086 mode

void pic_remap(void)
{
    /* ICW1: Initialisation
       0x11 */
    outb(PIC1_COMMAND_PORT, ICW1_INIT); //Sends the initialisation command to the Master PIC (0x11)
    io_wait(); //Waits a tiny amount of time to ensure the PIC has time to process the command (its slow apparently)
    outb(PIC2_COMMAND_PORT, ICW1_INIT); //Sends the initialisation command to the Slave PIC (0x11)
    io_wait();

    /* We now need to remap the PICs to avoid conflicts with CPU exceptions.
       We will map the Master PIC to interrupt vector 0x20 (32) and the Slave PIC to 0x28 (40) */

    outb(PIC1_DATA_PORT, MASTER_OFFSET); //Remap Master PIC to interrupt vector 0x20
    io_wait();
    outb(PIC2_DATA_PORT, SLAVE_OFFSET);  //Remap Slave PIC to interrupt vector 0x28
    io_wait();

    /* We now need to tell the Master Pic that it has a slave, We then need to give the slave a cascade identity 
    which is basically an ID number so the Master PIC knows what slave a interrupt came from.
    We will tell the Master there is a slave at IRQ2 - 0000 0100 or (4) this looks odd because the master uses a bitmap which starts at 0. So the second IRQ is actually in the 3rd
    The slave doesnt use a bitmap so we can just send in binary 2 0000 0010*/
    
    outb(PIC1_DATA_PORT, 0x04); //Tells Master PIC there is a slave at IRQ2
    io_wait();
    outb(PIC2_DATA_PORT, 0x02); //Gives the Slave PIC a cascade identity of 2
    io_wait();

    /* Now we need to set the Master and Slave to use 8086 mode since we are using x86 assembly architecture. The default is 8080 which is for an older chip*/

    outb(PIC1_DATA_PORT, SET_MODE_x86); //Set Master PIC to 8086 mode
    io_wait();
    outb(PIC2_DATA_PORT, SET_MODE_x86); //Set Slave PIC to 8086 mode
    io_wait();

    /* Finally we need to mask all the IRQs until we build handlers for them all. We will unmask them one by one when we write handlers */

    outb(PIC1_DATA_PORT, 0xFF); //Mask all 8 IRQs on Master PIC
    io_wait();
    outb(PIC2_DATA_PORT, 0xFF); //Mask all 8 IRQs on Slave PIC
    io_wait();
}   

#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7
#define PIC_ACKNOWLEGE 0x20

void pic_acknowledge_interrupt(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT)
    {
        return; //Not a PIC interrupt
    }

    if (interrupt < PIC2_START_INTERRUPT)
    {
        outb(PIC1_COMMAND_PORT, PIC_ACKNOWLEGE); //Acknowledge Master PIC
    }
    else
    {
        outb(PIC2_COMMAND_PORT, PIC_ACKNOWLEGE); //Acknowledge Slave PIC
    }
}