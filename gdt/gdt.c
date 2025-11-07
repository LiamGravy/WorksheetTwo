#include "gdt.h"

#define SEGMENT_DESCRIPTOR_COUNT 3

#define SEGMENT_BASE 0
#define SEGMENT_LIMIT 0xFFFFF

#define SEGMENT_CODE_TYPE 0x9A //Present, Privilege 0, Code Segment, Executable, Readable
#define SEGMENT_DATA_TYPE 0x92 //Present, Privilege 0, Data Segment, Writable

#define SEGMENT_GRANULARITY 0xCF //4KB Granularity, 32 bit protected mode

static gdt_entry_t gdt_entries[SEGMENT_DESCRIPTOR_COUNT];


void gdt_set_gate(int index, unsigned int base_address, unsigned int limit, unsigned char access_byte, unsigned char gran)
{
    gdt_entries[index].base_low = base_address & 0xFFFF;
    gdt_entries[index].base_middle = (base_address >> 16) & 0xFF;
    gdt_entries[index].base_high = (base_address >> 24) & 0xFF;
    gdt_entries[index].limit_low = limit & 0xFFFF;
    gdt_entries[index].access = access_byte;
    gdt_entries[index].granularity = (gran & 0xF0) | ((limit >> 16) & 0x0F);
}

void init_gdt()
{
    gdt_ptr_t gdt_ptr;
    gdt_ptr.limit = (sizeof(gdt_entry_t) * SEGMENT_DESCRIPTOR_COUNT) - 1;
    gdt_ptr.base = (unsigned int)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment

    gdt_set_gate(1, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_CODE_TYPE, SEGMENT_GRANULARITY); // Kernel Code Segment
    gdt_set_gate(2, SEGMENT_BASE, SEGMENT_LIMIT, SEGMENT_DATA_TYPE, SEGMENT_GRANULARITY); // Kernel Data Segment

    gdt_flush((unsigned int)&gdt_ptr);
}