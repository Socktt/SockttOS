#include <stdint.h>
#include "video.h"

void _start(void * fb, long int ppsl)
{
    terminit(fb, ppsl);

    println("Among us!\n");
    println("This is a cool test indeed\n");
}