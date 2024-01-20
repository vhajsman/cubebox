
i686-elf-as kernel/multiboot.asm	-o build/obj/multiboot.asm
i686-elf-as kernel/entry.asm		-o build/obj/entry.asm

i686-elf-gcc -c drivers/terminal.c -o build/obj/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c kernel/kmain.c -o build/obj/kmain.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-gcc -T linker.ld -o build/binaries/kernel.bin -ffreestanding -O2 -nostdlib build/obj/* -lgcc
