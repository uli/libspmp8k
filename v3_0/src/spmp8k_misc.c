//==========================================================================
//
//      SPMP8K_misc.c
//
//      HAL misc board support code for ARM9/SPMP8K
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    michael anburaj <michaelanburaj@hotmail.com>
// Contributors: michael anburaj <michaelanburaj@hotmail.com>
// Date:         2003-08-01
// Purpose:      HAL board support
// Description:  Implementations of HAL board interfaces
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/system.h>
#include CYGBLD_HAL_PLATFORM_H

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros

#include <cyg/hal/hal_io.h>             // IO macros
#include <cyg/hal/hal_arch.h>           // Register state info
#include <cyg/hal/hal_diag.h>
#include <cyg/hal/hal_intr.h>           // Interrupt names
#include <cyg/hal/hal_cache.h>
#include <cyg/hal/spmp8k.h>           // Platform specifics
//#include <cyg/hal/ttb.h>           // Platform specifics

#include <cyg/infra/diag.h>             // diag_printf

#include <string.h>                     // memset


//Protos
void plf_hardware_init(void);

// -------------------------------------------------------------------------
// MMU initialization:
// 
// These structures are laid down in memory to define the translation
// table.
// 

// ARM Translation Table Base Bit Masks
#define ARM_TRANSLATION_TABLE_MASK               0xFFFFC000

// ARM Domain Access Control Bit Masks
#define ARM_ACCESS_TYPE_NO_ACCESS(domain_num)    (0x0 << (domain_num)*2)
#define ARM_ACCESS_TYPE_CLIENT(domain_num)       (0x1 << (domain_num)*2)
#define ARM_ACCESS_TYPE_MANAGER(domain_num)      (0x3 << (domain_num)*2)

struct ARM_MMU_FIRST_LEVEL_FAULT {
    int id : 2;
    int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_FAULT_ID 0x0

struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE {
    int id : 2;
    int imp : 2;
    int domain : 4;
    int sbz : 1;
    int base_address : 23;
};
#define ARM_MMU_FIRST_LEVEL_PAGE_TABLE_ID 0x1

struct ARM_MMU_FIRST_LEVEL_SECTION {
    int id : 2;
    int b : 1;
    int c : 1;
    int imp : 1;
    int domain : 4;
    int sbz0 : 1;
    int ap : 2;
    int sbz1 : 8;
    int base_address : 12;
};
#define ARM_MMU_FIRST_LEVEL_SECTION_ID 0x2

struct ARM_MMU_FIRST_LEVEL_RESERVED {
    int id : 2;
    int sbz : 30;
};
#define ARM_MMU_FIRST_LEVEL_RESERVED_ID 0x3

#define ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, table_index) \
   (unsigned long *)((unsigned long)(ttb_base) + ((table_index) << 2))

#define ARM_FIRST_LEVEL_PAGE_TABLE_SIZE 0x4000

#define ARM_MMU_SECTION(ttb_base, actual_base, virtual_base,              \
                        cacheable, bufferable, perm)                      \
    CYG_MACRO_START                                                       \
        register union ARM_MMU_FIRST_LEVEL_DESCRIPTOR desc;               \
                                                                          \
        desc.word = 0;                                                    \
        desc.section.id = ARM_MMU_FIRST_LEVEL_SECTION_ID;                 \
        desc.section.imp = 1;                                             \
        desc.section.domain = 0;                                          \
        desc.section.c = (cacheable);                                     \
        desc.section.b = (bufferable);                                    \
        desc.section.ap = (perm);                                         \
        desc.section.base_address = (actual_base);                        \
        *ARM_MMU_FIRST_LEVEL_DESCRIPTOR_ADDRESS(ttb_base, (virtual_base)) \
                            = desc.word;                                  \
    CYG_MACRO_END

#define X_ARM_MMU_SECTION(abase,vbase,size,cache,buff,access)      \
    { int i; int j = abase; int k = vbase;                         \
      for (i = size; i > 0 ; i--,j++,k++)                          \
      {                                                            \
        ARM_MMU_SECTION(ttb_base, j, k, cache, buff, access);      \
      }                                                            \
    }

union ARM_MMU_FIRST_LEVEL_DESCRIPTOR {
    unsigned long word;
    struct ARM_MMU_FIRST_LEVEL_FAULT fault;
    struct ARM_MMU_FIRST_LEVEL_PAGE_TABLE page_table;
    struct ARM_MMU_FIRST_LEVEL_SECTION section;
    struct ARM_MMU_FIRST_LEVEL_RESERVED reserved;
};

#define ARM_UNCACHEABLE                         0
#define ARM_CACHEABLE                           1
#define ARM_UNBUFFERABLE                        0
#define ARM_BUFFERABLE                          1

#define ARM_ACCESS_PERM_NONE_NONE               0
#define ARM_ACCESS_PERM_RO_NONE                 0
#define ARM_ACCESS_PERM_RO_RO                   0
#define ARM_ACCESS_PERM_RW_NONE                 1
#define ARM_ACCESS_PERM_RW_RO                   2
#define ARM_ACCESS_PERM_RW_RW                   3




//----------------------------------------------------------------------------
// Platform specific initialization





// -------------------------------------------------------------------------
// Use Timer4 for system clock
void
hal_clock_initialize(cyg_uint32 period)
{
    cyg_uint32 temp;

    // Configure the Prescaler1
    HAL_READ_UINT32(TCFG0, temp);
    temp &= ~(0xff<<8);
    temp |= (CYGNUM_HAL_ARM_SPMP8K_TIMER_PRESCALE<<8);
    HAL_WRITE_UINT32(TCFG0, temp);

    // Configure the MUX to select the 1/2 divider
    HAL_READ_UINT32(TCFG1, temp);
    temp &= ~(0xf<<16);
    temp |= (0x0<<16);
    HAL_WRITE_UINT32(TCFG1, temp);
    
    // Set up the Timer4 for period
    HAL_WRITE_UINT32(TCNTB4, period);

    // Start Timer4
    HAL_READ_UINT32(TCON, temp);
    temp &= ~(0xf << 20);
    HAL_WRITE_UINT32(TCON, (temp|(6<<20)));
    HAL_WRITE_UINT32(TCON, (temp|(5<<20)));

    // Unmask Timer4 interrupt, need not be done here
    //HAL_INTERRUPT_CONFIGURE( CYGNUM_HAL_INTERRUPT_RTC, 1, 1 );
    //HAL_INTERRUPT_UNMASK( CYGNUM_HAL_INTERRUPT_RTC );
}

// This routine is called during a clock interrupt.
void
hal_clock_reset(cyg_uint32 vector, cyg_uint32 period)
{
    // Do nothing
}

// Read the current value of the clock, returning the number of hardware
// "ticks" that have occurred (i.e. how far away the current value is from
// the start)
void
hal_clock_read(cyg_uint32 *pvalue)
{
    cyg_int32 clock_val;

    // Read Timer4's current value
    HAL_READ_UINT32(TCNTO4, clock_val);

    *pvalue = CYGNUM_HAL_RTC_PERIOD - (clock_val & 0xFFFF);   // Note: counter is only 16 bits
                                                              // and decreases
}


// Delay for some number of micro-seconds
void 
hal_delay_us(cyg_int32 usecs)
{
    cyg_uint32 ticks = 0;
    // Divide by 1000000 in two steps to preserve precision.
    cyg_uint32 wait_ticks = (((PCLK/100000)*usecs)/CYGNUM_HAL_ARM_SPMP8K_TIMER_PRESCALE/2/10);
    cyg_int32 val, prev, diff;

    // Read Timer4's current value
    HAL_READ_UINT32(TCNTO4, prev);
    prev &= 0xFFFF;

    while (ticks < wait_ticks) {
        while (true) {
            // Read Timer4's current value
            HAL_READ_UINT32(TCNTO4, val);
            val &= 0xFFFF;

            diff = prev - val;
            if (diff != 0) {
                if(diff < 0)
                    diff += (CYGNUM_HAL_RTC_PERIOD+1);

                break;  // atleast 1 tick has passed
            } 
        }
        prev = val;
        ticks += diff;
    }
}

// -------------------------------------------------------------------------

// This routine is called to respond to a hardware interrupt (IRQ).  It
// should interrogate the hardware and return the IRQ vector number.
int 
hal_IRQ_handler(void)
{
    cyg_uint32 ior;
		cyg_uint32 i;

    HAL_READ_UINT32(INTOFFSET, ior);
    for(i=2;i<0x20;i++){
    	if(!(ior>>i)) return (int)ior;
    }
    return (int)ior;
}

//----------------------------------------------------------------------------
// Interrupt control

void
hal_interrupt_mask(int vector)
{
    cyg_uint32 imr;

    CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
               vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

    HAL_READ_UINT32(INTMSK, imr);
    imr |= (1<<vector);
    HAL_WRITE_UINT32(INTMSK, imr);
}

void
hal_interrupt_unmask(int vector)
{
    cyg_uint32 imr;

    CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
               vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

    HAL_READ_UINT32(INTMSK, imr);
    imr &= ~(1<<vector);
    HAL_WRITE_UINT32(INTMSK, imr);
}

void
hal_interrupt_acknowledge(int vector)
{
    cyg_uint32 ipr;

    CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
               vector >= CYGNUM_HAL_ISR_MIN , "Invalid vector");

    HAL_WRITE_UINT32(SRCPND, (1<<vector));
    HAL_READ_UINT32(INTPND, ipr);
    HAL_WRITE_UINT32(INTPND, ipr);
}

void
hal_interrupt_configure(int vector, int level, int up)
{
    CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
               vector >= CYGNUM_HAL_ISR_MIN, "Invalid vector");
}

void hal_interrupt_set_level(int vector, int level)
{
    CYG_ASSERT(vector <= CYGNUM_HAL_ISR_MAX &&
               vector >= CYGNUM_HAL_ISR_MIN, "Invalid vector");
}


//#define 

//-----------------------------------------------------------------------------
// End of SPMP8K_misc.c
//-----------------------------------------------------------------------------
//    	"LDR     r3, =0x060000;"                                \
//"r"(ttb_base)
// Verified Functions
void tlb_reset(void){
	//unsigned long* ttb_base = (0x00000000 + 0x060000);  //Update me to use defined param once known
    asm volatile (                                            			\
    	"LDR     r3, =0x60000;"                                			\
			"MRC     p15, 0, r0,c1,c0;" /*Get cp15 in R0*/								\
			"BIC     r0, r0, #1;"  			/*clear b1 - align fault chk*/		\
			"MCR     p15, 0, r0,c1,c0;" /*store updated control reg*/			\
			"MOV     r0, #0;"  					/*Load a 0 to begin invalidating*/\
			"MCR     p15, 0, r0,c7,c7;" /*Invalidate dcache and icache*/	\
			"MCR     p15, 0, r0,c8,c7;" /*Invalidate TLB*/								\
			"MCR     p15, 0, r3,c2,c0;" /*Store 0x064011 as TTB*/					\
			"MOV     r0, #1;"  					/*get ready for client mode*/			\
			"MCR     p15, 0, r0,c3,c0;" /*DA Ctrl -> client mode*/				\
        :                                                      			\
        :                                			\
        : "r0", "r3" /* Clobber list */                       			\
        );             
			
}

//-----------------------------------------------------------------------------
// In Progress Functions

void plf_hardware_init(void)
{
	tlb_reset();
	//scu_a_init
	//scu_b_init
	//scu_c_init
	//scu_d_init
	
	//TODO - these seem to be interrupt masks
	HAL_WRITE_UINT32(0x90010014, 0xffffffff);
	HAL_WRITE_UINT32(0x90020014, 0xffffffff);
	
	HAL_WRITE_UINT32(0x9001000C, 0);
	HAL_WRITE_UINT32(0x9002000C, 0);
	HAL_WRITE_UINT32(0x9001000C, 0);
	HAL_WRITE_UINT32(0x9002000C, 0);
	
	//Init_timers???(0x30d4)
}

void hal_hardware_init(void)
{
    // Perform any platform specific initializations
    plf_hardware_init();

    // Set up eCos/ROM interfaces
    hal_if_init();

#ifndef CYG_HAL_STARTUP_RAM
    // Invalidate caches
    HAL_DCACHE_INVALIDATE_ALL();
    HAL_ICACHE_INVALIDATE_ALL();
#endif
    // Enable caches
#ifdef CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP
    HAL_DCACHE_ENABLE();
#endif
#ifdef CYGSEM_HAL_ENABLE_ICACHE_ON_STARTUP
    HAL_ICACHE_ENABLE();
#endif
}





