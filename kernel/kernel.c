#include <stdint.h>
#include "video.h"
#include "gdt.h"
#include "idt.h"

void _start(void * fb, long int ppsl)
{
    create_gdt();
    terminit(fb, ppsl);

    println("Among us!\n");
    println("This is a cool test indeed\n");
    println("NO DONT DO IT PLEASE\n");

    prepare_idt();

    int a = 1 / 0;
}