/*

 Pitfall 2 8K bankswitch scheme -- similar to standard F8

	21 -- 8K+DPC Pitfall2 [P2]
	
	2 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF8
	select bank 1 by accessing $1FF9
	the DPC has:
		2K of graphics ROM,
		a random number genarator
		and a sample generator for 3 combined squarewave sound channels
	DPC read ports range from $1000 to $103F
	DPC write ports range from $1040 to $107F

 Based in part on David Crane's U.S. Patent 4,644,495, Feb 17,1987.


 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 contributors.	 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/

//db Pitfall2 = 0;	//  tell RIOT timer to clock the music

db P2_Flags[8] = 		{ 0,0,0,0,0,0,0,0 };
dd P2_Counters[8] = 	{ 0,0,0,0,0,0,0,0 };
db P2_Top[8] = 			{ 0,0,0,0,0,0,0,0 };
db P2_Bottom[8] = 		{ 0,0,0,0,0,0,0,0 };
db P2_Enable[8] = 		{ 0,0,0,0,0,0,0,0 };
dd P2_Music_Top[8] = 	{ 0,0,0,0,0,0,0,0 };
dd P2_Music_Bottom[8] = { 0,0,0,0,0,0,0,0 };
dd P2_Music_Count[8] = 	{ 0,0,0,0,0,0,0,0 };

db P2_Rbyte = 0;	 //  return value for CPU read commands
db P2_AUDV = 0;		 //  create an AUDV byte here

dd P2_sreg = 0;		 //  initialize shift register to non-zero val

/*
 sound mixing table
 convert 3 sound channel bits into an AUDV value
*/
db Mix_AUDV[8] = { 0, 6, 5, 0xb, 4, 0xa, 9, 0xf};



/*
	Pitfall 2 register handlers
*/

/*
	null register read/write
*/

void P2_NoIO(void){

	if(!(AddressBus & 0xfc0)) DataBus = 0;	// only change for read registers
}


/*
 routine to tune the pitch of the music

 We use this to match the Pitfall II music clock to the TIA music clock.

 Due to the discrete nature of this stuff, since the two clocks are not 
 integer multiples of one another, adjustments of the ratio can affect the
 relative pitch of notes in a chord as well as the overall pitch.  So you 
 need to make sure that the important chords sound *nice*.
*/

dd Tune_Music(void){

	return DataBus * 129 / 79;
}

/*
	 write top register
*/

void P2_WriteTop(boid){

	P2_Top[AddressBus & 0x7] = DataBus;
	if((AddressBus & 0x7) > 4)
		P2_Music_Top[AddressBus & 0x7] = Tune_Music();
}

/*
	write bottom register
*/

void P2_WriteBottom(boid){

	P2_Bottom[AddressBus & 0x7] = DataBus;
	if((AddressBus & 0x7) > 4)
		P2_Music_Bottom[AddressBus & 0x7] = Tune_Music();
}

/*
	write counter low
*/

void P2_WriteCounterLow(void){

	P2_Counters[AddressBus & 0x7] =
		(P2_Counters[AddressBus & 07] & 0x700) + DataBus;
	if((AddressBus & 0x7) > 4)
		P2_Music_Count[AddressBus & 0x7] = Tune_Music();
}

/*
	write counter high AND music enable
*/

void P2_WriteCounterHigh(void){

	P2_Counters[AddressBus & 0x7] =
		((P2_Counters[AddressBus & 07] & 0xff) + (DataBus * 256)) & 0x7ff;
	P2_Enable[AddressBus & 0x7] = DataBus;
	P2_Flags[AddressBus & 0x7] = 0;
}

/*
	reset the random number generator
*/

void P2_ResetRandom(void){

	P2_sreg = 1;
}

/*
	read flags
*/

void P2_ReadFlags(void){

	DataBus = P2_Flags[AddressBus & 0x7];
}

/*
	support function to read data via data fetcher
*/

void Read_DF(void){

	if((P2_Counters[AddressBus & 0x7] & 0xff) == P2_Top[AddressBus & 0x7])
		P2_Flags[AddressBus & 0x7] = 0xff;
	if((P2_Counters[AddressBus & 0x7] & 0xff) == P2_Bottom[AddressBus &0x7])
		P2_Flags[AddressBus & 0x7] = 0x0;

	P2_Rbyte = CartRom[0x27ff - P2_Counters[AddressBus & 0x7]];	// yes it *is* magic
	P2_Counters[AddressBus & 0x7]--;

}


/*
	read data via data fetcher
*/

void P2_Read_DF(void){

	Read_DF();
	DataBus = P2_Rbyte;
}

/*
	read data via data fetcher ANDed with flag
*/

void P2_Read_DF_Flag(void){

	Read_DF();
	DataBus = P2_Rbyte & P2_Flags[AddressBus & 0x7];
}	

/*
	Generate a sequence of pseudo-random numbers 255 numbers long
	by emulating an 8-bit shift register with feedback taps at
	positions 4, 5, 6, and 8.
*/

void P2_Read_Random(void){

	P2_sreg =
		(((((P2_sreg & 0x80) >> 7) ^
		   ((P2_sreg & 0x20) >> 5)) ^
		  (((P2_sreg & 0x10) >> 4) ^
		   ((P2_sreg & 0x08) >> 3))) ^ 1) |
		    (P2_sreg << 1);
	DataBus = P2_sreg;
}



/*
	read sound stuff
*/

/*
	read sound entry point

	This is just for show -- Pitfall 2 short-circuits AUDV.
*/

void P2_Read_Sound(void){

	DataBus = P2_AUDV;
}


/*
	clock music -- clock all channels
*/

void Clock_Music(void){

	if(P2_Enable[5] & 0x10){
		if(P2_Music_Count[5] == P2_Music_Bottom[5]){
			P2_Flags[5] = 0;
			P2_Music_Count[5]--;
		}else if(P2_Music_Count[5] == 0xffffffff){
			P2_Music_Count[5] = P2_Music_Top[5];
			P2_Flags[5] = 0xff;
		}else P2_Music_Count[5]--;
	}
	if(P2_Enable[6] & 0x10){
		if(P2_Music_Count[6] == P2_Music_Bottom[6]){
			P2_Flags[6] = 0;
			P2_Music_Count[6]--;
		}else if(P2_Music_Count[6] == 0xffffffff){
			P2_Music_Count[6] = P2_Music_Top[6];
			P2_Flags[6] = 0xff;
		}else P2_Music_Count[6]--;
	}
	if(P2_Enable[7] & 0x10){
		if(P2_Music_Count[7] == P2_Music_Bottom[7]){
			P2_Flags[7] = 0;
			P2_Music_Count[7]--;
		}else if(P2_Music_Count[7] == 0xffffffff){
			P2_Music_Count[7] = P2_Music_Top[7];
			P2_Flags[7] = 0xff;
		}else P2_Music_Count[7]--;
	}
}


/*
	build AUDV byte
*/

/*
	clock Pitfall 2 from TIA sound
*/

void Clock_Pitfall2(void){

	Clock_Music();
	P2_AUDV = Mix_AUDV[(P2_Flags[5] & 0x1) | (P2_Flags[6] & 0x2) | (P2_Flags[7] & 0x4)];
}


/*
	read commands
*/

void (* P2_ReadFunctions[0x40])(void) = {
	P2_Read_Random,		 //  00 -- Random # generator
	P2_Read_Random,		 //  01 -- Random # generator
	P2_Read_Random,		 //  02 -- Random # generator
	P2_Read_Random,		 //  03 -- Random # generator
	P2_Read_Sound,		 //  04 -- Sound value
	P2_Read_Sound,		 //  05 -- Sound value
	P2_Read_Sound,		 //  06 -- Sound value
	P2_Read_Sound,		 //  07 -- Sound value
	P2_Read_DF,			 //  08 -- DF0
	P2_Read_DF,			 //  09 -- DF1
	P2_Read_DF,			 //  0a -- DF2
	P2_Read_DF,			 //  0b -- DF3
	P2_Read_DF,			 //  0c -- DF4
	P2_Read_DF,			 //  0d -- DF5
	P2_Read_DF,			 //  0e -- DF6
	P2_Read_DF,			 //  0f -- DF7
	P2_Read_DF_Flag,	 //  10 -- DF0 AND flag
	P2_Read_DF_Flag,	 //  11 -- DF1 AND flag
	P2_Read_DF_Flag,	 //  12 -- DF2 AND flag
	P2_Read_DF_Flag,	 //  13 -- DF3 AND flag
	P2_Read_DF_Flag,	 //  14 -- DF4 AND flag
	P2_Read_DF_Flag,	 //  15 -- DF5 AND flag
	P2_Read_DF_Flag,	 //  16 -- DF6 AND flag
	P2_Read_DF_Flag,	 //  17 -- DF7 AND flag
	P2_NoIO,			 //  18 -- DF0 AND flag swapped
	P2_NoIO,			 //  19 -- DF1 AND flag swapped
	P2_NoIO,			 //  1a -- DF2 AND flag swapped
	P2_NoIO,			 //  1b -- DF3 AND flag swapped
	P2_NoIO,			 //  1c -- DF4 AND flag swapped
	P2_NoIO,			 //  1d -- DF5 AND flag swapped
	P2_NoIO,			 //  1e -- DF6 AND flag swapped
	P2_NoIO,			 //  1f -- DF7 AND flag swapped
	P2_NoIO,			 //  20 -- DF0 AND flag reversed
	P2_NoIO,			 //  21 -- DF1 AND flag reversed
	P2_NoIO,			 //  22 -- DF2 AND flag reversed
	P2_NoIO,			 //  23 -- DF3 AND flag reversed
	P2_NoIO,			 //  24 -- DF4 AND flag reversed
	P2_NoIO,			 //  25 -- DF5 AND flag reversed
	P2_NoIO,			 //  26 -- DF6 AND flag reversed
	P2_NoIO,			 //  27 -- DF7 AND flag reversed
	P2_NoIO,			 //  28 -- DF0 AND flag SHR 1
	P2_NoIO,			 //  29 -- DF1 AND flag SHR 1
	P2_NoIO,			 //  2a -- DF2 AND flag SHR 1
	P2_NoIO,			 //  2b -- DF3 AND flag SHR 1
	P2_NoIO,			 //  2c -- DF4 AND flag SHR 1
	P2_NoIO,			 //  2d -- DF5 AND flag SHR 1
	P2_NoIO,			 //  2e -- DF6 AND flag SHR 1
	P2_NoIO,			 //  2f -- DF7 AND flag SHR 1
	P2_NoIO,			 //  30 -- DF0 AND flag SHL 1
	P2_NoIO,			 //  31 -- DF1 AND flag SHL 1
	P2_NoIO,			 //  32 -- DF2 AND flag SHL 1
	P2_NoIO,			 //  33 -- DF3 AND flag SHL 1
	P2_NoIO,			 //  34 -- DF4 AND flag SHL 1
	P2_NoIO,			 //  35 -- DF5 AND flag SHL 1
	P2_NoIO,			 //  36 -- DF6 AND flag SHL 1
	P2_NoIO,			 //  37 -- DF7 AND flag SHL 1
	P2_ReadFlags,		 //  38 -- DF0 flag
	P2_ReadFlags,		 //  39 -- DF1 flag
	P2_ReadFlags,		 //  3a -- DF2 flag
	P2_ReadFlags,		 //  3b -- DF3 flag
	P2_ReadFlags,		 //  3c -- DF4 flag
	P2_ReadFlags,		 //  3d -- DF5 flag
	P2_ReadFlags,		 //  3e -- DF6 flag
	P2_ReadFlags		 //  3f -- DF7 flag
};


/*
	write commands
*/

void (* P2_WriteFunctions[0x40])(void) = {
	P2_WriteTop,		 //  40 -- DF0 top count
	P2_WriteTop,		 //  41 -- DF1 top count
	P2_WriteTop,		 //  42 -- DF2 top count
	P2_WriteTop,		 //  43 -- DF3 top count
	P2_WriteTop,		 //  44 -- DF4 top count
	P2_WriteTop,		 //  45 -- DF5 top count
	P2_WriteTop,		 //  46 -- DF6 top count
	P2_WriteTop,		 //  47 -- DF7 top count
	P2_WriteBottom,		 //  48 -- DF0 bottom count
	P2_WriteBottom,		 //  49 -- DF1 bottom count
	P2_WriteBottom,		 //  4a -- DF2 bottom count
	P2_WriteBottom,		 //  4b -- DF3 bottom count
	P2_WriteBottom,		 //  4c -- DF4 bottom count
	P2_WriteBottom,		 //  4d -- DF5 bottom count
	P2_WriteBottom,		 //  4e -- DF6 bottom count
	P2_WriteBottom,		 //  4f -- DF7 bottom count
	P2_WriteCounterLow,	 //  50 -- DF0 counter low
	P2_WriteCounterLow,	 //  51 -- DF1 counter low
	P2_WriteCounterLow,	 //  52 -- DF2 counter low
	P2_WriteCounterLow,	 //  53 -- DF3 counter low
	P2_WriteCounterLow,	 //  54 -- DF4 counter low
	P2_WriteCounterLow,	 //  55 -- DF5 counter low
	P2_WriteCounterLow,	 //  56 -- DF6 counter low
	P2_WriteCounterLow,	 //  57 -- DF7 counter low
	P2_WriteCounterHigh, //  58 -- DF0 counter high
	P2_WriteCounterHigh, //  59 -- DF1 counter high
	P2_WriteCounterHigh, //  5a -- DF2 counter high
	P2_WriteCounterHigh, //  5b -- DF3 counter high
	P2_WriteCounterHigh, //  5c -- DF4 counter high
	P2_WriteCounterHigh, //  5d -- DF5 counter high AND music enable
	P2_WriteCounterHigh, //  5e -- DF6 counter high AND music enable
	P2_WriteCounterHigh, //  5f -- DF7 counter high AND music enable
	P2_NoIO,			 //  60 -- not used (draw line movement)
	P2_NoIO,			 //  61 -- not used (draw line movement)
	P2_NoIO,			 //  62 -- not used (draw line movement)
	P2_NoIO,			 //  63 -- not used (draw line movement)
	P2_NoIO,			 //  64 -- not used (draw line movement)
	P2_NoIO,			 //  65 -- not used (draw line movement)
	P2_NoIO,			 //  66 -- not used (draw line movement)
	P2_NoIO,			 //  67 -- not used (draw line movement)
	P2_NoIO,			 //  68 -- not used
	P2_NoIO,			 //  69 -- not used
	P2_NoIO,			 //  6a -- not used
	P2_NoIO,			 //  6b -- not used
	P2_NoIO,			 //  6c -- not used
	P2_NoIO,			 //  6d -- not used
	P2_NoIO,			 //  6e -- not used
	P2_NoIO,			 //  6f -- not used
	P2_ResetRandom,		 //  70 -- random number generator reset
	P2_ResetRandom,		 //  71 -- random number generator reset
	P2_ResetRandom,		 //  72 -- random number generator reset
	P2_ResetRandom,		 //  73 -- random number generator reset
	P2_ResetRandom,		 //  74 -- random number generator reset
	P2_ResetRandom,		 //  75 -- random number generator reset
	P2_ResetRandom,		 //  76 -- random number generator reset
	P2_ResetRandom,		 //  77 -- random number generator reset
	P2_NoIO,			 //  78 -- not used
	P2_NoIO,			 //  79 -- not used
	P2_NoIO,			 //  7a -- not used
	P2_NoIO,			 //  7b -- not used
	P2_NoIO,			 //  7c -- not used
	P2_NoIO,			 //  7d -- not used
	P2_NoIO,			 //  7e -- not used
	P2_NoIO				 //  7f -- not used
};


/*
	Pitfall 2 initialization
*/

void Init_P2(void){				 //  <-- from init.asm

	int i;
	
	for(i=0; i<8; i++){
		P2_Flags[i] =  0;
		P2_Counters[i] =  0;
		P2_Top[i] = 0;
		P2_Bottom[i] = 0;
		P2_Enable[i] = 0;
		P2_Music_Top[i] = 0;
		P2_Music_Bottom[i] = 0;
		P2_Music_Count[i] = 0;
	}

	Pitfall2 = 0;
	P2_Rbyte = 0;
	P2_AUDV = 0;

	P2_sreg = 1;		 //  random # generator (must be non-zero)
}

void SetPitfallII(void){				 //  <-- from banks.asm
 	BSType = 21;
 	RomBank = 0x1000;	// don't know if this is needed...
 	Pitfall2 = 1;		// tell RIOT to clock the music
}

/*
	bankswitch entry points
*/

void InitP2(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 2; i++){
		ReadAccess[0x1ff8 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff8 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 8;

	for(i = 0; i < 0x40; i++){
		ReadAccess[0x1000 + i] = P2_ReadFunctions[i];
		WriteAccess[0x1000 + i] = P2_ReadFunctions[i];
		WriteAccess[0x1040 + i] = P2_WriteFunctions[i];
		ReadAccess[0x1040 + i] = P2_WriteFunctions[i];
	}
	Copy64K();
}
