/*
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/

extern dd P0_Position;
extern dd P1_Position;
extern dd M0_Position;
extern dd M1_Position;
extern dd BL_Position;

dd debugflag = 0;

db RROM, RROM1, RROM2;  /* contain the result of ReadROM(cpu_pc) */

enum {_imp, _acc, _rel, _imm, _abs, _abs_x, _abs_y, _zero, _zero_x, _zero_y, _ind, _ind_x, _ind_y};


typedef char InstName[4];
InstName InstList[256]={
   "brk", "ora", "jam",	"slo", "nop", "ora", "asl",	"slo",
   "php", "ora", "asl",	"anc", "nop", "ora", "asl",	"slo",

   "bpl", "ora", "jam",	"slo", "nop", "ora", "asl",	"slo",
   "clc", "ora", "nop",	"slo", "nop", "ora", "asl",	"slo",

   "jsr", "and", "jam",	"rla", "bit", "and", "rol",	"rla",
   "plp", "and", "rol",	"jam", "bit", "and", "rol",	"rla",

   "bmi", "and", "jam",	"rla", "nop", "and", "rol",	"rla",
   "sec", "and", "nop",	"rla", "nop", "and", "rol",	"rla",

   "rti", "eor", "jam",	"sre", "nop", "eor", "lsr",	"sre",
   "pha", "eor", "lsr",	"asr", "jmp", "eor", "lsr",	"sre",

   "bvc", "eor", "jam",	"sre", "nop", "eor", "lsr",	"sre",
   "cli", "eor", "nop",	"sre", "nop", "eor", "lsr",	"sre",

   "rts", "adc", "jam",	"rra", "nop", "adc", "ror",	"rra",
   "pla", "adc", "ror",	"arr", "jmp", "adc", "ror",	"rra",

   "bvs", "adc", "jam",	"rra", "nop", "adc", "ror",	"rra",
   "sei", "adc", "nop",	"rra", "nop", "adc", "ror",	"rra",

   "nop", "sta", "nop",	"sax", "sty", "sta", "stx",	"sax",
   "dey", "nop", "txa",	"ane", "sty", "sta", "stx",	"sax",

   "bcc", "sta", "jam",	"sha", "sty", "sta", "stx",	"sax",
   "tya", "sta", "txs",	"shs", "shy", "sta", "shx",	"sha",

   "ldy", "lda", "ldx",	"lax", "ldy", "lda", "ldx",	"lax",
   "tay", "lda", "tax",	"lxa", "ldy", "lda", "ldx",	"lax",

   "bcs", "lda", "jam",	"lax", "ldy", "lda", "ldx",	"lax",
   "clv", "lda", "tsx",	"las", "ldy", "lda", "ldx",	"lax",

   "cpy", "cmp", "nop",	"dcp", "cpy", "cmp", "dec",	"dcp",
   "iny", "cmp", "dex",	"sbx", "cpy", "cmp", "dec",	"dcp",

   "bne", "cmp", "jam",	"dcp", "nop", "cmp", "dec",	"dcp",
   "cld", "cmp", "nop",	"dcp", "nop", "cmp", "dec",	"dcp",

   "cpx", "sbc", "nop",	"isb", "cpx", "sbc", "inc",	"isb",
   "inx", "sbc", "nop",	"sbc", "cpx", "sbc", "inc",	"isb",

   "beq", "sbc", "jam",	"isb", "nop", "sbc", "inc",	"isb",
   "sed", "sbc", "nop",	"isb", "nop", "sbc", "inc",	"isb"
};

int AccessList[256] = {
   _imp, _ind_x, _imp, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _acc,  _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_x,  _abs_x,

   _abs, _ind_x, _imp, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _acc,  _imp,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero,   _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _imp,    _abs_x,  _abs_x,  _abs_x,

   _imp, _ind_x, _imp, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _acc,  _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_x,  _abs_x,

   _imp, _ind_x, _imp, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _acc,  _imm,   _ind,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_x,  _abs_x,

   _imm, _ind_x, _imm, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _imp, _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_y, _zero_y,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_y,  _abs_y,

   _imm, _ind_x, _imm, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _imp, _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_y, _zero_y,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_y,  _abs_y,

   _imm, _ind_x, _imm, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _imp, _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_x,  _abs_x,

   _imm, _ind_x, _imm, _ind_x, _zero,   _zero,   _zero,   _zero,
   _imp, _imm,   _imp, _imm,   _abs,    _abs,    _abs,    _abs,

   _rel, _ind_y, _imp, _ind_y, _zero_x, _zero_x, _zero_x, _zero_x,
   _imp, _abs_y, _imp, _abs_y, _abs_x,  _abs_x,  _abs_x,  _abs_x
};

typedef char TIAname[8];
TIAname TIAList[0x40] = {
   "VSYNC", "VBLANK", "WSYNC", "*RSYNC*", "NUSIZ0", "NUSIZ1", "COLUP0", "COLUP1",
   "COLUPF", "COLUBK", "CTRLPF", "REFP0", "REFP1", "PF0", "PF1", "PF2",
   "RESP0", "RESP1", "RESM0", "RESM1", "RESBL", "AUDC0", "AUDC1", "AUDF0",
   "AUDF1", "AUDV0", "AUDV1", "GRP0", "GRP1", "ENAM0", "ENAM1", "ENABL",
   "HMP0", "HMP1", "HMM0", "HMM1", "HMBL", "VDELP0", "VDELP1", "VDELBL",
   "RESMP0", "RESMP1", "HMOVE", "HMCLR", "CXCLR", "2d", "2e", "2f",

   "CXM0P", "CXM1P", "CXP0FB", "CXP1FB", "CXM0FB", "CXM1FB", "CXBLPF", "CXPPMM",
   "INPT0", "INPT1", "INPT2", "INPT3", "INPT4", "INPT5", "3e", "3f"
};

typedef char RIOTname[7];
RIOTname RIOTList1[8] = {
   "SWCHA", "SWACNT", "SWCHB", "SWBCNT",
   "INTIM", "TIMINT", "intim", "timint"
};

RIOTname RIOTList2[4] = {
   "TIM1T", "TIM8T", "TIM64T", "T1024T"
};

extern void cpu_Instruction(void);
extern void cpu_Reset(void);

db cpu_Rbyte;

unsigned char ReadROM(unsigned int adr)
{
	db dummy;
	
    AddressBus = (adr & 0xffff);
    dummy = DataBus;
    debugflag = 1;
//	(* ReadROMAccess)();		// <--- *** poison *** ---
	debugflag = 0;
	cpu_Rbyte = DataBus;
	DataBus = dummy;
    return(cpu_Rbyte);
}

unsigned int ReadRAM(unsigned int adr)
{
	return(RiotRam[adr & 0x7f] + (RiotRam[(adr + 1) & 0x7f] * 256));
}


/*
dd cpu_pc;
db cpu_a, cpu_carry, cpu_x, cpu_y, cpu_sp;
db cpu_ZTest, cpu_NTest, cpu_D, cpu_V, cpu_I, cpu_B;

dd P0_Pos, P1_Pos, M0_Pos, M1_Pos, BL_Pos;

dd frame;
dd line;
db cycle;
*/

void ShowWeird(dd Cycle)
{
	fprintf(zlog,"<-- weird (%d)\n", Cycle);
}

void ShowDeep(dd Now, dd Prev, dd Cycle)
{
	fprintf(zlog,"<-- deep (%d, %x, %x)\n", Cycle, Prev, Now);
}

void ShowVeryDeep(dd Now, dd Prev, dd Cycle)
{
	fprintf(zlog,"<-- very deep (%d, %x, %x)\n", Cycle, Prev, Now);
}

void ShowAdjusted(void)
{
	fprintf(zlog,"Adjusted\n");
}

void ShowUndocTIA(void)
{
	fprintf(zlog,"<-- Undoc TIA\n");
}

void ShowCollision(void)
{
	fprintf(zlog,"<-- Collision\n");
}

void ShowSCWrite(void)
{
	fprintf(zlog,"<-- SC Write\n");
}

int ti_op8(void)
{
/*        return (ReadROM(cpu_pc + 1)); */
        return (RROM1);
}

unsigned int ti_op16(void)
{
/*        return (ReadROM(cpu_pc + 1) + 256*ReadROM(cpu_pc + 2)); */
        return (RROM1 + 256 * RROM2);
}

void ti_show_imp(void)
{
        fprintf(zlog, "         ");
}

void ti_show_acc(void)
{
        fprintf(zlog, " a       ");
}

void ti_show_zero_xy(unsigned int op)
{
	if (op & 0x80) {
                fprintf(zlog, " %02x      ", op);
        } else {
                op &= 0x3f;
                /* simplified check for write-only access  *EST* */
                if (!((RROM & 0xe0) == 0x80))
                        op = (op & 0x0f) | 0x30;
                fprintf(zlog, " %-8s", TIAList[op]);
        }
}

void ti_show_zero(void)
{
        ti_show_zero_xy(ti_op8());
}

void ti_show_zero_x(void)
{
        fprintf(zlog, " %02x,x    =", ti_op8());
        ti_show_zero_xy(ti_op8() + reg_x);
}

void ti_show_zero_y(void)
{
        fprintf(zlog, " %02x,y    =", ti_op8());
        ti_show_zero_xy(ti_op8() + reg_y);
}

void ti_show_abs_xy(unsigned int op)
{
	if (op > 0x27f && op < 0x288) {
                fprintf(zlog, " %-8s", RIOTList1[op - 0x280]);
	} else if (op > 0x293 && op < 0x298) {
                fprintf(zlog, " %-8s", RIOTList2[op - 0x294]);
	} else {
		/* if not ROM, RIOT or RIOT RAM, it must be TIA; *EST* */
                if (!(op & 0x1280))
                        ti_show_zero_xy(op);
		else
                        fprintf(zlog, " %04x    ", op);
	}
}

void ti_show_abs(void)
{
        ti_show_abs_xy(ti_op16());
}

void ti_show_abs_y(void)
{
        fprintf(zlog, " %04x,y  =", ti_op16());
        ti_show_abs_xy(ti_op16() + reg_y);
}

void ti_show_abs_x(void)
{
        fprintf(zlog, " %04x,x  =", ti_op16());
        ti_show_abs_xy(ti_op16() + reg_x);
}


void ti_show_ind(void)
{
        fprintf(zlog, "(%04x)   ", ti_op16());
}

void ti_show_ind_x(void)
{
unsigned char real_TIA;

        real_TIA = ti_op8();
        fprintf(zlog, "(%02x,x)   =", real_TIA);
        real_TIA += reg_x;
        ti_show_abs_xy(ReadRAM(real_TIA));
}

void ti_show_ind_y(void)
{
unsigned char real_TIA;

        real_TIA = ti_op8();
        fprintf(zlog, "(%02x),y   =", real_TIA);
        ti_show_abs_xy(ReadRAM(real_TIA) + reg_y);
}

void ti_show_immediate(void)
{
        fprintf(zlog, "#%02x      ", ti_op8());
}

void ti_show_relative(void)
{
	int target;

	target = (char) ti_op8();
	target += reg_pc + 2;
        fprintf(zlog, " %04x    ", target);
}


typedef void (*FP)(void);

FP ti_show_op_fnc[] =
{
	ti_show_imp,
	ti_show_acc,
	ti_show_relative,
	ti_show_immediate,
	ti_show_abs,
	ti_show_abs_x,
	ti_show_abs_y,
	ti_show_zero,
	ti_show_zero_x,
	ti_show_zero_y,
	ti_show_ind,
	ti_show_ind_x,
	ti_show_ind_y
};

void ShowInstruction(void)
{
	int optype;

    optype = AccessList[RROM];
    fprintf(zlog, "%04x: ", reg_pc);

/* ti_show_code */
	switch (optype)
	{
		case _imp:
		case _acc:
            fprintf(zlog, "%02x       ", RROM);
			break;

		case _rel:
		case _imm:
		case _zero:
		case _zero_x:
		case _zero_y:
		case _ind_x:
		case _ind_y:
            fprintf(zlog, "%02x %02x    ", RROM, RROM1);
			break;

		case _abs:
		case _abs_x:
		case _abs_y:
		case _ind:
            fprintf(zlog, "%02x %02x %02x ", RROM, RROM1, RROM2);
			break;
	}

/* ti_show_op */
    fprintf(zlog, "%s ", InstList[RROM]);

	ti_show_op_fnc[optype]();
}

void ShowRegisters(void)
{
        RROM = ReadROM(reg_pc);
        RROM1 = ReadROM(reg_pc + 1);
        RROM2 = ReadROM(reg_pc + 2);

/*
        fprintf(zlog, "\n(%3d %3d %3d) (%3d %3d) (%3d %3d %3d %3d %3d) ",
           (int)frame, line, cycle, line-42, cycle*3-68,
           (P0_Pos-68+5)%160, (P1_Pos-68+5)%160, (M0_Pos-68+4)%160,
           (M1_Pos-68+4)%160, (BL_Pos-68+4)%160);
*/

	fprintf(zlog, "\n(%3d %3d %3d %3d) (%3d %3d %3d %3d %3d) ",
	   (int)Frame, ScanLine, RClock, RClock*3-68,
	   (P0_Position-68+5)%160, (P1_Position-68+5)%160, (M0_Position-68+4)%160,
	   (M1_Position-68+4)%160, (BL_Position-68+4)%160);

	fprintf(zlog, flag_N & 0x80 ? "N" : "n");
	fprintf(zlog, flag_V 			  ? "V" : "v");
	fprintf(zlog, flag_B 			  ? "B" : "b");
	fprintf(zlog, flag_D 			  ? "D" : "d");
	fprintf(zlog, flag_I 			  ? "I" : "i");
	fprintf(zlog, flag_Z		 	  ? "Z" : "z");
	fprintf(zlog, flag_C 			  ? "C" : "c");

    fprintf(zlog, " %02x %02x %02x %02x  ", reg_a, reg_x, reg_y, reg_sp);
}

void TraceInstruction(void)
{
	ShowRegisters();
	ShowInstruction();
}

/*
 * $Log: trace.c,v $
 * Revision 1.2  2004/05/08 18:06:58  urchlay
 *
 * Added Log tag to all C and asm source files.
 *
 */
