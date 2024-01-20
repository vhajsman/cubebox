.section .text

.global _start
.type _start, @function
_start:
	/* --- stack initialization ---*/
	mov $stack_top, %esp



	/* Initialize crucial processor state before the high-level kernel is entered here */

	call gdt_init
	call reloadSegments


	/* --- Enter the high-level kernel --- */
	
	call kmain


	/* --- CPU halt --- */
	
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
