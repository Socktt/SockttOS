#ifndef GDT_H
#define GDT_H
#include <stdint.h>

#define NUM_SEG 3

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t flags_and_limit;
    uint8_t base_hi;
} __attribute__((packed)) gdt_segment;

typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) gdtr_struct;

gdt_segment create_segment(uint32_t base, uint32_t limit, int p, int dpl, int s, int e, int dc, int rw, int a, int g, int db, int l);
void create_gdt();

#endif