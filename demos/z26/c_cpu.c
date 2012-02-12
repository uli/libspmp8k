/*

	6507 CPU emulator for z26


 This C file gets generated from cpu.m4, so please edit it there.

 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/

db dummy_flag;	/* used by ROL and ROR */
db dummy_high, dummy_low;	/* used by ADC and SBC */




/*
	the opcode definitions
*/

/* 00 - BRK implied */
void _00(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = reg_sp + 0x100;
	DataBus = ((reg_pc & 0xff00) >> 8);
	flag_B = 0x10;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = reg_pc & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = (flag_N | flag_V | 0x20 | flag_B | flag_D | flag_I | flag_Z | flag_C);
	flag_I = 0x04;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	if(TriggerWSYNC){
	 	// WSYNC only halts the CPU on read cycles
	 	TriggerWSYNC = 0;
		if(RClock > CYCLESPERSCANLINE){
	 		/* skip a line, if WSYNC came after end of line */
	 		Timer = Timer - (2 * CYCLESPERSCANLINE - RClock);	//clock RIOT
	 		RClock = 2 * CYCLESPERSCANLINE;	// and CPU clock
	 	}else{
	 		Timer = Timer - (CYCLESPERSCANLINE - RClock);	// clock RIOT
	 		RClock = CYCLESPERSCANLINE;	// and CPU clock
	 	}
	}
	reg_sp--;
	AddressBus = 0xfffe;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = DataBus;
	AddressBus = 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = (DataBus << 8) + reg_pc;
}

/* 01 - ORA (indirect,x) */
void _01(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 02 - JAM */

/* 03 - SLO (indirect,x) */
void _03(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 04 - NOP zeropage */
void _04(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 05 - ORA zeropage */
void _05(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 06 - ASL zeropage */
void _06(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 07 - SLO zeropage */
void _07(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 08 - PHP implied */
void _08(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	DataBus = (flag_N | flag_V | 0x20 | flag_B | flag_D | flag_I | flag_Z | flag_C);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	reg_sp--;
}

/* 09 - ORA immediate */
void _09(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 0a - ASL implied */
void _0a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
flag_C = (reg_a & 0x80) >> 7;
	reg_a = (reg_a << 1) & 0xfe;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 0b - ANC immediate */
void _0b(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	if(reg_a & 0x80) flag_C = 1;
	else flag_C = 0;
}

/* 0c - NOP absolute */
void _0c(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 0d - ORA absolute */
void _0d(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 0e - ASL absolute */
void _0e(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 0f - SLO absolute */
void _0f(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 10 - BPL relative */
void _10(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(!flag_N){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* 11 - ORA (indirect),y */
void _11(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 12 - JAM */

/* 13 - SLO (indirect),y */
void _13(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 14 - NOP zeropage,x */
void _14(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 15 - ORA zeropage,x */
void _15(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 16 - ASL zeropage,x */
void _16(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 17 - SLO zeropage,x */
void _17(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 18 - CLC implied */
void _18(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_C = 0;
}

/* 19 - ORA absolute,y */
void _19(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 1a - NOP implied */
void _1a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 1b - SLO absolute,y */
void _1b(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 1c - NOP absolute,x */
void _1c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 1d - ORA absolute,x */
void _1d(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 1e - ASL absolute,x */
void _1e(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 1f - SLO absolute,x */
void _1f(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 20 - JSR absolute */
void _20(void){
	
	db Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	DataBus = ((reg_pc & 0xff00) >> 8);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = reg_pc & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	if(TriggerWSYNC){
	 	// WSYNC only halts the CPU on read cycles
	 	TriggerWSYNC = 0;
		if(RClock > CYCLESPERSCANLINE){
	 		/* skip a line, if WSYNC came after end of line */
	 		Timer = Timer - (2 * CYCLESPERSCANLINE - RClock);	//clock RIOT
	 		RClock = 2 * CYCLESPERSCANLINE;	// and CPU clock
	 	}else{
	 		Timer = Timer - (CYCLESPERSCANLINE - RClock);	// clock RIOT
	 		RClock = CYCLESPERSCANLINE;	// and CPU clock
	 	}
	}
	reg_sp--;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = (DataBus << 8) + Address;
}

/* 21 - AND (indirect,x) */
void _21(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 22 - JAM */

/* 23 - RLA (indirect,x) */
void _23(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 24 - BIT zeropage */
void _24(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_V = (DataBus & 0x40);
	flag_N = (DataBus & 0x80);
	if((reg_a & DataBus) == 0) flag_Z = 0x02;
	else flag_Z = 0;
}

/* 25 - AND zeropage */
void _25(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 26 - ROL zeropage */
void _26(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 27 - RLA zeropage */
void _27(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 28 - PLP implied */
void _28(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_N = DataBus & 0x80;
	flag_V = DataBus & 0x40;
//	flag_B = DataBus & 0x10;
	flag_D = DataBus & 0x08;
	flag_I = DataBus & 0x04;
	flag_Z = DataBus & 0x02;
	flag_C = DataBus & 0x01;
}

/* 29 - AND immediate */
void _29(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 2a - ROL implied */
void _2a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (reg_a & 0x80) >> 7;
	reg_a = ((reg_a << 1) | dummy_flag) & 0xff;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 2b - ANC immediate */
void _2b(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	if(reg_a & 0x80) flag_C = 1;
	else flag_C = 0;
}

/* 2c - BIT absolute */
void _2c(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_V = (DataBus & 0x40);
	flag_N = (DataBus & 0x80);
	if((reg_a & DataBus) == 0) flag_Z = 0x02;
	else flag_Z = 0;
}

/* 2d - AND absolute */
void _2d(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 2e - ROL absolute */
void _2e(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 2f - RLA absolute */
void _2f(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 30 - BMI relative */
void _30(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(flag_N){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* 31 - AND (indirect),y */
void _31(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 32 - JAM */

/* 33 - RLA (indirect),y */
void _33(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 34 - NOP zeropage,x */
void _34(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 35 - AND zeropage,x */
void _35(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 36 - ROL zeropage,x */
void _36(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 37 - RLA zeropage,x */
void _37(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 38 - SEC implied */
void _38(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_C = 1;
}

/* 39 - AND absolute,y */
void _39(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 3a - NOP implied */
void _3a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 3b - RLA absolute,y */
void _3b(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 3c - NOP absolute,x */
void _3c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 3d - AND absolute,x */
void _3d(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 3e - ROL absolute,x */
void _3e(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 3f - RLA absolute,x */
void _3f(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 40 - RTI implied */
void _40(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_N = DataBus & 0x80;
	flag_V = DataBus & 0x40;
//	flag_B = DataBus & 0x10;
	flag_D = DataBus & 0x08;
	flag_I = DataBus & 0x04;
	flag_Z = DataBus & 0x02;
	flag_C = DataBus & 0x01;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = DataBus;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = (DataBus << 8) + reg_pc;
}

/* 41 - EOR (indirect,x) */
void _41(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 42 - JAM */

/* 43 - SRE (indirect,x) */
void _43(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 44 - NOP zeropage */
void _44(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 45 - EOR zeropage */
void _45(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 46 - LSR zeropage */
void _46(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 47 - SRE zeropage */
void _47(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 48 - PHA implied */
void _48(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
	reg_sp--;
}

/* 49 - EOR immediate */
void _49(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 4a - LSR implied */
void _4a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
flag_C = reg_a & 0x01;
	reg_a = (reg_a >> 1) & 0x7f;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 4b - ASR immediate */
void _4b(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a &= DataBus;
	flag_C = reg_a & 0x01;
	reg_a = (reg_a >> 1) & 0x7f;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 4c - JMP absolute */
void _4c(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	reg_pc = AddressBus;
}

/* 4d - EOR absolute */
void _4d(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 4e - LSR absolute */
void _4e(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 4f - SRE absolute */
void _4f(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 50 - BVC relative */
void _50(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(!flag_V){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* 51 - EOR (indirect),y */
void _51(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 52 - JAM */

/* 53 - SRE (indirect),y */
void _53(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 54 - NOP zeropage,x */
void _54(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 55 - EOR zeropage,x */
void _55(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 56 - LSR zeropage,x */
void _56(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 57 - SRE zeropage,x */
void _57(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 58 - CLI implied */
void _58(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_I = 0;
}

/* 59 - EOR absolute,y */
void _59(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 5a - NOP implied */
void _5a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 5b - SRE absolute,y */
void _5b(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 5c - NOP absolute,x */
void _5c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 5d - EOR absolute,x */
void _5d(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 5e - LSR absolute,x */
void _5e(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 5f - SRE absolute,x */
void _5f(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 60 - RTS implied */
void _60(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = DataBus;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = (DataBus << 8) + reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
}

/* 61 - ADC (indirect,x) */
void _61(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 62 - JAM */

/* 63 - RRA (indirect,x) */
void _63(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 64 - NOP zeropage */
void _64(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 65 - ADC zeropage */
void _65(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 66 - ROR zeropage */
void _66(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 67 - RRA zeropage */
void _67(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 68 - PLA implied */
void _68(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 69 - ADC immediate */
void _69(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 6a - ROR implied */
void _6a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = reg_a & 0x01;
	reg_a = ((reg_a >> 1) | dummy_flag) & 0xff;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 6b - ARR immediate */
void _6b(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		reg_a &= DataBus;
		reg_a = ((reg_a >> 1) | (flag_C << 7)) & 0xff;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
		flag_C = (reg_a & 0x40) >> 6;
		flag_V = (((reg_a & 0x40) ^ ((reg_a & 0x20) << 1))) & 0x40;
	}else{
		dummy_high = (reg_a & DataBus) >> 4;
		dummy_low = (reg_a & DataBus) & 0xf;
		flag_N = flag_C << 7;
		reg_a = (reg_a & DataBus) >> 1 | flag_N;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_V = (((reg_a & 0x40) ^ ((reg_a & 0x20) << 1))) & 0x40;
		if((dummy_low + (dummy_low & 1)) > 5) reg_a = (reg_a & 0xf0) | ((reg_a + 6) & 0xf);
		if((dummy_high + (dummy_high & 1)) > 5){
			reg_a = (reg_a + 0x60) & 0xff;
			flag_C = 1;
		}else flag_C = 0;
	}
}

/* 6c - JMP (indirect) */
void _6c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = DataBus;
	AddressBus = (AddressBus + 1) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = reg_pc + (DataBus << 8);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus & 0xff00) + ((AddressBus + 1) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = Address + (DataBus << 8);
	reg_pc = AddressBus;
}

/* 6d - ADC absolute */
void _6d(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 6e - ROR absolute */
void _6e(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 6f - RRA absolute */
void _6f(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 70 - BVS relative */
void _70(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(flag_V){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* 71 - ADC (indirect),y */
void _71(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 72 - JAM */

/* 73 - RRA (indirect),y */
void _73(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 74 - NOP zeropage,x */
void _74(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 75 - ADC zeropage,x */
void _75(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 76 - ROR zeropage,x */
void _76(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 77 - RRA zeropage,x */
void _77(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 78 - SEI implied */
void _78(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_I = 4;
}

/* 79 - ADC absolute,y */
void _79(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 7a - NOP implied */
void _7a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 7b - RRA absolute,y */
void _7b(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 7c - NOP absolute,x */
void _7c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 7d - ADC absolute,x */
void _7d(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
}

/* 7e - ROR absolute,x */
void _7e(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 7f - RRA absolute,x */
void _7f(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		if(((reg_a + DataBus + flag_C) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
		flag_N = (((dummy_high << 4) | (dummy_low & 0xf)) & 0x80);
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 80 - NOP immediate */
void _80(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 81 - STA (indirect,x) */
void _81(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 82 - NOP immediate */
void _82(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 83 - SAX (indirect),x */
void _83(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	DataBus = (reg_a & reg_x);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 84 - STY zeropage */
void _84(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	DataBus = reg_y;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 85 - STA zeropage */
void _85(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 86 - STX zeropage */
void _86(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	DataBus = reg_x;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 87 - SAX zeropage */
void _87(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	DataBus = (reg_a & reg_x);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 88 - DEY implied */
void _88(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y--;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* 89 - NOP immediate */
void _89(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* 8a - TXA implied */
void _8a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 8b - ANE immediate */
void _8b(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = ((reg_a | 0xee) & reg_x) & DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 8c - STY absolute */
void _8c(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	DataBus = reg_y;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 8d - STA absolute */
void _8d(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 8e - STX absolute */
void _8e(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	DataBus = reg_x;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 8f - SAX absolute */
void _8f(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	DataBus = (reg_a & reg_x);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 90 - BCC relative */
void _90(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(!flag_C){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* 91 - STA (indirect),y */
void _91(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 92 - JAM */

/* 93 - SHA (indirect),y */
void _93(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x & reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 94 - STY zeropage,x */
void _94(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	DataBus = reg_y;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 95 - STA zeropage,x */
void _95(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 96 - STX zeropage,y */
void _96(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_y) & 0xff);
	DataBus = reg_x;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 97 - SAX zeropage,y */
void _97(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_y) & 0xff);
	DataBus = (reg_a & reg_x);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 98 - TYA implied */
void _98(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_y;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* 99 - STA absolute,y */
void _99(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 9a - TXS implied */
void _9a(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_sp = reg_x;
}

/* 9b - SHS absolute,y */
void _9b(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x & reg_a;
	reg_sp = (reg_a & reg_x);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 9c - SHY absolute,x */
void _9c(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_y;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 9d - STA absolute,x */
void _9d(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	DataBus = reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 9e - SHX absolute,y */
void _9e(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* 9f - SHA absolute,y */
void _9f(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x & reg_a;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* a0 - LDY immediate */
void _a0(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = DataBus;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* a1 - LDA (indirect,x) */
void _a1(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* a2 - LDX immediate */
void _a2(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = DataBus;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* a3 - LAX (indirect,x) */
void _a3(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* a4 - LDY zeropage */
void _a4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = DataBus;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* a5 - LDA zeropage */
void _a5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* a6 - LDX zeropage */
void _a6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = DataBus;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* a7 - LAX zeropage */
void _a7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* a8 - TAY implied */
void _a8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = reg_a;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* a9 - LDA immediate */
void _a9(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* aa - TAX implied */
void _aa(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = reg_a;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* ab - LXA immediate */
void _ab(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = (reg_a | 0xee) & DataBus;
	reg_x = reg_a;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* ac - LDY absolute */
void _ac(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = DataBus;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* ad - LDA absolute */
void _ad(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* ae - LDX absolute */
void _ae(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = DataBus;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* af - LAX absolute */
void _af(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* b0 - BCS relative */
void _b0(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(flag_C){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* b1 - LDA (indirect),y */
void _b1(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* b2 - JAM */

/* b3 - LAX (indirect),y */
void _b3(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* b4 - LDY zeropage,x */
void _b4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = DataBus;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* b5 - LDA zeropage,x */
void _b5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* b6 - LDX zeropage,y */
void _b6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = DataBus;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* b7 - LAX zeropage,Y */
void _b7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* b8 - CLV implied */
void _b8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_V = 0;
}

/* b9 - LDA absolute,y */
void _b9(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* ba - TSX implied */
void _ba(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = reg_sp;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* bb - LAS absolute,y */
void _bb(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = reg_sp & DataBus;
	reg_sp = reg_a;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* bc - LDY absolute,x */
void _bc(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y = DataBus;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* bd - LDA absolute,x */
void _bd(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* be - LDX absolute,y */
void _be(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x = DataBus;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* bf - LAX absolute,y */
void _bf(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_a = reg_x = DataBus;
	if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
}

/* c0 - CPY immediate */
void _c0(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_y - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_y - DataBus) & 0xff) & 0x80);
	if(reg_y >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* c1 - CMP (indirect,x) */
void _c1(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* c2 - NOP immediate */
void _c2(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* c3 - DCP (indirect,x) */
void _c3(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* c4 - CPY zeropage */
void _c4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_y - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_y - DataBus) & 0xff) & 0x80);
	if(reg_y >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* c5 - CMP zeropage */
void _c5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* c6 - DEC zeropage */
void _c6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* c7 - DCP zeropage */
void _c7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* c8 - INY implied */
void _c8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_y++;
	if(reg_y == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_y & 0x80);
}

/* c9 - CMP immediate */
void _c9(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* ca - DEX implied */
void _ca(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x--;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* cb - SBX immediate */
void _cb(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!((reg_x & reg_a) < DataBus)) flag_C = 1;
	else flag_C = 0;
	reg_x = ((reg_x & reg_a) - DataBus) & 0xff;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* cc - CPY absolute */
void _cc(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_y - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_y - DataBus) & 0xff) & 0x80);
	if(reg_y >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* cd - CMP absolute */
void _cd(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* ce - DEC absolute */
void _ce(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* cf - DCP absolute */
void _cf(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* d0 - BNE relative */
void _d0(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(!flag_Z){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* d1 - CMP (indirect),y */
void _d1(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* d2 - JAM */

/* d3 - DCP (indirect),y */
void _d3(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* d4 - NOP zeropage,x */
void _d4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* d5 - CMP zeropage,x */
void _d5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* d6 - DEC zeropage,x */
void _d6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* d7 - DCP zeropage,x */
void _d7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* d8 - CLD implied */
void _d8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_D = 0;
}

/* d9 - CMP absolute,y */
void _d9(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* da - NOP implied */
void _da(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* db - DCP absolute,y */
void _db(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* dc - NOP absolute,x */
void _dc(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* dd - CMP absolute,x */
void _dd(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* de - DEC absolute,x */
void _de(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* df - DCP absolute,x */
void _df(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus--;
	if(((reg_a - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_a - DataBus) & 0xff) & 0x80);
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* e0 - CPX immediate */
void _e0(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_x - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_x - DataBus) & 0xff) & 0x80);
	if(reg_x >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* e1 - SBC (indirect,x) */
void _e1(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* e2 - NOP immediate */
void _e2(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* e3 - ISB (indirect,x) */
void _e3(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (AddressBus + reg_x) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* e4 - CPX zeropage */
void _e4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_x - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_x - DataBus) & 0xff) & 0x80);
	if(reg_x >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* e5 - SBC zeropage */
void _e5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* e6 - INC zeropage */
void _e6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* e7 - ISB zeropage */
void _e7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* e8 - INX implied */
void _e8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_x++;
	if(reg_x == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_x & 0x80);
}

/* e9 - SBC immediate */
void _e9(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* ea - NOP implied (this is the real one) */
void _ea(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* eb - SBC immediate */
void _eb(void){
		reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* ec - CPX absolute */
void _ec(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(((reg_x - DataBus) & 0xff) == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (((reg_x - DataBus) & 0xff) & 0x80);
	if(reg_x >= DataBus) flag_C = 1;
	else flag_C = 0;
}

/* ed - SBC absolute */
void _ed(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* ee - INC absolute */
void _ee(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* ef - ISB absolute */
void _ef(void){
	db Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* f0 - BEQ relative */
void _f0(void){
	
	int Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	if(flag_Z){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		}
		reg_pc = reg_pc + Address;
		
	}
}

/* f1 - SBC (indirect),y */
void _f1(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* f2 - JAM */

/* f3 - ISB (indirect),y */
void _f3(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = DataBus;
	reg_pc++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* f4 - NOP zeropage,x */
void _f4(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* f5 - SBC zeropage,x */
void _f5(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* f6 - INC zeropage,x */
void _f6(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* f7 - ISB zeropage,x */
void _f7(void){
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = DataBus;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = ((AddressBus + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* f8 - SED implied */
void _f8(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	flag_D = 8;
}

/* f9 - SBC absolute,y */
void _f9(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* fa - NOP implied */
void _fa(void){
	
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* fb - ISB absolute,y */
void _fb(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* fc - NOP absolute,x */
void _fc(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
}

/* fd - SBC absolute,x */
void _fd(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	if((Address + reg_x) > 0xff){
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	}
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
}

/* fe - INC absolute,x */
void _fe(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(DataBus == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (DataBus & 0x80);;
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}

/* ff - ISB absolute,x */
void _ff(void){
	dd Address;

	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + reg_x) & 0xff);
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_x) & 0xffff;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		if(reg_a == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (reg_a & 0x80);
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		if(dummy_low == 0) flag_Z = 0x02;
	else flag_Z = 0;
	flag_N = (dummy_low & 0x80);
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}
RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();
}


/*
	opcode vector table

 note:  the jam handler should be defined externally
        (since it is typically environment dependent)
*/

void (* vectors[256])(void) = {
	_00,_01,jam,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,_0e,_0f,
	_10,_11,jam,_13,_14,_15,_16,_17,_18,_19,_1a,_1b,_1c,_1d,_1e,_1f,
	_20,_21,jam,_23,_24,_25,_26,_27,_28,_29,_2a,_0b,_2c,_2d,_2e,_2f,	/* _2b=_0b */
	_30,_31,jam,_33,_34,_35,_36,_37,_38,_39,_3a,_3b,_3c,_3d,_3e,_3f,
	_40,_41,jam,_43,_44,_45,_46,_47,_48,_49,_4a,_4b,_4c,_4d,_4e,_4f,
	_50,_51,jam,_53,_54,_55,_56,_57,_58,_59,_5a,_5b,_5c,_5d,_5e,_5f,
	_60,_61,jam,_63,_64,_65,_66,_67,_68,_69,_6a,_6b,_6c,_6d,_6e,_6f,
	_70,_71,jam,_73,_74,_75,_76,_77,_78,_79,_7a,_7b,_7c,_7d,_7e,_7f,
	_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_8a,_8b,_8c,_8d,_8e,_8f,
	_90,_91,jam,_93,_94,_95,_96,_97,_98,_99,_9a,_9b,_9c,_9d,_9e,_9f,
	_a0,_a1,_a2,_a3,_a4,_a5,_a6,_a7,_a8,_a9,_aa,_ab,_ac,_ad,_ae,_af,
	_b0,_b1,jam,_b3,_b4,_b5,_b6,_b7,_b8,_b9,_ba,_bb,_bc,_bd,_be,_bf,
	_c0,_c1,_c2,_c3,_c4,_c5,_c6,_c7,_c8,_c9,_ca,_cb,_cc,_cd,_ce,_cf,
	_d0,_d1,jam,_d3,_d4,_d5,_d6,_d7,_d8,_d9,_da,_db,_dc,_dd,_de,_df,
	_e0,_e1,_e2,_e3,_e4,_e5,_e6,_e7,_e8,_e9,_ea,_e9,_ec,_ed,_ee,_ef,	/* _eb=_e9 */
	_f0,_f1,jam,_f3,_f4,_f5,_f6,_f7,_f8,_f9,_fa,_fb,_fc,_fd,_fe,_ff
};


/*
	Initialise the 6502/6507 CPU emulator
*/

void Init_CPU(void){

 	reg_pc = 0;
 	reg_a = 0;
 	flag_C = 0;
 	reg_x = 0;
 	reg_y = 0;
 	reg_sp = 0;
 	flag_Z = 0;
 	flag_N = 0;
 	flag_D = 0;
 	flag_V = 0;
 	flag_I = 0;
 	flag_B = 0x10;	/* B flag is always set exept when pushing the status byte after a hardware IRQ */
 	RCycles = 0;
}

/*
	Reset the CPU
*/

void Reset(void){

	/* stack pointer initializes to $01ff */
	reg_sp = 0xff;
	/* get reset vector from $fffc + $fffd */
	AddressBus = 0xfffc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc = DataBus;
	AddressBus++;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc += (DataBus << 8);
	RClock = 0;
}

/*
	do a single instruction
*/

void do_Instruction(void){

	if(TraceCount) TraceInstruction();
	RCycles = 0;
	AddressBus = reg_pc;
	RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
	reg_pc++;
	AddressBus = reg_pc;
	(* vectors[DataBus])();	/* do the instruction */
/*	ClockRIOT() is only used here, so we can include it directly */
	Timer -= RCycles;	/* This is all ClockRIOT() does */
}

void do_Instruction_line(void){

	while(RClock < CYCLESPERSCANLINE){
		if(TriggerWSYNC){
/*
	 	WSYNC only halts the CPU on read cycles
	 	except BRK and JSR we can handle it here,
	 	because the next read after the write is always an opcode fetch
*/
		 	TriggerWSYNC = 0;
//			if(RClock > CYCLESPERSCANLINE){
//	 			/* skip a line, if WSYNC came after end of line */
//	 			Timer = Timer - (2 * CYCLESPERSCANLINE - RClock);	//clock RIOT
//		 		RClock = 2 * CYCLESPERSCANLINE;	// and CPU clock
//		 	}else{
	 			Timer = Timer - (CYCLESPERSCANLINE - RClock);	// clock RIOT
	 			RClock = CYCLESPERSCANLINE;	// and CPU clock
//		 	}
		}

		if(TraceCount) TraceInstruction();
		RCycles = 0;
		AddressBus = reg_pc;
		RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();
		reg_pc++;
		AddressBus = reg_pc;
		(* vectors[DataBus])();	/* do the instruction */
	 /*	ClockRIOT() is only used here, so we can include it directly */
		Timer -= RCycles;	/* This is all ClockRIOT() does */
	}
}
