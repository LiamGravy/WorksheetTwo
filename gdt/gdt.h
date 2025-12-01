
#ifndef GDT_H
#define GDT_H

struct gdt_pointer_struct 
{
    unsigned short limit; //The 16 bit size/limit of the GDT
    unsigned int base;  //The 32 bit base address of the GDT
} __attribute__((packed));

typedef struct gdt_pointer_struct gdt_ptr_t;

struct gdt_entry_struct 
{
    unsigned short limit_low;      //The lower 16 bits of the limit.
    unsigned short base_low;       //The lower 16 bits of the base.
    unsigned char  base_middle;    //The next 8 bits of the base.
    unsigned char  access;         //Access flags, determine what ring this segment can be used in.
    unsigned char  granularity;    //Granularity, and other flags.
    unsigned char  base_high;      //The last 8 bits of the base.
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t; 

extern void gdt_flush(unsigned int);

void init_gdt(void);
void gdt_set_gate(int index, unsigned int base_address, unsigned int limit, unsigned char access_byte, unsigned char gran);

#endif
