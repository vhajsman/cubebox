.section .text

.global memory_getSize
memory_getSize:
	int 0x12
	jc .error

	test ax, ax
	je .error

	cmp ah, 0x86
	je .error

	cmp ah, 0x80
	je .error

	ret
.error
	mov ax, -1
	ret

.global memory_getExtended
memory_getExtended:
	mov ax, 0x88

	int 0x15
	jc .error
	
	test ax, ax
	je .error

	cmp ah, 0x86
	je .error

	cmp ah, 0x80
	je .error

	ret
.error
	mov ax, -1
	ret
