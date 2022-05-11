all:
	x86_64-elf-gcc -c kernel.c -o kernel.o -ffreestanding -O2 -Wall -Wextra
	x86_64-elf-gcc -c video.c -o video.o -ffreestanding -O2
	x86_64-elf-gcc -c gdt.c -o gdt.o -ffreestanding -O2 -Wall -Wextra
	x86_64-elf-gcc -c idt.c -o idt.o -ffreestanding -O2 -Wall -Wextra -mgeneral-regs-only

	x86_64-elf-gcc kernel.o video.o gdt.o idt.o font.o -o kernel.elf -nostdlib -lgcc