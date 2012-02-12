/*

 Starpath Supercharger support for z26

	15 -- Starpath [AR]

	the Supercharger has 2K of ROM and 3 2K banks of RAM
	address $1FF9 is the audio input for the tape loader
	address $1FF8 is the control byte:
	D7 - D5 = write pulse delay (needed for hardware timing - not emulated)
	D4 - D2 = memory config for $1000 - $17FF and $1800 - $1FFF:
	          000 = RAM 3 + ROM
	          001 = RAM 1 + ROM
	          010 = RAM 3 + RAM 1
	          011 = RAM 1 + RAM 3
	          100 = RAM 3 + ROM
	          101 = RAM 2 + ROM
	          110 = RAM 3 + RAM 2
	          111 = RAM 2 + RAM 3
	D1 = 1 -> writing to RAM allowed
	D0 = 1 -> power off ROM (to save energy when it's not needed)
	
	to latch a write value you have to access a address in $1000 - $10FF.
	the low byte of the address will be the value to write.
	the write will happen to the address that is accessed 5 address changes later.
	writes will only happen, if writing is enabled and the destination is in SC-RAM.
	writes to $1FF8 will alway happen immediately. (no address-change counting needed) 

*/

dd SP_Scheme[8][2] = { /* table of bankswitch schemes */
	{2 * 0x800,	3 * 0x800},
	{0 * 0x800,	3 * 0x800},
	{2 * 0x800,	0 * 0x800},
	{0 * 0x800,	2 * 0x800},
	{2 * 0x800,	3 * 0x800},
	{1 * 0x800,	3 * 0x800},
	{2 * 0x800,	1 * 0x800},
	{1 * 0x800,	2 * 0x800}
};

dd SPSlice[2] = { 0, 3 * 0x800 };

dd SP_PrevAdr = 0;

/* byte(s) to write to RAM: */
db SP_RamByte = 0;

//db Starpath = 0;
db SP_WriteEnable = 0;
db SP_AddressCount = 7;


/*
	Starpath initialization
*/

void Init_Starpath(void) {
	
	SPSlice[0] = 0;
	SPSlice[1] = 3*0x800;
	Starpath = 0;
	SP_WriteEnable = 0;
	SP_RamByte = 0;
	SP_AddressCount = 7;
	SP_PrevAdr = 0;
	SC_ControlByte = 0;
}

void SP_SetScheme(void){
	SP_WriteEnable = SC_ControlByte & 0x2;
	SPSlice[0] = SP_Scheme[(SC_ControlByte & 0x1c) >> 2][0];
	SPSlice[1] = SP_Scheme[(SC_ControlByte & 0x1c) >> 2][1];
}

void SetStarpath(void){
	
	BSType = 15;
	Starpath = 1;
	RiotRam[0] = 0x40;	// Starpath loader does this I think
	SP_SetScheme();	
}


/*
	Starpath exit routines
*/

void ShowJAMmessage(void){
	srv_Cleanup();
	sprintf(msg, "JAM instruction %02x @ %04x\n", reg_a, reg_pc);
	srv_print(msg);
	exit(0);
}

void StarpathLoadNotFound(void){
	srv_Cleanup();
	sprintf(msg, "Unable to find load %02x\n", SC_ControlByte);
	srv_print(msg);
	exit(0);
}

void StarpathRealJAM(void){
	srv_Cleanup();
	sprintf(msg, "Starpath call @ %04x\n", reg_pc);
	srv_print(msg);
	exit(0);
}


/*
	Starpath jam handler (game jumped to ROM)
*/

void StarpathJAM(void){

	if((AddressBus & 0x1fff) == 0x1ff0){
		SC_ControlByte = RiotRam[0x7a];		// $FA contains the load number
//		SC_ControlByte = RiotRam[0x01];		// $81 contains the load number
		cli_LoadNextStarpath();
		if(SC_StartAddress == 0) StarpathLoadNotFound();
		else{
			reg_pc = SC_StartAddress;	// tell CPU where to continue
			RiotRam[0] = SC_ControlByte | 0x40; // tell game current memory config
			SP_SetScheme();
		}
	}else if((AddressBus & 0x1fff) == 0x1ff1){
		cli_ReloadStarpath();
		reg_pc = SC_StartAddress;		// tell CPU where to continue
		RiotRam[0] = SC_ControlByte | 0x40;	// tell game current memory config
		SP_SetScheme();
	}else StarpathRealJAM();
}


/*
	process current address
*/

void SP_Q_Adr(void){

	if(SP_AddressCount <= 5){
		if((AddressBus & 0x1fff) != SP_PrevAdr){
			SP_PrevAdr = AddressBus & 0x1fff;
			SP_AddressCount++;
		}if((AddressBus & 0x1fff) == 0x1ff8){
			SC_ControlByte = SP_RamByte;
			SP_SetScheme();
			SP_AddressCount = 7;
		}else if((SP_AddressCount == 5) && (AddressBus & 0x1000) && (SP_WriteEnable)){
			CartRom[SPSlice[(AddressBus & 0x800) >> 11] + (AddressBus & 0x7ff)]
				= SP_RamByte;
			SP_AddressCount = 7;
			if(TraceCount) ShowSCWrite();
		}
	}else if(AddressBus & 0x1000){
		if(!(AddressBus & 0xf00)){
			SP_RamByte = AddressBus & 0xff;
			SP_AddressCount = 0;
			SP_PrevAdr = AddressBus & 0x1fff;
		}else if((AddressBus & 0x1fff) == 0x1ff8){
			SC_ControlByte = SP_RamByte;
			SP_SetScheme();
			SP_AddressCount = 7;
		}
	}
}


/*
	actual bankswitch code
*/

void ReadSPlow(void){

	if(!debugflag) SP_Q_Adr();
	(* TIARIOTReadAccess[AddressBus & 0xfff])();
}

void WriteSPlow(void){

	SP_Q_Adr();
	(* TIARIOTWriteAccess[AddressBus & 0xfff])();
}

void ReadSPhigh(void){

	if(!debugflag) SP_Q_Adr();
	DataBus = 
		CartRom[SPSlice[(AddressBus & 0x800) >> 11] + (AddressBus & 0x7ff)];
}

void WriteSPhigh(void){

	SP_Q_Adr();
}

void InitSP(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = &ReadSPlow;
		WriteAccess[i] = &WriteSPlow;
		ReadAccess[0x1000 + i] = &ReadSPhigh;
		WriteAccess[0x1000 + i] = &WriteSPhigh;
	}
	Copy64K();
}



void RBank_SP(void){

	if(!debugflag) SP_Q_Adr();
	if(!(AddressBus & 0x1000)) ReadHardware();
	else DataBus = 
		CartRom[SPSlice[(AddressBus & 0x800) >> 11] + (AddressBus & 0x7ff)];
}

void WBank_SP(void){

	SP_Q_Adr();
	if(!(AddressBus & 0x1000)) WriteHardware();
}


/**
 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/
