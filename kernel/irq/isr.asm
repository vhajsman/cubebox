.extern exception_handler

.macro isr_errStub a
isr_stub_\a:
	call exception_handler
	iret
.endm

.macro isr_no_errStub a
isr_stub_\a:
	call exception_handler
	iret
.endm

isr_no_errStub 0
isr_no_errStub 1
isr_no_errStub 2
isr_no_errStub 3
isr_no_errStub 4
isr_no_errStub 5
isr_no_errStub 6
isr_no_errStub 7
isr_errStub    8
isr_no_errStub 9
isr_errStub    10
isr_errStub    11
isr_errStub    12
isr_errStub    13
isr_errStub    14
isr_no_errStub 15
isr_no_errStub 16
isr_errStub    17
isr_no_errStub 18
isr_no_errStub 19
isr_no_errStub 20
isr_no_errStub 21
isr_no_errStub 22
isr_no_errStub 23
isr_no_errStub 24
isr_no_errStub 25
isr_no_errStub 26
isr_no_errStub 27
isr_no_errStub 28
isr_no_errStub 29
isr_errStub    30
isr_no_errStub 31



.global isr_stubTable
isr_stubTable:
.set i, 0
.rept 32
	dword isr_stub_\i
	.set i, i + 1
.endr


