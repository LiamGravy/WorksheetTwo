#include "timerInterrupt.h"
#include "../drivers/io.h"
#include "../idt/idt.h"
#include "../idt/pic.h"

volatile unsigned int timer_ticks = 0;


void timer_interrupt_handler()
{
    timer_ticks++;
    pic_acknowledge_interrupt(32); 
    return;
}

int get_cpu_clock()
{
    unsigned int cpuClock = readTimeStampCounter();
    // fb_write_integer(cpuClock);
    return cpuClock;
}

int get_timer_ticks()
{
    return timer_ticks;
}

