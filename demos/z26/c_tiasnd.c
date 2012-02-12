/*

	tiasnd.c -- z26 sound generation routines

	based on TIASound (c) 1996-1997 by Ron Fries
	please see the end of this file for Ron's banner

*/

extern void Clock_Pitfall2(void);
extern db P2_AUDV;
extern db Pitfall2;
extern db DataBus;

/* CONSTANT DEFINITIONS */

/* definitions for AUDCx (15, 16) */
#define SET_TO_1     0x00      /* 0000 */
#define POLY4        0x01      /* 0001 */
#define DIV31_POLY4  0x02      /* 0010 */
#define POLY5_POLY4  0x03      /* 0011 */
#define PURE1        0x04      /* 0100 */
#define PURE2        0x05      /* 0101 */
#define DIV31_PURE   0x06      /* 0110 */
#define POLY5_2      0x07      /* 0111 */
#define POLY9        0x08      /* 1000 */
#define POLY5        0x09      /* 1001 */
#define DIV31_POLY5  0x0a      /* 1010 */
#define POLY5_POLY5  0x0b      /* 1011 */
#define DIV3_PURE    0x0c      /* 1100 */
#define DIV3_PURE2   0x0d      /* 1101 */
#define DIV93_PURE   0x0e      /* 1110 */
#define DIV3_POLY5   0x0f      /* 1111 */
                 
#define DIV3_MASK    0x0c                 
                 
#define AUDC0        0x15
#define AUDC1        0x16
#define AUDF0        0x17
#define AUDF1        0x18
#define AUDV0        0x19
#define AUDV1        0x1a

/* the size (in entries) of the 4 polynomial tables */
#define POLY4_SIZE  0x000f
#define POLY5_SIZE  0x001f
#define POLY9_SIZE  0x01ff

/* channel definitions */
#define CHAN1       0
#define CHAN2       1

#define FALSE       0
#define TRUE        1

int TSB_result;

/*
 Initialze the bit patterns for the polynomials.

 The 4bit and 5bit patterns are the identical ones used in the tia chip.
 Though the patterns could be packed with 8 bits per byte, using only a
 single bit per byte keeps the math simple, which is important for
 efficient processing.
*/

db Bit4[] = { 0,1,1,0,0,1,0,1,0,0,0,0,1,1,1 };
db Bit5[] = { 0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1,1,0,1,0,0,1,1 };

/*
 1 = toggle output in 5 bit poly - used when poly5 clocks other outputs
*/

db Bit5T[] = { 1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0 };

/*
 The 'Div by 31' counter is treated as another polynomial because of
 the way it operates.  It does not have a 50% duty cycle, but instead
 has a 13:18 ratio (of course, 13+18 = 31).  This could also be
 implemented by using counters.
*/

db Div31[] = { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0 };

db Div6[] = { 0,1,0,0,1,0 };

/*
 The sample output is treated as another divide by N counter.
 For better accuracy, the Samp_n_cnt has a fixed binary decimal point
 which has 8 binary digits to the right of the decimal point.
*/

dd Samp_n_cnt = 0;
dd Samp_n_max = 0;


db D6[] = { 0, 0 };
db P4[] = { 0, 0 };
db P5[] = { 0, 0 };
db AUDC[] = { 0, 0 };
db AUDF[] = { 0, 0 };
db AUDV[] = { 0, 0 };
db Outvol[] = { 0, 0 };
db Div_n_cnt[] = { 0, 0 };
db Div_n_max[] = { 0, 0 };

dd P9_sreg[] = { 1, 1 };
dd prev_sample = 0;
db next_sample = 0;


void TSB_Always_High(int chan){
//	Outvol[chan] = AUDV[chan];
}
	
void TSB_Poly4(int chan){
	P4[chan]++;
	if(P4[chan] == 15) P4[chan] = 0;
	if(Bit4[P4[chan]] == 0) Outvol[chan] = 0;
	else Outvol[chan] = AUDV[chan];
}
	
void TSB_Div31_Poly4(int chan){
	if(Div31[P5[chan]] == 1){
		P4[chan]++;
		if(P4[chan] == 15) P4[chan] = 0;
		if(Bit4[P4[chan]] == 0) Outvol[chan] = 0;
		else Outvol[chan] = AUDV[chan];
	}
}
	
void TSB_Poly5_Poly4(int chan){
	if(Bit5T[P5[chan]] == 1){
		P4[chan]++;
		if(P4[chan] == 15) P4[chan] = 0;
		if(Bit4[P4[chan]] == 0) Outvol[chan] = 0;
		else Outvol[chan] = AUDV[chan];
	}
}
	
void TSB_Pure(int chan){
	if(Outvol[chan] == 0) Outvol[chan] = AUDV[chan];
	else Outvol[chan] = 0;
}
	
void TSB_Div31_Pure(int chan){
	if(Div31[P5[chan]] == 1){
		if(Outvol[chan] == 0) Outvol[chan] = AUDV[chan];
		else Outvol[chan] = 0;
	}
}
	
void TSB_Poly5(int chan){
	if(Bit5[P5[chan]] == 1) Outvol[chan] = AUDV[chan];
	else Outvol[chan] = 0;
}
	
void TSB_Poly9(int chan){
/*
 generate a sequence of pseudo-random bits 511 bits long
 by emulating a 9-bit shift register with feedback taps at
 positions 5 and 9.
*/
	P9_sreg[chan] = (P9_sreg[chan] >> 1) |
		(((P9_sreg[chan] & 0x1) ^ ((P9_sreg[chan] & 0x10) >> 4)) << 8);
	if(P9_sreg[chan] & 1) Outvol[chan] = AUDV[chan];
	else Outvol[chan] = 0;
}
	
void TSB_Poly5_Poly5(int chan){
	if(Bit5[P5[chan]] == 1){
		if(Bit5[P5[chan]] == 1) Outvol[chan] = AUDV[chan];
		else Outvol[chan] = 0;
	}
}
	
void TSB_Div6_Pure(int chan){
	D6[chan]++;
	if(D6[chan] == 6) D6[chan] = 0;
	if(Div6[D6[chan]] == 1){
		if(Outvol[chan] == 0) Outvol[chan] = AUDV[chan];
		else Outvol[chan] = 0;
	}
}
	
void TSB_Div31_Div6(int chan){
	if(Div31[P5[chan]] == 1){
		D6[chan]++;
		if(D6[chan] == 6) D6[chan] = 0;
		if(Div6[D6[chan]] == 1){
			if(Outvol[chan] == 0) Outvol[chan] = AUDV[chan];
			else Outvol[chan] = 0;
		}
	}
}
	
void TSB_Poly5_Div6(int chan){
	if(Bit5T[P5[chan]] == 1){
		D6[chan]++;
		if(D6[chan] == 6) D6[chan] = 0;
		if(Div6[D6[chan]] == 1){
			if(Outvol[chan] == 0) Outvol[chan] = AUDV[chan];
			else Outvol[chan] = 0;
		}
	}
}	

/*
AUDC_Jumptab:   ; dword           ; HEX  D3 D2 D1 D0    Clock Source    Clock Modifier    Source Pattern
		;                   --- -------------  --------------  ----------------  ----------------
        dd      TSB_Ch0done      ;   0    0  0  0  0    3.58 MHz/114 ->  none  (pure)  ->      none
        dd      TSB_Poly4        ;   1    0  0  0  1    3.58 MHz/114 ->  none  (pure)  ->   4-bit poly
        dd      TSB_Div31_Poly4  ;   2    0  0  1  0    3.58 MHz/114 ->  divide by 31  ->   4-bit poly
        dd      TSB_Poly5_Poly4  ;   3    0  0  1  1    3.58 MHz/114 ->   5-bit poly   ->   4-bit poly
        dd      TSB_Pure         ;   4    0  1  0  0    3.58 MHz/114 ->  none  (pure)  ->   pure  (~Q)
        dd      TSB_Pure         ;   5    0  1  0  1    3.58 MHz/114 ->  none  (pure)  ->   pure  (~Q)
        dd      TSB_Div31_Pure   ;   6    0  1  1  0    3.58 MHz/114 ->  divide by 31  ->   pure  (~Q)
;       dd      TSB_Poly5_Pure   ;   7    0  1  1  1    3.58 MHz/114 ->   5-bit poly   ->   pure  (~Q)
        dd      TSB_Poly5        ;   7    0  1  1  1    3.58 MHz/114 ->  none  (pure)  ->   5-bit poly
        dd      TSB_Poly9        ;   8    1  0  0  0    3.58 MHz/114 ->  none  (pure)  ->   9-bit poly
        dd      TSB_Poly5        ;   9    1  0  0  1    3.58 MHz/114 ->  none  (pure)  ->   5-bit poly
;       dd      TSB_Div31_Poly5  ;   A    1  0  1  0    3.58 MHz/114 ->  divide by 31  ->   5-bit poly
        dd      TSB_Div31_Pure   ;   A    1  0  1  0    3.58 MHz/114 ->  divide by 31  ->   pure  (~Q)
        dd      TSB_Poly5_Poly5  ;   B    1  0  1  1    3.58 MHz/114 ->   5-bit poly   ->   5-bit poly
        dd      TSB_Div6_Pure    ;   C    1  1  0  0    3.58 MHz/114 ->  divide by 6   ->   pure  (~Q)
        dd      TSB_Div6_Pure    ;   D    1  1  0  1    3.58 MHz/114 ->  divide by 6   ->   pure  (~Q)
        dd      TSB_Div31_Div6   ;   E    1  1  1  0    3.58 MHz/114 ->  divide by 31  ->   divide by 6
        dd      TSB_Poly5_Div6   ;   F    1  1  1  1    3.58 MHz/114 ->   5-bit poly   ->   divide by 6
*/

void (* AUDC_Jumptab[])(int chan) = {
	TSB_Always_High,
	TSB_Poly4,
	TSB_Div31_Poly4,
	TSB_Poly5_Poly4,
	TSB_Pure,
	TSB_Pure,
	TSB_Div31_Pure,
	TSB_Poly5,
	TSB_Poly9,
	TSB_Poly5,
	TSB_Div31_Pure,
	TSB_Poly5_Poly5,
	TSB_Div6_Pure,
	TSB_Div6_Pure,
	TSB_Div31_Div6,
	TSB_Poly5_Div6
};


/*
 	handle the power-up initialization functions
 	these functions should only be executed on a cold-restart

 calculate the sample 'divide by N' value based on the playback freq
*/

void Init_Tiasnd() {
	int chan;
	
	Samp_n_max = (31400 << 8) / 31400;
	Samp_n_cnt = 0;

	for (chan = CHAN1; chan <= CHAN2; chan++)
	{
		Outvol[chan] =	0;
		Div_n_cnt[chan]	= 0;
		Div_n_max[chan]	= 0;
		AUDC[chan] = 0;
		AUDF[chan] = 0;
		AUDV[chan] = 0;
		P4[chan] = 0;
		P5[chan] = 0;
		P9_sreg[chan] =	1;
	}
}

/*
	update TIA sound registers
*/

void UTS_Chan(int chan) {
	dd new_val;

	if(AUDC[chan] == 0) {
		new_val = 0;
		Outvol[chan] = AUDV[chan];
	} else new_val = AUDF[chan] + 1;

	if( new_val != Div_n_max[chan] ) {
		Div_n_max[chan] = new_val & 0xff;
		if( Div_n_cnt[chan] == 0 || new_val == 0 )
			Div_n_cnt[chan] = new_val & 0xff;
	}
}


void H_AUDC0() {
	AUDC[0] = DataBus & 0x0f;
	UTS_Chan(0);
}


void H_AUDC1() {
	AUDC[1] = DataBus & 0x0f;
	UTS_Chan(1);
}


void H_AUDF0() {
	AUDF[0] = DataBus & 0x1f;
	UTS_Chan(0);
}


void H_AUDF1() {
	AUDF[1] = DataBus & 0x1f;
	UTS_Chan(1);
}


void H_AUDV0() {
	AUDV[0] = (DataBus & 0xf) << 3;
	UTS_Chan(0);
}


void H_AUDV1() {
	AUDV[1] = (DataBus & 0xf) << 3;
	UTS_Chan(1);
}


/*
	generate a sound byte based on the TIA chip parameters
*/

void TSB_ProcessChannel(int chan) {
	if(Div_n_cnt[chan] < 1) return;
	if(Div_n_cnt[chan] > 1) {
		Div_n_cnt[chan]--;
	} else {
		/* reset the counter */
		Div_n_cnt[chan] = Div_n_max[chan];

		/* the P5 counter has multiple uses, so we inc it here */
		P5[chan]++;
		if (P5[chan] == POLY5_SIZE)
			P5[chan] = 0;

		(* AUDC_Jumptab[AUDC[chan] & 0xf])(chan);
	}
}

void TSB_Ch0done() {
	/* decrement the sample counter - value is 256 since the lower
		byte contains the fractional part */
	Samp_n_cnt -= 256;

	/* if the count down has reached zero */
	if (Samp_n_cnt < 256)
	{
		/* adjust the sample counter */
		Samp_n_cnt += Samp_n_max;

		/* calculate the latest output value and set our result */
		if(Pitfall2){
			Clock_Pitfall2();
			TSB_result = ((P2_AUDV & 0xf) << 3) + Outvol[1];
		}else TSB_result = Outvol[0] + Outvol[1];
		if(GamePaused) TSB_result = 0x80;

		if(dsp){	/* doing digital signal processing ? */
			/* take edge of square wave */
			next_sample = TSB_result & 0xff;
			/* dsp = 2: scaled moving average */
			prev_sample = (prev_sample + next_sample) >> 1;
			TSB_result = prev_sample & 0xff;
			/* dsp = 1: simple moving average */
			if(dsp == 1) prev_sample = next_sample;
		}
	}
}

dd TIA_Sound_Byte() {
	TSB_result = -1;

	while(TSB_result==-1) {
		if(!Pitfall2) TSB_ProcessChannel(0);
		TSB_ProcessChannel(1);
		TSB_Ch0done(); /* sets TSB_result for us */
	}
	return TSB_result;
}


/*****************************************************************************/
/*                                                                           */
/*                 License Information and Copyright Notice                  */
/*                 ========================================                  */
/*                                                                           */
/* TiaSound is Copyright(c) 1996 by Ron Fries                                */
/*                                                                           */
/* This library is free software; you can redistribute it and/or modify it   */
/* under the terms of version 2 of the GNU Library General Public License    */
/* as published by the Free Software Foundation.                             */
/*                                                                           */
/* This library is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY WARRANTY; without even the implied warranty of                */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library */
/* General Public License for more details.                                  */
/* To obtain a copy of the GNU Library General Public License, write to the  */
/* Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.   */
/*                                                                           */
/* Any permitted reproduction of these routines, in whole or in part, must   */
/* bear this legend.                                                         */
/*                                                                           */
/*****************************************************************************/

/**
 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/
