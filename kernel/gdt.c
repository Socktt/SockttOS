#include "gdt.h"

gdt_segment segments[NUM_SEG];

void create_gdt()
{
    segments[0] = create_segment(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    segments[1] = create_segment(0, 0xffffffff, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1);
    segments[2] = create_segment(0, 0xffffffff, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1);

    gdtr_struct gdtr;
    gdtr.offset = (uint64_t)&segments;
    gdtr.size = sizeof(segments);

    asm("lgdt %0" : "+m" (gdtr));
}

gdt_segment create_segment(uint32_t base, uint32_t limit, int p, int dpl, int s, int e, int dc, int rw, int a, int g, int db, int l)
{
    gdt_segment segment;
    segment.base_hi = (uint8_t)((base & 0xf000) >> 24);
    segment.base_mid = (uint8_t)((base & 0xf00) >> 16);
    segment.base_low = (uint16_t)(base & 0xff);

    segment.access = (p & 1) << 7 & (dpl & 0b11) << 5 & (s & 1) << 4 & (e & 1) << 3 & (dc & 1) << 2 & (rw & 1) << 1 & (a & 1);
    
    segment.flags_and_limit = (g & 1) << 7 & (db & 1) << 6 & (l & 1) << 5 & (limit & 0b11110000000000000000) >> 16;
    segment.limit_low = (uint16_t)(limit & 0xff);
    return segment;
}