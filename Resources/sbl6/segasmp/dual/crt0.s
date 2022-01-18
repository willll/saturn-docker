/* Based on Bart Trzynadlowski's CRT0.S
 * Public domain

 * Link this first, as you would do with regular, non-Saturn, crt0 object files
 */

.section .text

! The main entry point
.global ssStart
ssStart:
	mov.l	bss_start, r0 ! Clear the BSS
	mov.l	bss_end, r1
	mov		#0, r2
ssBSSClear:
	cmp/ge	r0, r1
	bt		ssBSSEnd
	add		#1, r0
	mov.b	r2, @r0
	bra		ssBSSClear
ssBSSEnd:
	mov.l	stack_ptr, r15	! The the stack pointer (0x06002000 - 0x06003FFF)
	mov.l	main_ptr, r0	! Go to main( )
	jsr		@r0
	nop
	mov.l	cart_ptr, r0	! Jump back to the cartridge start
	jmp		@r0
	nop
	nop
	mov		#0xF, r0	! It should be impossible to go beyond this point
	shll2	r0			! Disable interrupts and loop
	shll2	r0
	ldc		r0, sr
ssEnd:
	bra	ssEnd
	nop

cart_ptr:	.long	0x02000100
main_ptr:	.long	_main
stack_ptr:	.long	0x06004000
bss_start:	.long	__bss_start
bss_end:	.long	__bss_end

