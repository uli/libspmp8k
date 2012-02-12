changecom(`/*',`*/')dnl
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

define(TODO,`/* program me please */')dnl
define(bumpclock,`RClock++;	/* cycles for this scanline */
	RCycles++;	/* cycles for this instruction */')dnl
define(readbank,`bumpclock
/* read value at address in AddressBus to DataBus */
	(* ReadAccess[AddressBus])();')dnl
define(writebank,`bumpclock
/* write value in DataBus to address in AddressBus */
	(* WriteAccess[AddressBus])();')dnl
define(opcode,`void $1(void){
	$2
	$3
}')dnl
define(adr_implied,`')dnl
define(adr_immediate,`	reg_pc++;')dnl
define(adr_absolute,`db Address;

	readbank
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	readbank
	AddressBus = (DataBus << 8) + Address;
	reg_pc++;')dnl
define(adr_zeropage,`readbank
	reg_pc++;
	AddressBus = DataBus;')dnl
define(adr_zeropage_index,`readbank
	reg_pc++;
	AddressBus = DataBus;
	readbank
	AddressBus = ((AddressBus + $1) & 0xff);')dnl
define(adr_absolute_index_write,`dd Address;

	readbank
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	readbank
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + $1) & 0xff);
	readbank
	AddressBus = (((AddressBus & 0xff00) + Address) + $1) & 0xffff;')dnl
define(adr_absolute_index_read,`dd Address;

	readbank
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_pc;
	readbank
	reg_pc++;
	AddressBus = (DataBus << 8) + ((Address + $1) & 0xff);
	if((Address + $1) > 0xff){
		readbank
		AddressBus = (((AddressBus & 0xff00) + Address) + $1) & 0xffff;
	}')dnl
define(adr_index_indirect,`db Address;

	readbank
	AddressBus = DataBus;
	reg_pc++;
	readbank
	AddressBus = (AddressBus + reg_x) & 0xff;
	readbank
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	readbank
	AddressBus = (DataBus << 8) + Address;')dnl
define(adr_indirect_index_read,`dd Address;

	readbank
	AddressBus = DataBus;
	reg_pc++;
	readbank
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	readbank
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	if((Address + reg_y) > 0xff){
		readbank
		AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;
	}')dnl
define(adr_indirect_index_write,`dd Address;

	readbank
	AddressBus = DataBus;
	reg_pc++;
	readbank
	Address = DataBus;
	AddressBus = (AddressBus + 1) & 0xff;
	readbank
	AddressBus = (DataBus << 8) + ((Address + reg_y) & 0xff);
	readbank
	AddressBus = (((AddressBus & 0xff00) + Address) + reg_y) & 0xffff;')dnl
define(adr_relative,`int Address;

	readbank
	reg_pc++;
	if($1){
		if(DataBus & 0x80) Address = (0x100 - DataBus) * -1;
		else Address = DataBus;
		AddressBus++;
		readbank
		if(((reg_pc + Address) & 0xff00) != (reg_pc & 0xff00)){
			AddressBus = (reg_pc & 0xff00) + ((reg_pc + Address) & 0xff);
			readbank
		}
		reg_pc = reg_pc + Address;
		
	}')dnl
define(adr_absolute_indirect,`dd Address;

	readbank
	reg_pc = DataBus;
	AddressBus = (AddressBus + 1) & 0xffff;
	readbank
	AddressBus = reg_pc + (DataBus << 8);
	readbank
	Address = DataBus;
	AddressBus = (AddressBus & 0xff00) + ((AddressBus + 1) & 0xff);
	readbank
	AddressBus = Address + (DataBus << 8);')dnl
define(rmw_ram,`readbank
writebank
$1
writebank')dnl
define(rmw_accu,`readbank
$1')
define(adr_push,`readbank
	AddressBus = reg_sp + 0x100;
	DataBus = $1;
	writebank
	reg_sp--;')
define(adr_pull,`readbank
	AddressBus = reg_sp + 0x100;
	readbank
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank')dnl
define(test_z,`if($1 == 0) flag_Z = 0x02;
	else flag_Z = 0;')dnl
define(test_n,`flag_N = ($1 & 0x80);')dnl
define(test_v,`flag_V = ($1 & 0x40);')dnl
define(test_zn,`test_z($1)
	test_n($1)')dnl
define(ADC,`readbank
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		test_zn(reg_a)
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		test_z(((reg_a + DataBus + flag_C) & 0xff))
		test_n(((dummy_high << 4) | (dummy_low & 0xf)))
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}')dnl
define(AND,`readbank
	reg_a &= DataBus;
	test_zn(reg_a)')dnl
define(ANC,`AND
	if(reg_a & 0x80) flag_C = 1;
	else flag_C = 0;')dnl
define(ANE,`readbank
	reg_a = ((reg_a | 0xee) & reg_x) & DataBus;
	test_zn(reg_a)')dnl
define(ARR,`readbank
	if(!flag_D){
		reg_a &= DataBus;
		reg_a = ((reg_a >> 1) | (flag_C << 7)) & 0xff;
		test_zn(reg_a)
		flag_C = (reg_a & 0x40) >> 6;
		flag_V = (((reg_a & 0x40) ^ ((reg_a & 0x20) << 1))) & 0x40;
	}else{
		dummy_high = (reg_a & DataBus) >> 4;
		dummy_low = (reg_a & DataBus) & 0xf;
		flag_N = flag_C << 7;
		reg_a = (reg_a & DataBus) >> 1 | flag_N;
		test_z(reg_a)
		flag_V = (((reg_a & 0x40) ^ ((reg_a & 0x20) << 1))) & 0x40;
		if((dummy_low + (dummy_low & 1)) > 5) reg_a = (reg_a & 0xf0) | ((reg_a + 6) & 0xf);
		if((dummy_high + (dummy_high & 1)) > 5){
			reg_a = (reg_a + 0x60) & 0xff;
			flag_C = 1;
		}else flag_C = 0;
	}')dnl
define(ASL,`flag_C = ($1 & 0x80) >> 7;
	$1 = ($1 << 1) & 0xfe;
	test_zn($1)')dnl
define(ASR,`readbank
	reg_a &= DataBus;
	flag_C = reg_a & 0x01;
	reg_a = (reg_a >> 1) & 0x7f;
	test_zn(reg_a)')dnl
define(BCC,`adr_relative(`!flag_C')')dnl
define(BCS,`adr_relative(`flag_C')')dnl
define(BEQ,`adr_relative(`flag_Z')')dnl
define(BIT,`readbank
	test_v(DataBus)
	test_n(DataBus)
	test_z(`(reg_a & DataBus)')')dnl
define(BNE,`adr_relative(`!flag_Z')')dnl
define(BMI,`adr_relative(`flag_N')')dnl
define(BPL,`adr_relative(`!flag_N')')dnl
define(BRK,`readbank
	reg_pc++;
	AddressBus = reg_sp + 0x100;
	DataBus = ((reg_pc & 0xff00) >> 8);
	flag_B = 0x10;
	writebank
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = reg_pc & 0xff;
	writebank
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = (flag_N | flag_V | 0x20 | flag_B | flag_D | flag_I | flag_Z | flag_C);
	flag_I = 0x04;
	writebank
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
	readbank
	reg_pc = DataBus;
	AddressBus = 0xffff;
	readbank
	reg_pc = (DataBus << 8) + reg_pc;')dnl
define(BVC,`adr_relative(`!flag_V')')dnl
define(BVS,`adr_relative(`flag_V')')dnl
define(CLC,`readbank
	flag_C = 0;')dnl
define(CLD,`readbank
	flag_D = 0;')dnl
define(CLI,`readbank
	flag_I = 0;')dnl
define(CLV,`readbank
	flag_V = 0;')dnl
define(CMP,`readbank
	test_zn(`((reg_a - DataBus) & 0xff)')
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;')dnl 
define(CPX,`readbank
	test_zn(`((reg_x - DataBus) & 0xff)')
	if(reg_x >= DataBus) flag_C = 1;
	else flag_C = 0;')dnl
define(CPY,`readbank
	test_zn(`((reg_y - DataBus) & 0xff)')
	if(reg_y >= DataBus) flag_C = 1;
	else flag_C = 0;')dnl 
define(DCP,`DataBus--;
	test_zn(`((reg_a - DataBus) & 0xff)')
	if(reg_a >= DataBus) flag_C = 1;
	else flag_C = 0;')dnl 
define(DEC,`DataBus--;
	test_zn(DataBus);')dnl
define(DEX,`readbank
	reg_x--;
	test_zn(reg_x)')dnl
define(DEY,`readbank
	reg_y--;
	test_zn(reg_y)')dnl
define(EOR,`readbank
	reg_a ^= DataBus;
	test_zn(reg_a)')dnl
define(INC,`DataBus++;
	test_zn(DataBus);')dnl
define(INX,`readbank
	reg_x++;
	test_zn(reg_x)')dnl
define(INY,`readbank
	reg_y++;
	test_zn(reg_y)')dnl
define(ISB,`DataBus++;
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		test_zn(reg_a)
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		test_zn(dummy_low)
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}')dnl
define(JMP,`reg_pc = AddressBus;')dnl
define(JSR,`db Address;
	readbank
	Address = DataBus;
	reg_pc++;
	AddressBus = reg_sp + 0x100;
	readbank
	DataBus = ((reg_pc & 0xff00) >> 8);
	writebank
	reg_sp--;
	AddressBus = reg_sp + 0x100;
	DataBus = reg_pc & 0xff;
	writebank
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
	readbank
	reg_pc = (DataBus << 8) + Address;')dnl
define(LAS,`readbank
	reg_a = reg_x = reg_sp & DataBus;
	reg_sp = reg_a;
	test_zn(reg_a)')dnl
define(LAX,`readbank
	reg_a = reg_x = DataBus;
	test_zn(reg_a)')dnl
define(LDA,`readbank
	reg_a = DataBus;
	test_zn(reg_a)')dnl
define(LDX,`readbank
	reg_x = DataBus;
	test_zn(reg_x)')dnl
define(LDY,`readbank
	reg_y = DataBus;
	test_zn(reg_y)')dnl
define(LSR,`flag_C = $1 & 0x01;
	$1 = ($1 >> 1) & 0x7f;
	test_zn($1)')dnl
define(LXA,`readbank
	reg_a = (reg_a | 0xee) & DataBus;
	reg_x = reg_a;
	test_zn(reg_a)')dnl
define(NOP,`readbank')dnl
define(ORA,`readbank
	reg_a |= DataBus;
	test_zn(reg_a)')dnl
define(PHA,`adr_push(reg_a)')dnl
define(PHP,`adr_push(`(flag_N | flag_V | 0x20 | flag_B | flag_D | flag_I | flag_Z | flag_C)')')dnl
define(PLA,`adr_pull
	reg_a = DataBus;
	test_zn(reg_a)')dnl
define(PLP,`adr_pull
	flag_N = DataBus & 0x80;
	flag_V = DataBus & 0x40;
//	flag_B = DataBus & 0x10;
	flag_D = DataBus & 0x08;
	flag_I = DataBus & 0x04;
	flag_Z = DataBus & 0x02;
	flag_C = DataBus & 0x01;')dnl
define(RLA,`dummy_flag = flag_C;
	flag_C = (DataBus & 0x80) >> 7;
	DataBus = ((DataBus << 1) | dummy_flag) & 0xff;
	reg_a &= DataBus;
	test_zn(reg_a)')dnl
define(ROL,`dummy_flag = flag_C;
	flag_C = ($1 & 0x80) >> 7;
	$1 = (($1 << 1) | dummy_flag) & 0xff;
	test_zn($1)')dnl
define(ROR,`dummy_flag = flag_C << 7;
	flag_C = $1 & 0x01;
	$1 = (($1 >> 1) | dummy_flag) & 0xff;
	test_zn($1)')dnl
define(RRA,`dummy_flag = flag_C << 7;
	flag_C = DataBus & 0x01;
	DataBus = ((DataBus >> 1) | dummy_flag) & 0xff;
	if(!flag_D){
		if( (((reg_a + DataBus + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + DataBus + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + DataBus + flag_C) & 0xff);
		flag_C = dummy_flag;
		test_zn(reg_a)
	}else{
		dummy_low = (reg_a & 0xf) + (DataBus & 0xf) + flag_C;
		dummy_high = ((reg_a & 0xf0) >> 4) + ((DataBus & 0xf0) >> 4);	// + ((dummy_low & 0x10) >> 4);
		if(dummy_low > 9) dummy_low += 6;
		if (dummy_low > 0xf) dummy_high++;
		test_z(((reg_a + DataBus + flag_C) & 0xff))
		test_n(((dummy_high << 4) | (dummy_low & 0xf)))
		if( (((dummy_high << 4) ^ reg_a) & 0x80) && !((reg_a ^ DataBus) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		if (dummy_high > 9) dummy_high += 6;
		if (dummy_high > 0xf) flag_C = 0x1;
		else flag_C = 0;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
	}')dnl
define(RTI,`readbank
	AddressBus = reg_sp + 0x100;
	readbank
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank
	flag_N = DataBus & 0x80;
	flag_V = DataBus & 0x40;
//	flag_B = DataBus & 0x10;
	flag_D = DataBus & 0x08;
	flag_I = DataBus & 0x04;
	flag_Z = DataBus & 0x02;
	flag_C = DataBus & 0x01;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank
	reg_pc = DataBus;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank
	reg_pc = (DataBus << 8) + reg_pc;')dnl
define(RTS,`readbank
	AddressBus = reg_sp + 0x100;
	readbank
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank
	reg_pc = DataBus;
	reg_sp++;
	AddressBus = reg_sp + 0x100;
	readbank
	reg_pc = (DataBus << 8) + reg_pc;
	readbank
	reg_pc++;')dnl
define(SAX,`DataBus = (reg_a & reg_x);
	writebank')dnl
define(SBC,`readbank
	if(!flag_D){
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		reg_a = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		flag_C = dummy_flag;
		test_zn(reg_a)
	}else{
		if( (((reg_a + (0xff - DataBus) + flag_C) ^ reg_a) & 0x80) && !((reg_a ^ (0xff - DataBus)) & 0x80) ) flag_V = 0x40;
		else flag_V = 0;
		dummy_flag = (((reg_a + (0xff - DataBus) + flag_C) & 0x100) >> 8);
		dummy_low = ((reg_a + (0xff - DataBus) + flag_C) & 0xff);
		test_zn(dummy_low)
		dummy_low = (reg_a & 0xf) - (DataBus & 0xf) - (1 - flag_C);
		dummy_high = (reg_a >> 4) - (DataBus >> 4) - ((dummy_low & 0x10) >> 4);
		if (dummy_low & 0x10) dummy_low -= 6;
		if (dummy_high & 0x10) dummy_high -= 6;
		reg_a = ((dummy_high << 4) | (dummy_low & 0xf)) & 0xff;
		flag_C = dummy_flag;
		}')dnl
define(SBX,`readbank
	if(!((reg_x & reg_a) < DataBus)) flag_C = 1;
	else flag_C = 0;
	reg_x = ((reg_x & reg_a) - DataBus) & 0xff;
	test_zn(reg_x)')dnl
define(SEC,`readbank
	flag_C = 1;')dnl
define(SED,`readbank
	flag_D = 8;')dnl
define(SEI,`readbank
	flag_I = 4;')dnl
define(SHA,`DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x & reg_a;
	writebank')dnl
define(SHS,`DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x & reg_a;
	reg_sp = (reg_a & reg_x);
	writebank')dnl
define(SHX,`DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_x;
	writebank')dnl
define(SHY,`DataBus = (((AddressBus + 0x100) & 0xff00) >> 8) & reg_y;
	writebank')dnl
define(SLO,`flag_C = (DataBus & 0x80) >> 7;
	DataBus = (DataBus << 1) & 0xfe;
	reg_a |= DataBus;
	test_zn(reg_a)')dnl
define(SRE,`flag_C = DataBus & 0x01;
	DataBus = (DataBus >> 1) & 0x7f;
	reg_a ^= DataBus;
	test_zn(reg_a)')dnl
define(STA,`DataBus = reg_a;
	writebank')dnl
define(STX,`DataBus = reg_x;
	writebank')dnl
define(STY,`DataBus = reg_y;
	writebank')dnl
define(TAX,`readbank
	reg_x = reg_a;
	test_zn(reg_x)')dnl
define(TAY,`readbank
	reg_y = reg_a;
	test_zn(reg_y)')dnl
define(TXA,`readbank
	reg_a = reg_x;
	test_zn(reg_a)')dnl
define(TYA,`readbank
	reg_a = reg_y;
	test_zn(reg_a)')dnl
define(TSX,`readbank
	reg_x = reg_sp;
	test_zn(reg_x)')dnl
define(TXS,`readbank
	reg_sp = reg_x;')dnl

/*
	the opcode definitions
*/

/* 00 - BRK implied */
opcode(_00,adr_implied,BRK)

/* 01 - ORA (indirect,x) */
opcode(_01,adr_index_indirect,ORA)

/* 02 - JAM */

/* 03 - SLO (indirect,x) */
opcode(_03,adr_index_indirect,rmw_ram(SLO))

/* 04 - NOP zeropage */
opcode(_04,adr_zeropage,NOP)

/* 05 - ORA zeropage */
opcode(_05,adr_zeropage,ORA)

/* 06 - ASL zeropage */
opcode(_06,adr_zeropage,rmw_ram(ASL(DataBus)))

/* 07 - SLO zeropage */
opcode(_07,adr_zeropage,rmw_ram(SLO))

/* 08 - PHP implied */
opcode(_08,adr_implied,PHP)

/* 09 - ORA immediate */
opcode(_09,adr_immediate,ORA)

/* 0a - ASL implied */
opcode(_0a,adr_implied,rmw_accu(ASL(reg_a)))

/* 0b - ANC immediate */
opcode(_0b,adr_immediate,ANC)

/* 0c - NOP absolute */
opcode(_0c,adr_absolute,NOP)

/* 0d - ORA absolute */
opcode(_0d,adr_absolute,ORA)

/* 0e - ASL absolute */
opcode(_0e,adr_absolute,rmw_ram(ASL(DataBus)))

/* 0f - SLO absolute */
opcode(_0f,adr_absolute,rmw_ram(SLO))

/* 10 - BPL relative */
opcode(_10,adr_implied,BPL)

/* 11 - ORA (indirect),y */
opcode(_11,adr_indirect_index_read,ORA)

/* 12 - JAM */

/* 13 - SLO (indirect),y */
opcode(_13,adr_indirect_index_write,rmw_ram(SLO))

/* 14 - NOP zeropage,x */
opcode(_14,adr_zeropage_index(reg_x),NOP)

/* 15 - ORA zeropage,x */
opcode(_15,adr_zeropage_index(reg_x),ORA)

/* 16 - ASL zeropage,x */
opcode(_16,adr_zeropage_index(reg_x),rmw_ram(ASL(DataBus)))

/* 17 - SLO zeropage,x */
opcode(_17,adr_zeropage_index(reg_x),rmw_ram(SLO))

/* 18 - CLC implied */
opcode(_18,adr_implied,CLC)

/* 19 - ORA absolute,y */
opcode(_19,adr_absolute_index_read(reg_y),ORA)

/* 1a - NOP implied */
opcode(_1a,adr_implied,NOP)

/* 1b - SLO absolute,y */
opcode(_1b,adr_absolute_index_write(reg_y),rmw_ram(SLO))

/* 1c - NOP absolute,x */
opcode(_1c,adr_absolute_index_read(reg_x),NOP)

/* 1d - ORA absolute,x */
opcode(_1d,adr_absolute_index_read(reg_x),ORA)

/* 1e - ASL absolute,x */
opcode(_1e,adr_absolute_index_write(reg_x),rmw_ram(ASL(DataBus)))

/* 1f - SLO absolute,x */
opcode(_1f,adr_absolute_index_write(reg_x),rmw_ram(SLO))

/* 20 - JSR absolute */
opcode(_20,`',JSR)

/* 21 - AND (indirect,x) */
opcode(_21,adr_index_indirect,AND)

/* 22 - JAM */

/* 23 - RLA (indirect,x) */
opcode(_23,adr_index_indirect,rmw_ram(RLA))

/* 24 - BIT zeropage */
opcode(_24,adr_zeropage,BIT)

/* 25 - AND zeropage */
opcode(_25,adr_zeropage,AND)

/* 26 - ROL zeropage */
opcode(_26,adr_zeropage,rmw_ram(ROL(DataBus)))

/* 27 - RLA zeropage */
opcode(_27,adr_zeropage,rmw_ram(RLA))

/* 28 - PLP implied */
opcode(_28,adr_implied,PLP)

/* 29 - AND immediate */
opcode(_29,adr_immediate,AND)

/* 2a - ROL implied */
opcode(_2a,adr_implied,rmw_accu(ROL(reg_a)))

/* 2b - ANC immediate */
opcode(_2b,adr_immediate,ANC)

/* 2c - BIT absolute */
opcode(_2c,adr_absolute,BIT)

/* 2d - AND absolute */
opcode(_2d,adr_absolute,AND)

/* 2e - ROL absolute */
opcode(_2e,adr_absolute,rmw_ram(ROL(DataBus)))

/* 2f - RLA absolute */
opcode(_2f,adr_absolute,rmw_ram(RLA))

/* 30 - BMI relative */
opcode(_30,adr_implied,BMI)

/* 31 - AND (indirect),y */
opcode(_31,adr_indirect_index_read,AND)

/* 32 - JAM */

/* 33 - RLA (indirect),y */
opcode(_33,adr_indirect_index_write,rmw_ram(RLA))

/* 34 - NOP zeropage,x */
opcode(_34,adr_zeropage_index(reg_x),NOP)

/* 35 - AND zeropage,x */
opcode(_35,adr_zeropage_index(reg_x),AND)

/* 36 - ROL zeropage,x */
opcode(_36,adr_zeropage_index(reg_x),rmw_ram(ROL(DataBus)))

/* 37 - RLA zeropage,x */
opcode(_37,adr_zeropage_index(reg_x),rmw_ram(RLA))

/* 38 - SEC implied */
opcode(_38,adr_implied,SEC)

/* 39 - AND absolute,y */
opcode(_39,adr_absolute_index_read(reg_y),AND)

/* 3a - NOP implied */
opcode(_3a,adr_implied,NOP)

/* 3b - RLA absolute,y */
opcode(_3b,adr_absolute_index_write(reg_y),rmw_ram(RLA))

/* 3c - NOP absolute,x */
opcode(_3c,adr_absolute_index_read(reg_x),NOP)

/* 3d - AND absolute,x */
opcode(_3d,adr_absolute_index_read(reg_x),AND)

/* 3e - ROL absolute,x */
opcode(_3e,adr_absolute_index_write(reg_x),rmw_ram(ROL(DataBus)))

/* 3f - RLA absolute,x */
opcode(_3f,adr_absolute_index_write(reg_x),rmw_ram(RLA))

/* 40 - RTI implied */
opcode(_40,adr_implied,RTI)

/* 41 - EOR (indirect,x) */
opcode(_41,adr_index_indirect,EOR)

/* 42 - JAM */

/* 43 - SRE (indirect,x) */
opcode(_43,adr_index_indirect,rmw_ram(SRE))

/* 44 - NOP zeropage */
opcode(_44,adr_zeropage,NOP)

/* 45 - EOR zeropage */
opcode(_45,adr_zeropage,EOR)

/* 46 - LSR zeropage */
opcode(_46,adr_zeropage,rmw_ram(LSR(DataBus)))

/* 47 - SRE zeropage */
opcode(_47,adr_zeropage,rmw_ram(SRE))

/* 48 - PHA implied */
opcode(_48,adr_implied,PHA)

/* 49 - EOR immediate */
opcode(_49,adr_immediate,EOR)

/* 4a - LSR implied */
opcode(_4a,adr_implied,rmw_accu(LSR(reg_a)))

/* 4b - ASR immediate */
opcode(_4b,adr_immediate,ASR)

/* 4c - JMP absolute */
opcode(_4c,adr_absolute,JMP)

/* 4d - EOR absolute */
opcode(_4d,adr_absolute,EOR)

/* 4e - LSR absolute */
opcode(_4e,adr_absolute,rmw_ram(LSR(DataBus)))

/* 4f - SRE absolute */
opcode(_4f,adr_absolute,rmw_ram(SRE))

/* 50 - BVC relative */
opcode(_50,adr_implied,BVC)

/* 51 - EOR (indirect),y */
opcode(_51,adr_indirect_index_read,EOR)

/* 52 - JAM */

/* 53 - SRE (indirect),y */
opcode(_53,adr_indirect_index_write,rmw_ram(SRE))

/* 54 - NOP zeropage,x */
opcode(_54,adr_zeropage_index(reg_x),NOP)

/* 55 - EOR zeropage,x */
opcode(_55,adr_zeropage_index(reg_x),EOR)

/* 56 - LSR zeropage,x */
opcode(_56,adr_zeropage_index(reg_x),rmw_ram(LSR(DataBus)))

/* 57 - SRE zeropage,x */
opcode(_57,adr_zeropage_index(reg_x),rmw_ram(SRE))

/* 58 - CLI implied */
opcode(_58,adr_implied,CLI)

/* 59 - EOR absolute,y */
opcode(_59,adr_absolute_index_read(reg_y),EOR)

/* 5a - NOP implied */
opcode(_5a,adr_implied,NOP)

/* 5b - SRE absolute,y */
opcode(_5b,adr_absolute_index_write(reg_y),rmw_ram(SRE))

/* 5c - NOP absolute,x */
opcode(_5c,adr_absolute_index_read(reg_x),NOP)

/* 5d - EOR absolute,x */
opcode(_5d,adr_absolute_index_read(reg_x),EOR)

/* 5e - LSR absolute,x */
opcode(_5e,adr_absolute_index_write(reg_x),rmw_ram(LSR(DataBus)))

/* 5f - SRE absolute,x */
opcode(_5f,adr_absolute_index_write(reg_x),rmw_ram(SRE))

/* 60 - RTS implied */
opcode(_60,adr_implied,RTS)

/* 61 - ADC (indirect,x) */
opcode(_61,adr_index_indirect,ADC)

/* 62 - JAM */

/* 63 - RRA (indirect,x) */
opcode(_63,adr_index_indirect,rmw_ram(RRA))

/* 64 - NOP zeropage */
opcode(_64,adr_zeropage,NOP)

/* 65 - ADC zeropage */
opcode(_65,adr_zeropage,ADC)

/* 66 - ROR zeropage */
opcode(_66,adr_zeropage,rmw_ram(ROR(DataBus)))

/* 67 - RRA zeropage */
opcode(_67,adr_zeropage,rmw_ram(RRA))

/* 68 - PLA implied */
opcode(_68,adr_implied,PLA)

/* 69 - ADC immediate */
opcode(_69,adr_immediate,ADC)

/* 6a - ROR implied */
opcode(_6a,adr_implied,rmw_accu(ROR(reg_a)))

/* 6b - ARR immediate */
opcode(_6b,adr_immediate,ARR)

/* 6c - JMP (indirect) */
opcode(_6c,adr_absolute_indirect,JMP)

/* 6d - ADC absolute */
opcode(_6d,adr_absolute,ADC)

/* 6e - ROR absolute */
opcode(_6e,adr_absolute,rmw_ram(ROR(DataBus)))

/* 6f - RRA absolute */
opcode(_6f,adr_absolute,rmw_ram(RRA))

/* 70 - BVS relative */
opcode(_70,adr_implied,BVS)

/* 71 - ADC (indirect),y */
opcode(_71,adr_indirect_index_read,ADC)

/* 72 - JAM */

/* 73 - RRA (indirect),y */
opcode(_73,adr_indirect_index_write,rmw_ram(RRA))

/* 74 - NOP zeropage,x */
opcode(_74,adr_zeropage_index(reg_x),NOP)

/* 75 - ADC zeropage,x */
opcode(_75,adr_zeropage_index(reg_x),ADC)

/* 76 - ROR zeropage,x */
opcode(_76,adr_zeropage_index(reg_x),rmw_ram(ROR(DataBus)))

/* 77 - RRA zeropage,x */
opcode(_77,adr_zeropage_index(reg_x),rmw_ram(RRA))

/* 78 - SEI implied */
opcode(_78,adr_implied,SEI)

/* 79 - ADC absolute,y */
opcode(_79,adr_absolute_index_read(reg_y),ADC)

/* 7a - NOP implied */
opcode(_7a,adr_implied,NOP)

/* 7b - RRA absolute,y */
opcode(_7b,adr_absolute_index_write(reg_y),rmw_ram(RRA))

/* 7c - NOP absolute,x */
opcode(_7c,adr_absolute_index_read(reg_x),NOP)

/* 7d - ADC absolute,x */
opcode(_7d,adr_absolute_index_read(reg_x),ADC)

/* 7e - ROR absolute,x */
opcode(_7e,adr_absolute_index_write(reg_x),rmw_ram(ROR(DataBus)))

/* 7f - RRA absolute,x */
opcode(_7f,adr_absolute_index_write(reg_x),rmw_ram(RRA))

/* 80 - NOP immediate */
opcode(_80,adr_immediate,NOP)

/* 81 - STA (indirect,x) */
opcode(_81,adr_index_indirect,STA)

/* 82 - NOP immediate */
opcode(_82,adr_immediate,NOP)

/* 83 - SAX (indirect),x */
opcode(_83,adr_index_indirect,SAX)

/* 84 - STY zeropage */
opcode(_84,adr_zeropage,STY)

/* 85 - STA zeropage */
opcode(_85,adr_zeropage,STA)

/* 86 - STX zeropage */
opcode(_86,adr_zeropage,STX)

/* 87 - SAX zeropage */
opcode(_87,adr_zeropage,SAX)

/* 88 - DEY implied */
opcode(_88,adr_implied,DEY)

/* 89 - NOP immediate */
opcode(_89,adr_immediate,NOP)

/* 8a - TXA implied */
opcode(_8a,adr_implied,TXA)

/* 8b - ANE immediate */
opcode(_8b,adr_immediate,ANE)

/* 8c - STY absolute */
opcode(_8c,adr_absolute,STY)

/* 8d - STA absolute */
opcode(_8d,adr_absolute,STA)

/* 8e - STX absolute */
opcode(_8e,adr_absolute,STX)

/* 8f - SAX absolute */
opcode(_8f,adr_absolute,SAX)

/* 90 - BCC relative */
opcode(_90,adr_implied,BCC)

/* 91 - STA (indirect),y */
opcode(_91,adr_indirect_index_write,STA)

/* 92 - JAM */

/* 93 - SHA (indirect),y */
opcode(_93,adr_indirect_index_write,SHA)

/* 94 - STY zeropage,x */
opcode(_94,adr_zeropage_index(reg_x),STY)

/* 95 - STA zeropage,x */
opcode(_95,adr_zeropage_index(reg_x),STA)

/* 96 - STX zeropage,y */
opcode(_96,adr_zeropage_index(reg_y),STX)

/* 97 - SAX zeropage,y */
opcode(_97,adr_zeropage_index(reg_y),SAX)

/* 98 - TYA implied */
opcode(_98,adr_implied,TYA)

/* 99 - STA absolute,y */
opcode(_99,adr_absolute_index_write(reg_y),STA)

/* 9a - TXS implied */
opcode(_9a,adr_implied,TXS)

/* 9b - SHS absolute,y */
opcode(_9b,adr_absolute_index_write(reg_y),SHS)

/* 9c - SHY absolute,x */
opcode(_9c,adr_absolute_index_write(reg_x),SHY)

/* 9d - STA absolute,x */
opcode(_9d,adr_absolute_index_write(reg_x),STA)

/* 9e - SHX absolute,y */
opcode(_9e,adr_absolute_index_write(reg_y),SHX)

/* 9f - SHA absolute,y */
opcode(_9f,adr_absolute_index_write(reg_y),SHA)

/* a0 - LDY immediate */
opcode(_a0,adr_immediate,LDY)

/* a1 - LDA (indirect,x) */
opcode(_a1,adr_index_indirect,LDA)

/* a2 - LDX immediate */
opcode(_a2,adr_immediate,LDX)

/* a3 - LAX (indirect,x) */
opcode(_a3,adr_index_indirect,LAX)

/* a4 - LDY zeropage */
opcode(_a4,adr_zeropage,LDY)

/* a5 - LDA zeropage */
opcode(_a5,adr_zeropage,LDA)

/* a6 - LDX zeropage */
opcode(_a6,adr_zeropage,LDX)

/* a7 - LAX zeropage */
opcode(_a7,adr_zeropage,LAX)

/* a8 - TAY implied */
opcode(_a8,adr_implied,TAY)

/* a9 - LDA immediate */
opcode(_a9,adr_immediate,LDA)

/* aa - TAX implied */
opcode(_aa,adr_implied,TAX)

/* ab - LXA immediate */
opcode(_ab,adr_immediate,LXA)

/* ac - LDY absolute */
opcode(_ac,adr_absolute,LDY)

/* ad - LDA absolute */
opcode(_ad,adr_absolute,LDA)

/* ae - LDX absolute */
opcode(_ae,adr_absolute,LDX)

/* af - LAX absolute */
opcode(_af,adr_absolute,LAX)

/* b0 - BCS relative */
opcode(_b0,adr_implied,BCS)

/* b1 - LDA (indirect),y */
opcode(_b1,adr_indirect_index_read,LDA)

/* b2 - JAM */

/* b3 - LAX (indirect),y */
opcode(_b3,adr_indirect_index_read,LAX)

/* b4 - LDY zeropage,x */
opcode(_b4,adr_zeropage_index(reg_x),LDY)

/* b5 - LDA zeropage,x */
opcode(_b5,adr_zeropage_index(reg_x),LDA)

/* b6 - LDX zeropage,y */
opcode(_b6,adr_zeropage_index(reg_y),LDX)

/* b7 - LAX zeropage,Y */
opcode(_b7,adr_zeropage_index(reg_y),LAX)

/* b8 - CLV implied */
opcode(_b8,adr_implied,CLV)

/* b9 - LDA absolute,y */
opcode(_b9,adr_absolute_index_read(reg_y),LDA)

/* ba - TSX implied */
opcode(_ba,adr_implied,TSX)

/* bb - LAS absolute,y */
opcode(_bb,adr_absolute_index_read(reg_y),LAS)

/* bc - LDY absolute,x */
opcode(_bc,adr_absolute_index_read(reg_x),LDY)

/* bd - LDA absolute,x */
opcode(_bd,adr_absolute_index_read(reg_x),LDA)

/* be - LDX absolute,y */
opcode(_be,adr_absolute_index_read(reg_y),LDX)

/* bf - LAX absolute,y */
opcode(_bf,adr_absolute_index_read(reg_y),LAX)

/* c0 - CPY immediate */
opcode(_c0,adr_immediate,CPY)

/* c1 - CMP (indirect,x) */
opcode(_c1,adr_index_indirect,CMP)

/* c2 - NOP immediate */
opcode(_c2,adr_immediate,NOP)

/* c3 - DCP (indirect,x) */
opcode(_c3,adr_index_indirect,rmw_ram(DCP))

/* c4 - CPY zeropage */
opcode(_c4,adr_zeropage,CPY)

/* c5 - CMP zeropage */
opcode(_c5,adr_zeropage,CMP)

/* c6 - DEC zeropage */
opcode(_c6,adr_zeropage,rmw_ram(DEC))

/* c7 - DCP zeropage */
opcode(_c7,adr_zeropage,rmw_ram(DCP))

/* c8 - INY implied */
opcode(_c8,adr_implied,INY)

/* c9 - CMP immediate */
opcode(_c9,adr_immediate,CMP)

/* ca - DEX implied */
opcode(_ca,adr_implied,DEX)

/* cb - SBX immediate */
opcode(_cb,adr_immediate,SBX)

/* cc - CPY absolute */
opcode(_cc,adr_absolute,CPY)

/* cd - CMP absolute */
opcode(_cd,adr_absolute,CMP)

/* ce - DEC absolute */
opcode(_ce,adr_absolute,rmw_ram(DEC))

/* cf - DCP absolute */
opcode(_cf,adr_absolute,rmw_ram(DCP))

/* d0 - BNE relative */
opcode(_d0,adr_implied,BNE)

/* d1 - CMP (indirect),y */
opcode(_d1,adr_indirect_index_read,CMP)

/* d2 - JAM */

/* d3 - DCP (indirect),y */
opcode(_d3,adr_indirect_index_write,rmw_ram(DCP))

/* d4 - NOP zeropage,x */
opcode(_d4,adr_zeropage_index(reg_x),NOP)

/* d5 - CMP zeropage,x */
opcode(_d5,adr_zeropage_index(reg_x),CMP)

/* d6 - DEC zeropage,x */
opcode(_d6,adr_zeropage_index(reg_x),rmw_ram(DEC))

/* d7 - DCP zeropage,x */
opcode(_d7,adr_zeropage_index(reg_x),rmw_ram(DCP))

/* d8 - CLD implied */
opcode(_d8,adr_implied,CLD)

/* d9 - CMP absolute,y */
opcode(_d9,adr_absolute_index_read(reg_y),CMP)

/* da - NOP implied */
opcode(_da,adr_implied,NOP)

/* db - DCP absolute,y */
opcode(_db,adr_absolute_index_write(reg_y),rmw_ram(DCP))

/* dc - NOP absolute,x */
opcode(_dc,adr_absolute_index_read(reg_x),NOP)

/* dd - CMP absolute,x */
opcode(_dd,adr_absolute_index_read(reg_x),CMP)

/* de - DEC absolute,x */
opcode(_de,adr_absolute_index_write(reg_x),rmw_ram(DEC))

/* df - DCP absolute,x */
opcode(_df,adr_absolute_index_write(reg_x),rmw_ram(DCP))

/* e0 - CPX immediate */
opcode(_e0,adr_immediate,CPX)

/* e1 - SBC (indirect,x) */
opcode(_e1,adr_index_indirect,SBC)

/* e2 - NOP immediate */
opcode(_e2,adr_immediate,NOP)

/* e3 - ISB (indirect,x) */
opcode(_e3,adr_index_indirect,rmw_ram(ISB))

/* e4 - CPX zeropage */
opcode(_e4,adr_zeropage,CPX)

/* e5 - SBC zeropage */
opcode(_e5,adr_zeropage,SBC)

/* e6 - INC zeropage */
opcode(_e6,adr_zeropage,rmw_ram(INC))

/* e7 - ISB zeropage */
opcode(_e7,adr_zeropage,rmw_ram(ISB))

/* e8 - INX implied */
opcode(_e8,adr_implied,INX)

/* e9 - SBC immediate */
opcode(_e9,adr_immediate,SBC)

/* ea - NOP implied (this is the real one) */
opcode(_ea,adr_implied,NOP)

/* eb - SBC immediate */
opcode(_eb,adr_immediate,SBC)

/* ec - CPX absolute */
opcode(_ec,adr_absolute,CPX)

/* ed - SBC absolute */
opcode(_ed,adr_absolute,SBC)

/* ee - INC absolute */
opcode(_ee,adr_absolute,rmw_ram(INC))

/* ef - ISB absolute */
opcode(_ef,adr_absolute,rmw_ram(ISB))

/* f0 - BEQ relative */
opcode(_f0,adr_implied,BEQ)

/* f1 - SBC (indirect),y */
opcode(_f1,adr_indirect_index_read,SBC)

/* f2 - JAM */

/* f3 - ISB (indirect),y */
opcode(_f3,adr_indirect_index_write,rmw_ram(ISB))

/* f4 - NOP zeropage,x */
opcode(_f4,adr_zeropage_index(reg_x),NOP)

/* f5 - SBC zeropage,x */
opcode(_f5,adr_zeropage_index(reg_x),SBC)

/* f6 - INC zeropage,x */
opcode(_f6,adr_zeropage_index(reg_x),rmw_ram(INC))

/* f7 - ISB zeropage,x */
opcode(_f7,adr_zeropage_index(reg_x),rmw_ram(ISB))

/* f8 - SED implied */
opcode(_f8,adr_implied,SED)

/* f9 - SBC absolute,y */
opcode(_f9,adr_absolute_index_read(reg_y),SBC)

/* fa - NOP implied */
opcode(_fa,adr_implied,NOP)

/* fb - ISB absolute,y */
opcode(_fb,adr_absolute_index_write(reg_y),rmw_ram(ISB))

/* fc - NOP absolute,x */
opcode(_fc,adr_absolute_index_read(reg_x),NOP)

/* fd - SBC absolute,x */
opcode(_fd,adr_absolute_index_read(reg_x),SBC)

/* fe - INC absolute,x */
opcode(_fe,adr_absolute_index_write(reg_x),rmw_ram(INC))

/* ff - ISB absolute,x */
opcode(_ff,adr_absolute_index_write(reg_x),rmw_ram(ISB))


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
	readbank
	reg_pc = DataBus;
	AddressBus++;
	readbank
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
	readbank
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
		readbank
		reg_pc++;
		AddressBus = reg_pc;
		(* vectors[DataBus])();	/* do the instruction */
	 /*	ClockRIOT() is only used here, so we can include it directly */
		Timer -= RCycles;	/* This is all ClockRIOT() does */
	}
}
