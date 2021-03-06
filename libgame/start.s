

	.align	4
	.text

	.arm

	.global		start
	.global		_sbrk_asm
	.global		_exit

	.global		_gFunTable
	.global		__heap_end_asm

	.extern		main
	.extern		_end
	.extern		_bss_start
	.extern		_bss_end


start:
	stmfd	sp!,{r0-r12, lr}
	ldr		r1,=_gFunTable
	str		r0, [r1]			// store the pointer to the function table 
	
	ldr		r1,=_save_stack
	str		sp, [r1]
	
	// zero out the bss section
	ldr		r1,=_bss_start
	ldr		r2,=_bss_end
	mov		r0, #0
bss_zero:
	str		r0, [r1], #4
	cmp		r1, r2
	bls		bss_zero

	ldr		r0, .Lfini
	bl		atexit
	bl		_libgame_init
	bl		__libc_init_array
	bl		main				// enter the show

	b		exit

	.align	4
_gFunTable:
	.word	0

	.align	4
_save_stack:
	.word	0

	.align	4
__heap_end_asm:
	.word	0

_sbrk_asm:
	stmfd	sp!,{r1-r3, lr}
	ldr		r1, =__heap_end_asm
	ldr		r2, [r1]
	subs	r2, r2, #0
	bne		_sbrk_asm_L1		// if __heap_end_asm == 0
	ldr		r2, =_end			//   __heap_end_asm = &_end
	str		r2, [r1]

_sbrk_asm_L1:
	ldr		r2, [r1]			// r2 = __heap_end_asm
	add		r3, r2, r0			// r3 = __heap_end_asm + incr
	str		r3, [r1]			// __heap_end_asm = r3
	mov		r0, r2				// return __heap_end_asm
	ldmia	sp!,{r1-r3, pc}
	nop

_exit:
	ldr		r1,=_save_stack
	ldr		sp, [r1]
	ldmia	sp!,{r0-r12, pc}

.global __dso_handle
.weak __dso_handle
__dso_handle: .word 0

.global __aeabi_unwind_cpp_pr0
.weak __aeabi_unwind_cpp_pr0
__aeabi_unwind_cpp_pr0:
.global _init
.weak   _init
_init:
.global _fini
.weak   _fini
_fini:
	bx lr
.Lfini:
	.word __libc_fini_array
