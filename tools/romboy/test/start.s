

	.text
	.align	4

	.global	start
	
	.global	uart_putc
	.global	uart_puts


// ---------------------------------------------------------------------
//  void start(void)
//
//  entry point
// ---------------------------------------------------------------------
start:
	stmfd	sp!, {r0-r12, lr}	// save all regs except sp and pc
	bl		main
	ldmfd	sp!, {r0-r12, pc}


// ---------------------------------------------------------------------
//  void uart_putc(char c)
//
// ---------------------------------------------------------------------
uart_putc:
	mov		r1, #0x92000000
	add		r1, r1, #0xB00000
	add		r1, r1, #0x4000
	
uart_putc_wait:
	ldr		r2, [r1, #0x20]
	tst		r2, #4
	bne		uart_putc_wait

	str		r0, [r1]
	bx		lr

// ---------------------------------------------------------------------
//  void uart_puts(char *str)
//
// ---------------------------------------------------------------------
uart_puts:
	mov		r1, #0x92000000
	add		r1, r1, #0xB00000
	add		r1, r1, #0x4000
	mov		r2, #0
	
uart_puts_lp:
	ldrb	r3, [r0, r2]
	cmp		r3, #0
	add		r2, r2, #1
	beq		uart_puts_done
	str		r3, [r1]
	b		uart_puts_lp

uart_puts_done:
	bx		lr

