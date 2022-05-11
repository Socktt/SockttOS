#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

typedef struct {
    uint16_t offset_low;
    uint16_t segment;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed)) idt_gate;

typedef struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed)) idtr_struct;

void prepare_idt();

#endif