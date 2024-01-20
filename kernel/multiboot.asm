/*
	This file contains everything around multiboot headers.
*/


/* --- multiboot header constants --- */
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)



/* --- multiboot header --- */
.section .multiboot
.align 4

.long MAGIC
.long FLAGS
.long CHECKSUM


/* --- allocation of a room for a stack --- */
.section .bss
.align 16

stack_bottom:
.skip 16384 # 16 KiB

stack_top: