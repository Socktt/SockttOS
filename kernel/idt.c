#include "idt.h"
#include "video.h"

idtr_struct idtr;
idt_gate idt[256] __attribute__((aligned(8)));

struct interrupt_frame;
 
__attribute__((interrupt)) void interrupt_handler(struct interrupt_frame* frame)
{
    println("\n-----\n\n\nInterrupt\n\n\n-----\n");
    for(;;){}
}

void setIdtEntryOffset(idt_gate* gate, uint64_t offset)
{
    gate->offset_low = (uint16_t)(offset & 0xffff);
    gate->offset_mid = (uint16_t)((offset & (0xffff << 16)) >> 16);
    gate->offset_high = (uint32_t)((offset & (0xffffffff << 32)) >> 32);
}

void prepare_idt()
{
    idtr.limit = sizeof(idt);
    idtr.offset = (uint64_t)idt;

    setIdtEntryOffset(&idt[0], interrupt_handler);
    idt[0].type_attr = IDT_TA_InterruptGate;
    idt[0].segment = 0x8;

    asm("lidt %0" : "+m" (idtr));
}