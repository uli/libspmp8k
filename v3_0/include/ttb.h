#ifndef CYGONCE_TTB_H
#define CYGONCE_TTB_H
//==========================================================================
//
//      hal_platform_ints.h
//
//      HAL Interrupt and clock support
//
//==========================================================================

#include <cyg/infra/cyg_type.h>

//
#define ttb_base (0x00000000 + 0x060000)  //Update me to use defined param once known

// Set the TTB Table
#define TTB_SET()                                     									\
CYG_MACRO_START                                                         				\
    asm volatile (                                                          		\
    	"LDR     r3, ttb_base;"                                              			\
			"MRC     p15, 0, r0,c1,c0;"  	/*Get cp15 control register in R0*/					\
			"BIC     r0, r0, #1;"  				/*clear bit 1 - alignment fault checking*/	\
			"MCR     p15, 0, r0,c1,c0;"  	/*store updated control reg*/								\
			"MOV     r0, #0;"  						/*Load a 0 to begin invalidating*/					\
			"MCR     p15, 0, r0,c7,c7;"  	/*Invalidate dcache and icache*/						\
			"MCR     p15, 0, r0,c8,c7;"  	/*Invalidate TLB*/													\
			"MCR     p15, 0, r3,c2,c0;"  	/*Store 0x064011 as TTB*/										\
			"MOV     r0, #1;"  						/*get ready for client mode*/								\
			"MCR     p15, 0, r0,c3,c0;" 	/* Put Domain Access Ctr into client mode*/	\
        :                                                               				\
        :                                                               				\
        : "r0", "r3" /* Clobber list */                                       				\
        );                                                              				\
CYG_MACRO_END

#endif //CYGONCE_TTB_H
