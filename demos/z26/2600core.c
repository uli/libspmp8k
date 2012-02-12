/*

	2600core.c -- c_emulator() main entry point here...

*/

void do_Instruction_line(void);
void SetStarpath(void);
void SetPitfallII(void);

#include "c_trace.c"
#include "c_riot.c"
#include "c_tialine.c"
#include "c_tiasnd.c"
#include "c_soundq.c"
#include "c_tiawrite.c"
#include "c_cpuhand.c"
#include "c_banks.c"
#include "c_pitfall2.c"
#include "c_starpath.c"
#include "c_cpujam.c"
#include "c_cpu.c"
#include "c_init.c"

/*
 routine to blank the remains of the screen buffer, if not all of the
 displayed data gets filled by the rendering routine

 compare with TIABlank in tiawrite.asm ???
*/

void BlankBufferEnd(void) {
	int i;
	dd *PTR;
	
	PTR = (dd*) DisplayPointer;
	if(LinesInFrame > 100){	
		if((LinesInFrame - CFirst) < MaxLines){
			for(i=0; i < ((MaxLines - LinesInFrame + CFirst) * 80); i++){
				*PTR++ = 0;
			}
		}
	}
}


/*
	Do one frame
*/

void ScanFrame() {

	/* Reset display pointer */
	DisplayPointer = (dw*) ScreenBuffer;

	do {
		/* Generate a raster line */
		nTIALineTo();

		ScanLine++;

		/* adjust RClock for next line */
		RClock -= CYCLESPERSCANLINE;

		/* if charging capacitors... */
		if( !(VBlank & 0x80) )
			/* and if not fully charged... */
			if(ChargeCounter < 0x80000000)
				/* add some charge. */
				ChargeCounter++;

		if( ScanLine >= OurBailoutLine) {
			OurBailoutLine = BailoutLine;
			PrevLinesInFrame = LinesInFrame;
			LinesInFrame = ScanLine-1;

			Frame++;
			ScanLine = 1;
		}

	} while (Frame == PrevFrame); /* Frame gets updated by tiawrite.asm */

	/* Done with frame. Blank rest of screen buffer, update PrevFrame, and return to caller. */

	//BlankBufferEnd();
	PrevFrame = Frame;
}


void Reset_emulator(void){
	
	ResetEmulator = 0;
	printf("Reset_emulator 1()\n");
	InitData();
	printf("Reset_emulator 2()\n");
	RecognizeCart();
	printf("Reset_emulator 3()\n");
	SetupBanks();
	printf("Reset_emulator 4()\n");
	Reset();
	printf("Reset_emulator 5()\n");
	Init_Service();
	printf("Reset_emulator 6()\n");
	Controls();
}


/* 
	Entry point. 
*/

void c_emulator(void) {

	Reset_emulator();

	/* ExitEmulator gets set by Controls() if the user presses Escape */
	while( !ExitEmulator ) 
	{
		if(ResetEmulator) Reset_emulator();

		srv_Events();
		if(srv_done) break;	/* SDL got a 'close window' message */

		ScanFrame();
		Controls();
		srv_CopyScreen();
		while(GamePaused) Controls();
	}
	
	srv_Cleanup();	/* shutdown SDL */
}


/**
 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  
 z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/
