

	.align	4
	.text

	.arm

	.global		start
	.global		ftab

	.extern		main


start:
	stmfd	sp!,{r0-r12, lr}

	ldr		r1,=ftab
	str		r0, [r1]			// store the pointer to the function table 

	bl		main				// enter the show
/*
	ldr		r2,=ftab
	ldr		r1, [r2]
	ldr		r1, [r1, #4]
	ldr		r0,=bye

	mov		lr, pc
	bx		r1
	nop

	ldr		r2,=ftab
	ldr		r1, [r2]
	ldr		r1, [r1, #4]
	ldr		r0,=bye

	mov		lr, pc
	bx		r1
	nop
*/
	ldmia	sp!,{r0-r12, pc}

ftab:
	.word	0

	.align	4
bye:
	.ascii	"Bye!\n\0"

/*
	ldr		r2,=ftab
	ldr		r1, [r2]
	ldr		r0,=bye
	mov		lr, pc
	ldr		pc, [r1, #4]		// dprintf("Bye!");
	nop
*/

