#ifndef TIMER_INTERRUPT_H
#define TIMER_INTERRUPT_H

int get_timer_ticks();
void timer_interrupt_handler();
unsigned int readTimeStampCounter(void);
int get_cpu_clock();
int get_timer_ticks();

#endif