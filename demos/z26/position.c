/*
** position.c -- position rendering window depending on video mode
*/

dd TopLine = 0;    /* top line of display */
dd BottomLine = 0; /* bottom line of display */

void position_game() {
	CFirst = DefaultCFirst;
	if(DefaultCFirst == 0xffff) 	// if no default first line to render
	{
		if(PaletteNumber > 2)	PaletteNumber = 0;
			
		if (PaletteNumber == 0) CFirst = 18;	// NTSC
		else 			CFirst = 38;	// 39 PAL or SECAM
	}
	
	CFirst += GameOffset;	/* add in game specific offset */
	
	OldCFirst = CFirst;	/* remember starting line for homing display */
	TopLine = CFirst;	/* set up in case there's no vsync (like bowg_tw.bin) */
	BottomLine = CFirst + MaxLines;
}

/**
 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/
