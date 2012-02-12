/*
	z26 RIOT emu

	I'm not really sure what mode the timer starts up in but it's not mode 1.
	Otherwise blueprnt.bin doesn't come up and others as well.
*/

#define START_TIME 0x7fff

dd Timer = START_TIME;	//  the RIOT Timer
						// (gets initialized in INIT.C now)

void (* TimerReadVec)(void);	//  timer read vector

db DDR_A = 0;
db DDR_B = 0;


void WriteRIOTRAM(void){
	
	RiotRam[AddressBus & 0x7f] = DataBus;
}

void ReadRIOTRAM(void){
	
	DataBus = RiotRam[AddressBus & 0x7f];
}

void ReadDDR_A(void){			//  read data direction register A

	DataBus = DDR_A;
}

void ReadDDR_B(void){			//  read data direction register B

	DataBus = DDR_B;
}

void ReadPortB(void){			//  read console switches (port b)

	DataBus = IOPortB;
}

void ReadPortA(void){			//  read hand controllers (port a)

	//UpdateTrakBall(ScanLine);
/* pins grounded by controller read0 even on pins where HIGH was written to (see Star Raiders) */
	DataBus = ((IOPortA_Controllers | IOPortA_UnusedBits) & IOPortA) &
		((DDR_A ^ 0xff) | IOPortA_Write);
}


void WriteNothing(void){}

void WritePortA(void){

	IOPortA_Write = DataBus;	// remember all written bits
	/* make sure only output bits get wtitten to SWCHA */
	IOPortA = (DataBus & DDR_A) | ((DDR_A ^ 0xff) & IOPortA);
	/* update controllers on SWCHA write (KEypad, Compumate, Mindlink) */
	ControlSWCHAWrite();
}
	
void WriteDDR_A(void){

	DDR_A = DataBus;
}

void WriteDDR_B(void){

	DDR_B = DataBus;
}


/*
	CPU wants to read the RIOT timer
*/

void ReadTimer1(void){

	DataBus = Timer & 0xff;
}

void ReadTimer8(void){

	DataBus = (Timer >> 3) & 0xff;
}

void ReadTimer64(void){

	DataBus = (Timer >> 6) & 0xff;
}

void ReadTimer1024(void){

	DataBus = (Timer >> 10) & 0xff;
}

void ReadTimer(void){

	Timer -= RCycles;	// clock this instruction
	RCycles = 0;		// prevent double clock
	if(Timer & 0x40000) DataBus = Timer & 0xff;	// timer has overflowed - switch ti TIM1T
	else (* TimerReadVec)();
}


/*
	CPU wants to read the RIOT Timer Interrupt Register
*/

void ReadTimerIntReg(void){

	DataBus = (Timer >> 24) & 0x80;
/*
 I don't exactly know how many bits to leave in the Timer counter
 because I don't exactly know how long it is to the next interrupt.
 But another interrupt *does* come.  (Otherwise lockchse.bin fails.)
*/
	Timer &= START_TIME;
}


/*
	CPU wants to set the timer by writing to one of the RIOT timer regs:

	$294 (TIM1T)
	$295 (TIM8T)
	$296 (TIM64T)
	$297 (TIM1024T)

*/

void SetRIOTTimer1(void){

	RCycles = 0;	// don't clock this instruction
	Timer = DataBus;
	TimerReadVec = &ReadTimer1;
}

void SetRIOTTimer8(void){

	RCycles = 0;	// don't clock this instruction
	Timer = DataBus << 3;
	TimerReadVec = &ReadTimer8;
}

void SetRIOTTimer64(void){

	RCycles = 0;	// don't clock this instruction
	Timer = DataBus << 6;
	TimerReadVec = &ReadTimer64;
}

void SetRIOTTimer1024(void){

	RCycles = 0;	// don't clock this instruction
	Timer = DataBus << 10;
	TimerReadVec = &ReadTimer1024;
}


/*
	randomize RIOT timer
*/

void RandomizeRIOTTimer() {
	/* Seconds gets set in globals.c */
	Timer = ((Seconds & 0xff) << 10);
}


void (* ReadRIOTTab[8])(void) = {						
	ReadPortA,			//  280h PA Data
	ReadDDR_A,			//  281h PA Direction
	ReadPortB,			//  282h PB Data
	ReadDDR_B,			//  283h PB Direction
	ReadTimer,			//  284h Read Timer
	ReadTimerIntReg,	//  285h Read Timer Interrupt Register
	ReadTimer,			//  286h Read Timer
	ReadTimerIntReg		//  287h Read Timer Interrupt Register
};

void (* WriteRIOTTab[4])(void) = {
	SetRIOTTimer1,		//  294h
	SetRIOTTimer8,		//  295h
	SetRIOTTimer64,		//  296h
	SetRIOTTimer1024	//  297h
};

void (* WriteRIOTTab2[4])(void) = {
	WritePortA,		//  280h
	WriteDDR_A,		//  281h
	WriteNothing,	//  282h
	WriteDDR_B		//  283h
};


void Init_Riot(void){
	
	Timer = START_TIME;
	TimerReadVec = &ReadTimer1024;
}


/*
	CPU wants to read a RIOT register
*/

void ReadRIOT(void){

	(* ReadRIOTTab[AddressBus & 0x7])();
}

/*
	CPU wants to write to a RIOT register
*/

void WriteRIOT(void){

	if(AddressBus & 0x10){
		if(AddressBus & 0x4) (* WriteRIOTTab[AddressBus & 0x3])();
		else WriteNothing();
	}else{
		if(!(AddressBus & 0x4)) (* WriteRIOTTab2[AddressBus & 0x3])();
		else WriteNothing();
	}
}


/*
	clock the RIOT timer (after every instruction)
	
	gets used in cpu.asm and tialine.asm
*/

void  ClockRIOT(void){
	Timer -= RCycles;
}

void (* WriteRIOTHandler[32])(void) = {
	WritePortA,		//  280h
	WriteDDR_A,		//  281h
	WriteNothing,	//  282h
	WriteDDR_B,		//  283h
	WriteNothing,
	WriteNothing,
	WriteNothing,
	WriteNothing,

	WritePortA,		//  280h
	WriteDDR_A,		//  281h
	WriteNothing,	//  282h
	WriteDDR_B,		//  283h
	WriteNothing,
	WriteNothing,
	WriteNothing,
	WriteNothing,

	WritePortA,		//  280h
	WriteDDR_A,		//  281h
	WriteNothing,	//  282h
	WriteDDR_B,		//  283h
	SetRIOTTimer1,		//  294h
	SetRIOTTimer8,		//  295h
	SetRIOTTimer64,		//  296h
	SetRIOTTimer1024,	//  297h

	WritePortA,		//  280h
	WriteDDR_A,		//  281h
	WriteNothing,	//  282h
	WriteDDR_B,		//  283h
	SetRIOTTimer1,		//  294h
	SetRIOTTimer8,		//  295h
	SetRIOTTimer64,		//  296h
	SetRIOTTimer1024	//  297h
};

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).  
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
