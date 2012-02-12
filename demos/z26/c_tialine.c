/*

	tialine.c -- generate a raster line


	magic numbers:

		HBLANK starts at zero, ends at 67.
		Left Playfield starts at 68, ends at 147.
		Right Playfield starts at 148, ends at 227.
		HMOVE blank starts at 68, ends at 75.

 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 contributors.	 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/


/* just to be able to compile with trace */
dd P0_Position = 0;
dd P1_Position = 0;
dd M0_Position = 0;
dd M1_Position = 0;
dd BL_Position = 0;


dd TIACollide = 0;	// state of the 15 collision bits

dd TIA_Last_Pixel = 0;	// remember how far we have rendered this line
db TIA_Display_HBlank = 0;	// show HMOVE bar in this line
dd TIA_HMOVE_Setup = 0;	// it takes several steps to setup an HMOVE
dd TIA_HMOVE_Clock = 16;	// how many HMOVE clocks since last HMOVE command
db TIA_HMOVE_Latches = 0;	// are we done with HMOVE pulses
db TIA_HMOVE_DoMove	= 0;	// do we need to apply more HMOVE pulses
/*
for continuity with TIA_Pixel_State:
P0 = 0x20
P1 = 0x10
M0 = 0x08
M1 = 0x04
BL = 0x02
*/
// how far to move object with HMOVE (0-15)
db TIA_HMP0_Value = 8;
db TIA_HMP1_Value = 8;
db TIA_HMM0_Value = 8;
db TIA_HMM1_Value = 8;
db TIA_HMBL_Value = 8;
db TIA_Pixel_State = 0;	// which objects to draw in this pixel (one bit each)
db TIA_Mask_Objects = 0x3f;	// enable VBLANK and let user disable objects
db TIA_Do_Output = 1;	// if line is offscreen, don't copy pixels to buffer
db CTRLPF_PF_Reflect = 0;	// reflect PF in right half of screen
db Current_PF_Pixel = 0;	// state of currently displayed PF pixel
// how wide are we supposed to display this pixel
db NUSIZ_P0_width = 0;
db NUSIZ_P1_width = 0;
db NUSIZ_M0_width = 0;
db NUSIZ_M1_width = 0;
db CTRLPF_BL_width = 0;
// reset missile to pixel 4 of player main copy
db TIA_RESMP0 = 0;
db TIA_RESMP1 = 0;
// use 'old' object data
db TIA_VDELP0 = 0;
db TIA_VDELP1 = 0;
db TIA_VDELBL = 0;
// reflect player
dd TIA_REFP0 = 0;
dd TIA_REFP1 = 0;
// graphics data for objects
dd TIA_GRP0_new = 0;
dd TIA_GRP0_old = 0;
dd TIA_GRP1_new = 0;
dd TIA_GRP1_old = 0;
dd TIA_ENAM0 = 0;
dd TIA_ENAM1 = 0;
dd TIA_ENABL_new = 0;
dd TIA_ENABL_old = 0;
// which object copies do we show
db NUSIZ0_number = 0;
db NUSIZ1_number = 0;
db CTRLPF_Score = 0;	// use score mode colours for playfield
db CTRLPF_Priority = 0;	// has playfield priority over movable objects
// object pixel counters for visible part of screen
dd TIA_P0_counter = 0;
dd TIA_P1_counter = 0;
dd TIA_M0_counter = 0;
dd TIA_M1_counter = 0;
dd TIA_BL_counter = 0;
// VBLANK, GRP0, GRP1, ENAM0, ENAM1, ENABL, REFP0, REFP1, PF0, PF1, PF2
// need one extra pixel for the write to take effect
// -> render one pixel ahead of counter before updating register state
db TIA_Delayed_Write = 0;

#define BG_COLOUR 0
#define PF_COLOUR 1
#define P1_COLOUR 2
#define P0_COLOUR 3

dw TIA_Colour_Table[4] = {0, 0, 0, 0};

db TIA_Priority_Table[2][64];
db TIA_Score_Priority_Table[2][64];

db ObjectStartTable[8][320];


dd TIA_P0_counter_reset = 0;
dd TIA_P1_counter_reset = 0;
dd TIA_M0_counter_reset = 0;
dd TIA_M1_counter_reset = 0;
dd TIA_BL_counter_reset = 0;

dd TIA_Playfield_Value = 0;
dd TIA_REFPF_Flag = 0;
db TIA_VBLANK = 0;

dw LoopCount;	// counter for the CatchUpPixels loop
dw CountLoop;

dd Pointer_Index_P0 = 0;
dd Pointer_Index_P1 = 0;
dd Pointer_Index_M0 = 0;
dd Pointer_Index_M1 = 0;
dd Pointer_Index_BL = 0;

db *TIA_P0_Line_Pointer;
db *TIA_P1_Line_Pointer;
db *TIA_M0_Line_Pointer;
db *TIA_M1_Line_Pointer;
db *TIA_BL_Line_Pointer;

db *TIA_P0_Table[0x2001];
db *TIA_P1_Table[0x2001];
db *TIA_M0_Table[0x81];
db *TIA_M1_Table[0x81];
db *TIA_BL_Table[9];

dd TIA_Playfield_Pixels[80] = {
	//repeated
	0x00100000, 0x00200000, 0x00400000, 0x00800000,	// PF0
	0x00008000, 0x00004000, 0x00002000, 0x00001000,	// PF1
	0x00000800, 0x00000400, 0x00000200, 0x00000100,	// PF1
	0x00000001, 0x00000002, 0x00000004, 0x00000008,	// PF2
	0x00000010, 0x00000020, 0x00000040, 0x00000080,	// PF2

	0x00100000, 0x00200000, 0x00400000, 0x00800000,	// PF0
	0x00008000, 0x00004000, 0x00002000, 0x00001000,	// PF1
	0x00000800, 0x00000400, 0x00000200, 0x00000100,	// PF1
	0x00000001, 0x00000002, 0x00000004, 0x00000008,	// PF2
	0x00000010, 0x00000020, 0x00000040, 0x00000080,	// PF2
	//reflected
	0x00100000, 0x00200000, 0x00400000, 0x00800000,	// PF0
	0x00008000, 0x00004000, 0x00002000, 0x00001000,	// PF1
	0x00000800, 0x00000400, 0x00000200, 0x00000100,	// PF1
	0x00000001, 0x00000002, 0x00000004, 0x00000008,	// PF2
	0x00000010, 0x00000020, 0x00000040, 0x00000080,	// PF2

	0x00000080, 0x00000040, 0x00000020, 0x00000010,	// PF2
	0x00000008, 0x00000004, 0x00000002, 0x00000001,	// PF2
	0x00000100, 0x00000200, 0x00000400, 0x00000800,	// PF1
	0x00001000, 0x00002000, 0x00004000, 0x00008000,	// PF1
	0x00800000, 0x00400000, 0x00200000, 0x00100000	// PF0
};
dd TIA_Playfield_Bits = 0;	// all 3 PF register values (0x00 PF0 PF1 PF2)

// where to draw missiles; used for generating the TIA_Mx_Tables
db Object_Table[8][4] = {
	{4,  0,  0,  0},
	{4, 20,  0,  0},
	{4,  0, 36,  0},
	{4, 20, 36,  0},
	{4,  0,  0, 68},
	{4,  0,  0,  0},
	{4,  0, 36, 68},
	{4,  0,  0,  0}
};

dw TIA_Collision_Table[64] = {
	0x0000, 0x0000, 0x0000, 0x2000, 0x0000, 0x0800, 0x0400, 0x2c00,
	0x0000, 0x0200, 0x0100, 0x2300, 0x4000, 0x4a00, 0x4500, 0x6f00,
	0x0000, 0x0080, 0x0040, 0x20c0, 0x0004, 0x0884, 0x0444, 0x2cc4,
	0x0002, 0x0282, 0x0142, 0x23c2, 0x4006, 0x4a26, 0x4546, 0x6f66,
	0x0000, 0x0020, 0x0010, 0x2030, 0x0008, 0x0828, 0x0418, 0x2c38,
	0x0001, 0x0221, 0x0111, 0x2331, 0x4009, 0x4a29, 0x4519, 0x6f39,
	0x8000, 0x80a0, 0x8050, 0xa0f0, 0x800c, 0x88ac, 0x845c, 0xacfc,
	0x8003, 0x82a3, 0x8153, 0xa3f3, 0xc00f, 0xcaaf, 0xc55f, 0xefff
};

/*
P0 P1 M0 M1 BL PF   P0P1 M0M1 BLPF ---- M1PF M1BL M0PF M0BL P1PF P1BL P0PF P0BL M1P0 M1P1 M0P1 M0P0
 0  0  0  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  0  0  0  0  1      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  0  0  0  1  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  0  0  0  1  1      0    0    1    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     2000

 0  0  0  1  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  0  0  1  0  1      0    0    0    0 .. 1    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0800
 0  0  0  1  1  0      0    0    0    0 .. 0    1    0    0 .. 0    0    0    0 .. 0    0    0    0     0400
 0  0  0  1  1  1      0    0    1    0 .. 1    1    0    0 .. 0    0    0    0 .. 0    0    0    0     2c00

 0  0  1  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  0  1  0  0  1      0    0    0    0 .. 0    0    1    0 .. 0    0    0    0 .. 0    0    0    0     0200
 0  0  1  0  1  0      0    0    0    0 .. 0    0    0    1 .. 0    0    0    0 .. 0    0    0    0     0100
 0  0  1  0  1  1      0    0    1    0 .. 0    0    1    1 .. 0    0    0    0 .. 0    0    0    0     2300

 0  0  1  1  0  0      0    1    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     4000
 0  0  1  1  0  1      0    1    0    0 .. 1    0    1    0 .. 0    0    0    0 .. 0    0    0    0     4a00
 0  0  1  1  1  0      0    1    0    0 .. 0    1    0    1 .. 0    0    0    0 .. 0    0    0    0     4500
 0  0  1  1  1  1      0    1    1    0 .. 1    1    1    1 .. 0    0    0    0 .. 0    0    0    0     6f00

 0  1  0  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 0  1  0  0  0  1      0    0    0    0 .. 0    0    0    0 .. 1    0    0    0 .. 0    0    0    0     0080
 0  1  0  0  1  0      0    0    0    0 .. 0    0    0    0 .. 0    1    0    0 .. 0    0    0    0     0040
 0  1  0  0  1  1      0    0    1    0 .. 0    0    0    0 .. 1    1    0    0 .. 0    0    0    0     20c0

 0  1  0  1  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    1    0    0     0004
 0  1  0  1  0  1      0    0    0    0 .. 1    0    0    0 .. 1    0    0    0 .. 0    1    0    0     0884
 0  1  0  1  1  0      0    0    0    0 .. 0    1    0    0 .. 0    1    0    0 .. 0    1    0    0     0444
 0  1  0  1  1  1      0    0    1    0 .. 1    1    0    0 .. 1    1    0    0 .. 0    1    0    0     2cc4

 0  1  1  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    1    0     0002
 0  1  1  0  0  1      0    0    0    0 .. 0    0    1    0 .. 1    0    0    0 .. 0    0    1    0     0282
 0  1  1  0  1  0      0    0    0    0 .. 0    0    0    1 .. 0    1    0    0 .. 0    0    1    0     0142
 0  1  1  0  1  1      0    0    1    0 .. 0    0    1    1 .. 1    1    0    0 .. 0    0    1    0     23c2

 0  1  1  1  0  0      0    1    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    1    1    0     4006
 0  1  1  1  0  1      0    1    0    0 .. 1    0    1    0 .. 0    0    1    0 .. 0    1    1    0     4a26
 0  1  1  1  1  0      0    1    0    0 .. 0    1    0    1 .. 0    1    0    0 .. 0    1    1    0     4546
 0  1  1  1  1  1      0    1    1    0 .. 1    1    1    1 .. 0    1    1    0 .. 0    1    1    0     6f66

 1  0  0  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     0000
 1  0  0  0  0  1      0    0    0    0 .. 0    0    0    0 .. 0    0    1    0 .. 0    0    0    0     0020
 1  0  0  0  1  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    1 .. 0    0    0    0     0010
 1  0  0  0  1  1      0    0    1    0 .. 0    0    0    0 .. 0    0    1    1 .. 0    0    0    0     2030

 1  0  0  1  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 1    0    0    0     0008
 1  0  0  1  0  1      0    0    0    0 .. 1    0    0    0 .. 0    0    1    0 .. 1    0    0    0     0828
 1  0  0  1  1  0      0    0    0    0 .. 0    1    0    0 .. 0    0    0    1 .. 1    0    0    0     0418
 1  0  0  1  1  1      0    0    1    0 .. 1    1    0    0 .. 0    0    1    1 .. 1    0    0    0     2c38

 1  0  1  0  0  0      0    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    1     0001
 1  0  1  0  0  1      0    0    0    0 .. 0    0    1    0 .. 0    0    1    0 .. 0    0    0    1     0221
 1  0  1  0  1  0      0    0    0    0 .. 0    0    0    1 .. 0    0    0    1 .. 0    0    0    1     0111
 1  0  1  0  1  1      0    0    1    0 .. 0    0    1    1 .. 0    0    1    1 .. 0    0    0    1     2331

 1  0  1  1  0  0      0    1    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 1    0    0    1     4009
 1  0  1  1  0  1      0    1    0    0 .. 1    0    1    0 .. 0    0    1    0 .. 1    0    0    1     4a29
 1  0  1  1  1  0      0    1    0    0 .. 0    1    0    1 .. 0    0    0    1 .. 1    0    0    1     4519
 1  0  1  1  1  1      0    1    1    0 .. 1    1    1    1 .. 0    0    1    1 .. 1    0    0    1     6f39

 1  1  0  0  0  0      1    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    0    0     8000
 1  1  0  0  0  1      1    0    0    0 .. 0    0    0    0 .. 1    0    1    0 .. 0    0    0    0     80a0
 1  1  0  0  1  0      1    0    0    0 .. 0    0    0    0 .. 0    1    0    1 .. 0    0    0    0     8050
 1  1  0  0  1  1      1    0    1    0 .. 0    0    0    0 .. 1    1    1    1 .. 0    0    0    0     a0f0

 1  1  0  1  0  0      1    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 1    1    0    0     800c
 1  1  0  1  0  1      1    0    0    0 .. 1    0    0    0 .. 1    0    1    0 .. 1    1    0    0     88ac
 1  1  0  1  1  0      1    0    0    0 .. 0    1    0    0 .. 0    1    0    1 .. 1    1    0    0     845c
 1  1  0  1  1  1      1    0    1    0 .. 1    1    0    0 .. 1    1    1    1 .. 1    1    0    0     acfc

 1  1  1  0  0  0      1    0    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 0    0    1    1     8003
 1  1  1  0  0  1      1    0    0    0 .. 0    0    1    0 .. 1    0    1    0 .. 0    0    1    1     82a3
 1  1  1  0  1  0      1    0    0    0 .. 0    0    0    1 .. 0    1    0    1 .. 0    0    1    1     8153
 1  1  1  0  1  1      1    0    1    0 .. 0    0    1    1 .. 1    1    1    1 .. 0    0    1    1     a3f3

 1  1  1  1  0  0      1    1    0    0 .. 0    0    0    0 .. 0    0    0    0 .. 1    1    1    1     c00f
 1  1  1  1  0  1      1    1    0    0 .. 1    0    1    0 .. 1    0    1    0 .. 1    1    1    1     caaf
 1  1  1  1  1  0      1    1    0    0 .. 0    1    0    1 .. 0    1    0    1 .. 1    1    1    1     c55f
 1  1  1  1  1  1      1    1    1    0 .. 1    1    1    1 .. 1    1    1    1 .. 1    1    1    1     efff
P0 P1 M0 M1 BL PF   P0P1 M0M1 BLPF ---- M1PF M1BL M0PF M0BL P1PF P1BL P0PF P0BL M1P0 M1P1 M0P1 M0P0

*/

void Init_TIA(void){
	
	int	i, j, k, l, m, n;

	TIACollide = 0;	// reset collision latches

 	for(i = 0; i < 0x2001; i++){
		TIA_P0_Table[i] = malloc(160);
		if(TIA_P0_Table[i] == NULL){
			sprintf(msg, "Not enough memory available to run z26.\n");
			srv_print(msg);
			exit(1);
		}
		TIA_P1_Table[i] = malloc(160);
		if(TIA_P1_Table[i] == NULL){
			sprintf(msg, "Not enough memory available to run z26.\n");
			srv_print(msg);
			exit(1);
		}
		for(j = 0; j < 160; j++){
			TIA_P0_Table[i][j] = 0;
			TIA_P1_Table[i][j] = 0;
		}
		switch(i & 0x100f){
			case 0x0000:
			// one copy, close spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
					}
				}
			break;
			case 0x0001:
			// two copies, close spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
					}
				}
			break;
			case 0x0002:
			// two copies, medium spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x0003:
			// three copies, close spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x0004:
			// two copies, far spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x0005:
			// one copy, close spacing, single width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j * 2 + 6] = 0x20;
//						TIA_P0_Table[i][j * 2 + 7] = 0x20;
//						TIA_P1_Table[i][j * 2 + 6] = 0x10;
//						TIA_P1_Table[i][j * 2 + 7] = 0x10;
					}
				}
			break;
			case 0x0006:
			// three copies, medium spacing, double width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x0007:
			// one copy, close spacing, quadrouple width,
			// not reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
//						TIA_P0_Table[i][j * 4 + 6] = 0x20;
//						TIA_P0_Table[i][j * 4 + 7] = 0x20;
//						TIA_P0_Table[i][j * 4 + 8] = 0x20;
//						TIA_P0_Table[i][j * 4 + 9] = 0x20;
//						TIA_P1_Table[i][j * 4 + 6] = 0x10;
//						TIA_P1_Table[i][j * 4 + 7] = 0x10;
//						TIA_P1_Table[i][j * 4 + 8] = 0x10;
//						TIA_P1_Table[i][j * 4 + 9] = 0x10;
					}
				}
			break;
			case 0x0008:
			// one copy, close spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
					}
				}
			break;
			case 0x0009:
			// two copies, close spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
					}
				}
			break;
			case 0x000a:
			// two copies, medium spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x000b:
			// three copies, close spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x000c:
			// two copies, far spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x000d:
			// one copy, close spacing, single width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j * 2 + 6] = 0x20;
						TIA_P0_Table[i][j * 2 + 7] = 0x20;
						TIA_P1_Table[i][j * 2 + 6] = 0x10;
						TIA_P1_Table[i][j * 2 + 7] = 0x10;
					}
				}
			break;
			case 0x000e:
			// three copies, medium spacing, double width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x000f:
			// one copy, close spacing, quadrouple width,
			// not reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0800 >> j)){
						TIA_P0_Table[i][j * 4 + 6] = 0x20;
						TIA_P0_Table[i][j * 4 + 7] = 0x20;
						TIA_P0_Table[i][j * 4 + 8] = 0x20;
						TIA_P0_Table[i][j * 4 + 9] = 0x20;
						TIA_P1_Table[i][j * 4 + 6] = 0x10;
						TIA_P1_Table[i][j * 4 + 7] = 0x10;
						TIA_P1_Table[i][j * 4 + 8] = 0x10;
						TIA_P1_Table[i][j * 4 + 9] = 0x10;
					}
				}
			break;
			case 0x1000:
			// one copy, close spacing, single width,
			// reflected, don't show first copy
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
					}
				}
			break;
			case 0x1001:
			// two copies, close spacing, single width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
					}
				}
			break;
			case 0x1002:
			// two copies, medium spacing, single width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x1003:
			// three copies, close spacing, single width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x1004:
			// two copies, far spacing, single width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x1005:
			// one copy, close spacing, single width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j * 2 + 6] = 0x20;
//						TIA_P0_Table[i][j * 2 + 7] = 0x20;
//						TIA_P1_Table[i][j * 2 + 6] = 0x10;
//						TIA_P1_Table[i][j * 2 + 7] = 0x10;
					}
				}
			break;
			case 0x1006:
			// three copies, medium spacing, double width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
//						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x1007:
			// one copy, close spacing, quadrouple width,
			// reflected, don't show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
//						TIA_P0_Table[i][j * 4 + 6] = 0x20;
//						TIA_P0_Table[i][j * 4 + 7] = 0x20;
//						TIA_P0_Table[i][j * 4 + 8] = 0x20;
//						TIA_P0_Table[i][j * 4 + 9] = 0x20;
//						TIA_P1_Table[i][j * 4 + 6] = 0x10;
//						TIA_P1_Table[i][j * 4 + 7] = 0x10;
//						TIA_P1_Table[i][j * 4 + 8] = 0x10;
//						TIA_P1_Table[i][j * 4 + 9] = 0x10;
					}
				}
			break;
			case 0x1008:
			// one copy, close spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
					}
				}
			break;
			case 0x1009:
			// two copies, close spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
					}
				}
			break;
			case 0x100a:
			// two copies, medium spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x100b:
			// three copies, close spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 16] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 16] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
					}
				}
			break;
			case 0x100c:
			// two copies, far spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x100d:
			// one copy, close spacing, single width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j * 2 + 6] = 0x20;
						TIA_P0_Table[i][j * 2 + 7] = 0x20;
						TIA_P1_Table[i][j * 2 + 6] = 0x10;
						TIA_P1_Table[i][j * 2 + 7] = 0x10;
					}
				}
			break;
			case 0x100e:
			// three copies, medium spacing, double width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j + 5] = 0x20;
						TIA_P0_Table[i][j + 5 + 32] = 0x20;
						TIA_P0_Table[i][j + 5 + 64] = 0x20;
						TIA_P1_Table[i][j + 5] = 0x10;
						TIA_P1_Table[i][j + 5 + 32] = 0x10;
						TIA_P1_Table[i][j + 5 + 64] = 0x10;
					}
				}
			break;
			case 0x100f:
			// one copy, close spacing, quadrouple width,
			// reflected, show first copy 
				for(j = 0; j < 8; j++){
					if(i & (0x0010 << j)){
						TIA_P0_Table[i][j * 4 + 6] = 0x20;
						TIA_P0_Table[i][j * 4 + 7] = 0x20;
						TIA_P0_Table[i][j * 4 + 8] = 0x20;
						TIA_P0_Table[i][j * 4 + 9] = 0x20;
						TIA_P1_Table[i][j * 4 + 6] = 0x10;
						TIA_P1_Table[i][j * 4 + 7] = 0x10;
						TIA_P1_Table[i][j * 4 + 8] = 0x10;
						TIA_P1_Table[i][j * 4 + 9] = 0x10;
					}
				}
			break;
		}
	}

	for(i = 0; i < 0x81; i++){
		TIA_M0_Table[i] = malloc(160);
		if(TIA_M0_Table[i] == NULL){
			sprintf(msg, "Not enough memory available to run z26.\n");
			srv_print(msg);
			exit(1);
		}
		TIA_M1_Table[i] = malloc(160);
		if(TIA_M1_Table[i] == NULL){
			sprintf(msg, "Not enough memory available to run z26.\n");
			srv_print(msg);
			exit(1);
		}
		for(j = 0; j < 160; j++){
			TIA_M0_Table[i][j] = 0;
			TIA_M1_Table[i][j] = 0;
		}
	}

	l = 1;
	for(i = 0; i < 8; i++){
		for(k = 0; k < 4; k++){
			for(n = 0; n < 4; n++){
				j = Object_Table[i][n];
				for(m = 0; m < l; m++){
					if(j){
						TIA_M0_Table[0x48 + (k << 4) + i][j + m] = 0x08;
						TIA_M1_Table[0x48 + (k << 4) + i][j + m] = 0x04;
					}
					if(j > 4){
						TIA_M0_Table[0x40 + (k << 4) + i][j + m] = 0x08;
						TIA_M1_Table[0x40 + (k << 4) + i][j + m] = 0x04;
					}
				}
			}
			l = l * 2;
		}
		l = 1;
	}
	for(i = 0; i < 9; i++){
		TIA_BL_Table[i] = malloc(160);
		if(TIA_BL_Table[i] == NULL){
			sprintf(msg, "Not enough memory available to run z26.\n");
			srv_print(msg);
			exit(1);
		}
		for(j = 0; j < 160; j++){
			TIA_BL_Table[i][j] = 0;
		}
	}

	TIA_BL_Table[4][4] = 0x02;

	TIA_BL_Table[5][4] = 0x02;
	TIA_BL_Table[5][5] = 0x02;

	TIA_BL_Table[6][4] = 0x02;
	TIA_BL_Table[6][5] = 0x02;
	TIA_BL_Table[6][6] = 0x02;
	TIA_BL_Table[6][7] = 0x02;

	TIA_BL_Table[7][4] = 0x02;
	TIA_BL_Table[7][5] = 0x02;
	TIA_BL_Table[7][6] = 0x02;
	TIA_BL_Table[7][7] = 0x02;
	TIA_BL_Table[7][8] = 0x02;
	TIA_BL_Table[7][9] = 0x02;
	TIA_BL_Table[7][10] = 0x02;
	TIA_BL_Table[7][11] = 0x02;

	Pointer_Index_P0 = 0;
	Pointer_Index_P1 = 0;
	Pointer_Index_M0 = 0;
	Pointer_Index_M1 = 0;
	Pointer_Index_BL = 0;

	TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
	TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
	TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
	TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
	TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
	
	TIA_Mask_Objects = 0x3f;	// display all objects

	for(i = 0; i < 8; i++){
		for(j = 0; j < 320; j++) ObjectStartTable[i][j] = 0;
	
		ObjectStartTable[i][159] = 2;	// 2 to identify main copy for RESMPx
		ObjectStartTable[i][160 + 159] = 2;
	}

	ObjectStartTable[1][15] = 1;
	ObjectStartTable[1][160 + 15] = 1;

	ObjectStartTable[2][31] = 1;
	ObjectStartTable[2][160 + 31] = 1;

	ObjectStartTable[3][15] = 1;
	ObjectStartTable[3][160 + 15] = 1;
	ObjectStartTable[3][31] = 1;
	ObjectStartTable[3][160 + 31] = 1;

	ObjectStartTable[4][63] = 1;
	ObjectStartTable[4][160 + 63] = 1;

	ObjectStartTable[6][31] = 1;
	ObjectStartTable[6][160 + 31] = 1;
	ObjectStartTable[6][63] = 1;
	ObjectStartTable[6][160 + 63] = 1;

/*
	TIA_Pixel_State:
		0x00 background
		0x01 playfield
		0x02 ball
		0x04 missile 1
		0x08 missile 0
		0x10 player 1
		0x20 player 0
		0x40 vblank
*/
	for(i = 0; i < 64; i++){
		// normal priority - P0/M0 - P1/M1 - PF/BL - BG
		TIA_Priority_Table[0][i] = BG_COLOUR;
		if(i & 0x03) TIA_Priority_Table[0][i] = PF_COLOUR;
		if(i & 0x14) TIA_Priority_Table[0][i] = P1_COLOUR;
		if(i & 0x28) TIA_Priority_Table[0][i] = P0_COLOUR;

		// playfield priority - PF/BL - P0/M0 - P1/M1 - BG
		TIA_Priority_Table[1][i] = BG_COLOUR;
		if(i & 0x14) TIA_Priority_Table[1][i] = P1_COLOUR;
		if(i & 0x28) TIA_Priority_Table[1][i] = P0_COLOUR;
		if(i & 0x03) TIA_Priority_Table[1][i] = PF_COLOUR;

		// score mode left half - PF/P0/M0 - P1/M1 - BL - BG
		TIA_Score_Priority_Table[0][i] = BG_COLOUR;
		if(i & 0x02) TIA_Score_Priority_Table[0][i] = PF_COLOUR;
		if(i & 0x14) TIA_Score_Priority_Table[0][i] = P1_COLOUR;
		if(i & 0x29) TIA_Score_Priority_Table[0][i] = P0_COLOUR;

		// score mode right half - P0/M0 - PF/P1/M1 - BL - BG
		TIA_Score_Priority_Table[1][i] = BG_COLOUR;
		if(i & 0x02) TIA_Score_Priority_Table[1][i] = PF_COLOUR;
		if(i & 0x15) TIA_Score_Priority_Table[1][i] = P1_COLOUR;
		if(i & 0x28) TIA_Score_Priority_Table[1][i] = P0_COLOUR;
	}
	
	for(i = 0; i < 64; i++){
		j = 0;
		if((i & 0x30) == 0x30) j |= 0x8000;	// P0 P1
		if((i & 0x0c) == 0x0c) j |= 0x4000;	// M0 M1
		if((i & 0x03) == 0x03) j |= 0x2000;	// BL PF
		if((i & 0x05) == 0x05) j |= 0x0800;	// M1 PF
		if((i & 0x06) == 0x06) j |= 0x0400;	// M1 BL
		if((i & 0x09) == 0x09) j |= 0x0200;	// M0 PF
		if((i & 0x0a) == 0x0a) j |= 0x0100;	// M0 BL
		if((i & 0x11) == 0x11) j |= 0x0080;	// P1 PF
		if((i & 0x12) == 0x12) j |= 0x0040;	// P1 BL
		if((i & 0x21) == 0x21) j |= 0x0020;	// P0 PF
		if((i & 0x22) == 0x22) j |= 0x0010;	// P0 BL
		if((i & 0x24) == 0x24) j |= 0x0008;	// M1 P0
		if((i & 0x14) == 0x14) j |= 0x0004;	// M1 P1
		if((i & 0x18) == 0x18) j |= 0x0002;	// M0 P1
		if((i & 0x28) == 0x28) j |= 0x0001;	// M0 P0
		TIA_Collision_Table[i] = j;
	}
}

void CatchUpPixels(void){

#include "c_catchuppixels.c"

}

//  	for(CountLoop = TIA_Last_Pixel; CountLoop < ((RClock * 3) + TIA_Delayed_Write); CountLoop++){
//  		LoopCount = CountLoop;
// 		if(LoopCount > 227) LoopCount -= 228;
//  
// 		if((LoopCount & 0x03) == 1){
// 			// counter at H1 = HIGH
// 			if(TIA_HMOVE_Setup == 1) TIA_HMOVE_Setup = 2;
// 			if(TIA_HMOVE_Latches){
// 				if(TIA_HMP0_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x1e;
// 				if(TIA_HMP1_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x2e;
// 				if(TIA_HMM0_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x36;
// 				if(TIA_HMM1_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x3a;
// 				if(TIA_HMBL_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x3c;
// 			}
// 		}else if((LoopCount & 0x03) == 3){
// 			// counter at H2 = HIGH
// 			TIA_HMOVE_DoMove = TIA_HMOVE_Latches;
// 			if(TIA_HMOVE_Clock < 16) TIA_HMOVE_Clock++;
// 			if(TIA_HMOVE_Setup == 2){
// 				TIA_HMOVE_Setup = 0;
// 				if(TIA_HMOVE_Clock == 16) TIA_HMOVE_Clock = 0;	// only reset if HMOVE isn't already in process
// 				TIA_HMOVE_Latches = 0x3e;	// enable movement for all 5 objects
// 			}
// 		}

// 		if(LoopCount > 75){
// 			
// 			if(LoopCount == 147){
// 				
// 				// we're at the center of the displayed line here
// 				// -> queue a sound byte
// 				// -> test REFPF bit
// 				// -> fix half a pixel of last PF pixel in score mode

// 				if((LoopCount & 0x03) == 0){
// 					if(TIA_Playfield_Pixels[(((LoopCount - 68) >> 2) + TIA_REFPF_Flag)] & TIA_Playfield_Bits)
// 						Current_PF_Pixel = 0x41;
// 					else Current_PF_Pixel = 0x40;
// 				};
// 				TIA_Pixel_State = Current_PF_Pixel
// 				                | TIA_P0_Line_Pointer[TIA_P0_counter]
// 				                | TIA_P1_Line_Pointer[TIA_P1_counter]
// 				                | TIA_M0_Line_Pointer[TIA_M0_counter]
// 				                | TIA_M1_Line_Pointer[TIA_M1_counter]
// 				                | TIA_BL_Line_Pointer[TIA_BL_counter];

// 				TIA_P0_counter++;
// 				if(TIA_P0_counter == 160){
// 					
// 					TIA_P0_counter = TIA_P0_counter_reset;
// 					/*
// 						0x2000 = handle hand rendered graphics
// 						
// 						0x1000 REFP0
// 						0x0ff0 GRP0_new or GRP0_old value
// 						0x0008 show first copy of current NUSIZ0
// 						0x0007 NUSIZ0_number
// 					*/
// 					Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
// 					if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
// 					if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
// 					else Pointer_Index_P0 |= TIA_GRP0_new;
// 					
// 					TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
// 					TIA_P0_counter_reset = 0;
// 				// TODO: handle RESPM0 here
// 				}

// 				TIA_P1_counter++;
// 				if(TIA_P1_counter == 160){
// 					
// 					TIA_P1_counter = TIA_P1_counter_reset;
// 					/*
// 						0x2000 = handle hand rendered graphics
// 						
// 						0x1000 REFP1
// 						0x0ff0 GRP1_new or GRP1_old value
// 						0x0008 show first copy of current NUSIZ1
// 						0x0007 NUSIZ1_number
// 					*/
// 					Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
// 					if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
// 					if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
// 					else Pointer_Index_P1 |= TIA_GRP1_new;
// 					
// 					TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
// 					TIA_P1_counter_reset = 0;
// 				// TODO: handle RESPM1 here
// 				}

// 				TIA_M0_counter++;
// 				if(TIA_M0_counter == 160){
// 					
// 					TIA_M0_counter = TIA_M0_counter_reset;
// 					/*
// 						0x80 = handle hand rendered graphics
// 						
// 						0x40 TIA_ENAM0
// 						0x30 NUSIZ_M0_width
// 						0x08 show first copy of current NUSIZ0
// 						0x07 NUSIZ0_number
// 					*/
// 					Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
// 					if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
// 					
// 					TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
// 					TIA_M0_counter_reset = 0;
// 				}

// 				TIA_M1_counter++;
// 				if(TIA_M1_counter == 160){
// 					
// 					TIA_M1_counter = TIA_M1_counter_reset;
// 					/*
// 						0x80 = handle hand rendered graphics
// 						
// 						0x40 TIA_ENAM1
// 						0x30 NUSIZ_M1_width
// 						0x08 show first copy of current NUSIZ1
// 						0x07 NUSIZ1_number
// 					*/
// 					Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
// 					if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
// 					
// 					TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
// 					TIA_M1_counter_reset = 0;
// 				}

// 				TIA_BL_counter++;
// 				if(TIA_BL_counter == 160){
// 					
// 					TIA_BL_counter = TIA_BL_counter_reset;
// 					/*
// 						0x08 = handle hand rendered graphics
// 						
// 						0x04 TIA_ENABL_new or TIA_ENABL_old
// 						0x03 CTRLPF_BL_width
// 					*/
// 					Pointer_Index_BL = CTRLPF_BL_width;
// 					if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
// 					else Pointer_Index_BL |= TIA_ENABL_new;
// 					
// 					TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
// 					TIA_BL_counter_reset = 0;
// 				}

// 				// TODO: add support for disabling objects and for PAL colour loss
// 				
// 				// collisions aren't possible during VBLANK
// 				if(!(TIA_Mask_Objects & 0x40)) TIACollide |= TIA_Collision_Table[(TIA_Pixel_State & 0x3f)];

// 				if(TIA_Do_Output){
// 					// disable colours if VBLANK and let user disable objects
// 					TIA_Pixel_State &= TIA_Mask_Objects;
// 					// playfield doesn't use score colouring mode when it has display priority
// 					if(CTRLPF_Score){
// 						/*
// 							Due to a race condition in the TIA colour encoder the last half
// 							pixel of the last PF quad in the left screen half will get a
// 							temperature dependant mix of the P0, P1 and PF colour in score mode.
// 							We simulate it be setting the colour of that half pixel to PF colour.
// 						*/
// 						if(TIA_Pixel_State == 0x01)	// only playfield active?
// 							*DisplayPointer =
// 								// TODO: make this endian safe
// 								(TIA_Colour_Table[P0_COLOUR] & 0x00ff) | (TIA_Colour_Table[PF_COLOUR] & 0xff00);
// 						else *DisplayPointer =
// 							TIA_Colour_Table[TIA_Score_Priority_Table[(LoopCount - 68) / 80][TIA_Pixel_State]];
// 					}else *DisplayPointer =
// 						TIA_Colour_Table[TIA_Priority_Table[CTRLPF_Priority][TIA_Pixel_State]];
// 					
// 					DisplayPointer++;
// 				}

// 				// The PF reflect bit gets only checked at center screen.
// 				if(CTRLPF_PF_Reflect) TIA_REFPF_Flag = 40;
// 				else TIA_REFPF_Flag = 0;

// 			}else{

// 				if((LoopCount & 0x03) == 0){
// 					if(TIA_Playfield_Pixels[(((LoopCount - 68) >> 2) + TIA_REFPF_Flag)] & TIA_Playfield_Bits)
// 						Current_PF_Pixel = 0x41;
// 					else Current_PF_Pixel = 0x40;
// 				};
// 				TIA_Pixel_State = Current_PF_Pixel
// 				                | TIA_P0_Line_Pointer[TIA_P0_counter]
// 				                | TIA_P1_Line_Pointer[TIA_P1_counter]
// 				                | TIA_M0_Line_Pointer[TIA_M0_counter]
// 				                | TIA_M1_Line_Pointer[TIA_M1_counter]
// 				                | TIA_BL_Line_Pointer[TIA_BL_counter];

// 				TIA_P0_counter++;
// 				if(TIA_P0_counter == 160){
// 					
// 					TIA_P0_counter = TIA_P0_counter_reset;
// 					/*
// 						0x2000 = handle hand rendered graphics
// 						
// 						0x1000 REFP0
// 						0x0ff0 GRP0_new or GRP0_old value
// 						0x0008 show first copy of current NUSIZ0
// 						0x0007 NUSIZ0_number
// 					*/
// 					Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
// 					if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
// 					if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
// 					else Pointer_Index_P0 |= TIA_GRP0_new;
// 					
// 					TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
// 					TIA_P0_counter_reset = 0;
// 				// TODO: handle RESPM0 here
// 				}

// 				TIA_P1_counter++;
// 				if(TIA_P1_counter == 160){
// 					
// 					TIA_P1_counter = TIA_P1_counter_reset;
// 					/*
// 						0x2000 = handle hand rendered graphics
// 						
// 						0x1000 REFP1
// 						0x0ff0 GRP1_new or GRP1_old value
// 						0x0008 show first copy of current NUSIZ1
// 						0x0007 NUSIZ1_number
// 					*/
// 					Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
// 					if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
// 					if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
// 					else Pointer_Index_P1 |= TIA_GRP1_new;
// 					
// 					TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
// 					TIA_P1_counter_reset = 0;
// 				// TODO: handle RESPM1 here
// 				}

// 				TIA_M0_counter++;
// 				if(TIA_M0_counter == 160){
// 					
// 					TIA_M0_counter = TIA_M0_counter_reset;
// 					/*
// 						0x80 = handle hand rendered graphics
// 						
// 						0x40 TIA_ENAM0
// 						0x30 NUSIZ_M0_width
// 						0x08 show first copy of current NUSIZ0
// 						0x07 NUSIZ0_number
// 					*/
// 					Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
// 					if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
// 					
// 					TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
// 					TIA_M0_counter_reset = 0;
// 				}

// 				TIA_M1_counter++;
// 				if(TIA_M1_counter == 160){
// 					
// 					TIA_M1_counter = TIA_M1_counter_reset;
// 					/*
// 						0x80 = handle hand rendered graphics
// 						
// 						0x40 TIA_ENAM1
// 						0x30 NUSIZ_M1_width
// 						0x08 show first copy of current NUSIZ1
// 						0x07 NUSIZ1_number
// 					*/
// 					Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
// 					if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
// 					
// 					TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
// 					TIA_M1_counter_reset = 0;
// 				}

// 				TIA_BL_counter++;
// 				if(TIA_BL_counter == 160){
// 					
// 					TIA_BL_counter = TIA_BL_counter_reset;
// 					/*
// 						0x08 = handle hand rendered graphics
// 						
// 						0x04 TIA_ENABL_new or TIA_ENABL_old
// 						0x03 CTRLPF_BL_width
// 					*/
// 					Pointer_Index_BL = CTRLPF_BL_width;
// 					if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
// 					else Pointer_Index_BL |= TIA_ENABL_new;
// 					
// 					TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
// 					TIA_BL_counter_reset = 0;
// 				}

// 				// TODO: add support for disabling objects and for PAL colour loss
// 				
// 				// collisions aren't possible during VBLANK
// 				if(!(TIA_Mask_Objects & 0x40)) TIACollide |= TIA_Collision_Table[(TIA_Pixel_State & 0x3f)];

// 				if(TIA_Do_Output){
// 					// disable colours if VBLANK and let user disable objects
// 					TIA_Pixel_State &= TIA_Mask_Objects;
// 					// playfield doesn't use score colouring mode when it has display priority
// 					if(CTRLPF_Score) *DisplayPointer =
// 						TIA_Colour_Table[TIA_Score_Priority_Table[(LoopCount - 68) / 80][TIA_Pixel_State]];
// 					else *DisplayPointer =
// 						TIA_Colour_Table[TIA_Priority_Table[CTRLPF_Priority][TIA_Pixel_State]];
// 					
// 					DisplayPointer++;
// 				}
// 			}
// 		}else if(LoopCount < 68){

// 			if((TIA_HMOVE_DoMove) && ((LoopCount & 0x03) == 1)){
// 				// counter at H1 = HIGH and more movement pulses need to be generated

// 				TIA_Pixel_State = TIA_P0_Line_Pointer[TIA_P0_counter]
// 				                | TIA_P1_Line_Pointer[TIA_P1_counter]
// 				                | TIA_M0_Line_Pointer[TIA_M0_counter]
// 				                | TIA_M1_Line_Pointer[TIA_M1_counter]
// 				                | TIA_BL_Line_Pointer[TIA_BL_counter];

// 				// collisions aren't possible during VBLANK
// 				if(!(TIA_Mask_Objects & 0x40)) TIACollide |= TIA_Collision_Table[TIA_Pixel_State];

// 				if(TIA_HMOVE_DoMove & 0x20){					
// 					TIA_P0_counter++;
// 					if(TIA_P0_counter == 160){
// 						
// 						TIA_P0_counter = TIA_P0_counter_reset;
// 						/*
// 							0x2000 = handle hand rendered graphics
// 							
// 							0x1000 REFP0
// 							0x0ff0 GRP0_new or GRP0_old value
// 							0x0008 show first copy of current NUSIZ0
// 							0x0007 NUSIZ0_number
// 						*/
// 						Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
// 						if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
// 						if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
// 						else Pointer_Index_P0 |= TIA_GRP0_new;
// 						
// 						TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
// 						TIA_P0_counter_reset = 0;
// 					// TODO: handle RESPM0 here
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x10){					
// 					TIA_P1_counter++;
// 					if(TIA_P1_counter == 160){
// 						
// 						TIA_P1_counter = TIA_P1_counter_reset;
// 						/*
// 							0x2000 = handle hand rendered graphics
// 							
// 							0x1000 REFP1
// 							0x0ff0 GRP1_new or GRP1_old value
// 							0x0008 show first copy of current NUSIZ1
// 							0x0007 NUSIZ1_number
// 						*/
// 						Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
// 						if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
// 						if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
// 						else Pointer_Index_P1 |= TIA_GRP1_new;
// 						
// 						TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
// 						TIA_P1_counter_reset = 0;
// 					// TODO: handle RESPM1 here
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x08){					
// 					TIA_M0_counter++;
// 					if(TIA_M0_counter == 160){
// 						
// 						TIA_M0_counter = TIA_M0_counter_reset;
// 						/*
// 							0x80 = handle hand rendered graphics
// 							
// 							0x40 TIA_ENAM0
// 							0x30 NUSIZ_M0_width
// 							0x08 show first copy of current NUSIZ0
// 							0x07 NUSIZ0_number
// 						*/
// 						Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
// 						if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
// 						
// 						TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
// 						TIA_M0_counter_reset = 0;
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x04){					
// 					TIA_M1_counter++;
// 					if(TIA_M1_counter == 160){
// 						
// 						TIA_M1_counter = TIA_M1_counter_reset;
// 						/*
// 							0x80 = handle hand rendered graphics
// 							
// 							0x40 TIA_ENAM1
// 							0x30 NUSIZ_M1_width
// 							0x08 show first copy of current NUSIZ1
// 							0x07 NUSIZ1_number
// 						*/
// 						Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
// 						if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
// 						
// 						TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
// 						TIA_M1_counter_reset = 0;
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x02){					
// 					TIA_BL_counter++;
// 					if(TIA_BL_counter == 160){
// 						
// 						TIA_BL_counter = TIA_BL_counter_reset;
// 						/*
// 							0x08 = handle hand rendered graphics
// 							
// 							0x04 TIA_ENABL_new or TIA_ENABL_old
// 							0x03 CTRLPF_BL_width
// 						*/
// 						Pointer_Index_BL = CTRLPF_BL_width;
// 						if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
// 						else Pointer_Index_BL |= TIA_ENABL_new;
// 						
// 						TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
// 						TIA_BL_counter_reset = 0;
// 					}
// 				}
// 			}			
// 		}else if(TIA_Display_HBlank){
// 			
// 			if(TIA_Do_Output){
// 				*DisplayPointer = 0;
// 				DisplayPointer++;
// 			}
// 			if(LoopCount == 75) TIA_Display_HBlank = 0;

// 			if((TIA_HMOVE_DoMove) && ((LoopCount & 0x03) == 1)){
// 				// counter at H1 = HIGH and more movement pulses need to be generated

// 				TIA_Pixel_State = TIA_P0_Line_Pointer[TIA_P0_counter]
// 				                | TIA_P1_Line_Pointer[TIA_P1_counter]
// 				                | TIA_M0_Line_Pointer[TIA_M0_counter]
// 				                | TIA_M1_Line_Pointer[TIA_M1_counter]
// 				                | TIA_BL_Line_Pointer[TIA_BL_counter];

// 				// collisions aren't possible during VBLANK
// 				if(!(TIA_Mask_Objects & 0x40)) TIACollide |= TIA_Collision_Table[TIA_Pixel_State];

// 				if(TIA_HMOVE_DoMove & 0x20){					
// 					TIA_P0_counter++;
// 					if(TIA_P0_counter == 160){
// 						
// 						TIA_P0_counter = TIA_P0_counter_reset;
// 						/*
// 							0x2000 = handle hand rendered graphics
// 							
// 							0x1000 REFP0
// 							0x0ff0 GRP0_new or GRP0_old value
// 							0x0008 show first copy of current NUSIZ0
// 							0x0007 NUSIZ0_number
// 						*/
// 						Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
// 						if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
// 						if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
// 						else Pointer_Index_P0 |= TIA_GRP0_new;
// 						
// 						TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
// 						TIA_P0_counter_reset = 0;
// 					// TODO: handle RESPM0 here
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x10){					
// 					TIA_P1_counter++;
// 					if(TIA_P1_counter == 160){
// 						
// 						TIA_P1_counter = TIA_P1_counter_reset;
// 						/*
// 							0x2000 = handle hand rendered graphics
// 							
// 							0x1000 REFP1
// 							0x0ff0 GRP1_new or GRP1_old value
// 							0x0008 show first copy of current NUSIZ1
// 							0x0007 NUSIZ1_number
// 						*/
// 						Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
// 						if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
// 						if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
// 						else Pointer_Index_P1 |= TIA_GRP1_new;
// 						
// 						TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
// 						TIA_P1_counter_reset = 0;
// 					// TODO: handle RESPM1 here
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x08){					
// 					TIA_M0_counter++;
// 					if(TIA_M0_counter == 160){
// 						
// 						TIA_M0_counter = TIA_M0_counter_reset;
// 						/*
// 							0x80 = handle hand rendered graphics
// 							
// 							0x40 TIA_ENAM0
// 							0x30 NUSIZ_M0_width
// 							0x08 show first copy of current NUSIZ0
// 							0x07 NUSIZ0_number
// 						*/
// 						Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
// 						if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
// 						
// 						TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
// 						TIA_M0_counter_reset = 0;
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x04){					
// 					TIA_M1_counter++;
// 					if(TIA_M1_counter == 160){
// 						
// 						TIA_M1_counter = TIA_M1_counter_reset;
// 						/*
// 							0x80 = handle hand rendered graphics
// 							
// 							0x40 TIA_ENAM1
// 							0x30 NUSIZ_M1_width
// 							0x08 show first copy of current NUSIZ1
// 							0x07 NUSIZ1_number
// 						*/
// 						Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
// 						if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
// 						
// 						TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
// 						TIA_M1_counter_reset = 0;
// 					}
// 				}

// 				if(TIA_HMOVE_DoMove & 0x02){					
// 					TIA_BL_counter++;
// 					if(TIA_BL_counter == 160){
// 						
// 						TIA_BL_counter = TIA_BL_counter_reset;
// 						/*
// 							0x08 = handle hand rendered graphics
// 							
// 							0x04 TIA_ENABL_new or TIA_ENABL_old
// 							0x03 CTRLPF_BL_width
// 						*/
// 						Pointer_Index_BL = CTRLPF_BL_width;
// 						if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
// 						else Pointer_Index_BL |= TIA_ENABL_new;
// 						
// 						TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
// 						TIA_BL_counter_reset = 0;
// 					}
// 				}
// 			}			
// 		}else{

// 			if((LoopCount & 0x03) == 0){
// 				if(TIA_Playfield_Pixels[(((LoopCount - 68) >> 2) + TIA_REFPF_Flag)] & TIA_Playfield_Bits)
// 					Current_PF_Pixel = 0x41;
// 				else Current_PF_Pixel = 0x40;
// 			};
// 			TIA_Pixel_State = Current_PF_Pixel
// 			                | TIA_P0_Line_Pointer[TIA_P0_counter]
// 			                | TIA_P1_Line_Pointer[TIA_P1_counter]
// 			                | TIA_M0_Line_Pointer[TIA_M0_counter]
// 			                | TIA_M1_Line_Pointer[TIA_M1_counter]
// 			                | TIA_BL_Line_Pointer[TIA_BL_counter];

// 			TIA_P0_counter++;
// 			if(TIA_P0_counter == 160){
// 				
// 				TIA_P0_counter = TIA_P0_counter_reset;
// 				/*
// 					0x2000 = handle hand rendered graphics
// 					
// 					0x1000 REFP0
// 					0x0ff0 GRP0_new or GRP0_old value
// 					0x0008 show first copy of current NUSIZ0
// 					0x0007 NUSIZ0_number
// 				*/
// 				Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
// 				if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
// 				if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
// 				else Pointer_Index_P0 |= TIA_GRP0_new;
// 				
// 				TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
// 				TIA_P0_counter_reset = 0;
// 			// TODO: handle RESPM0 here
// 			}

// 			TIA_P1_counter++;
// 			if(TIA_P1_counter == 160){
// 				
// 				TIA_P1_counter = TIA_P1_counter_reset;
// 				/*
// 					0x2000 = handle hand rendered graphics
// 					
// 					0x1000 REFP1
// 					0x0ff0 GRP1_new or GRP1_old value
// 					0x0008 show first copy of current NUSIZ1
// 					0x0007 NUSIZ1_number
// 				*/
// 				Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
// 				if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
// 				if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
// 				else Pointer_Index_P1 |= TIA_GRP1_new;
// 				
// 				TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
// 				TIA_P1_counter_reset = 0;
// 			// TODO: handle RESPM1 here
// 			}

// 			TIA_M0_counter++;
// 			if(TIA_M0_counter == 160){
// 				
// 				TIA_M0_counter = TIA_M0_counter_reset;
// 				/*
// 					0x80 = handle hand rendered graphics
// 					
// 					0x40 TIA_ENAM0
// 					0x30 NUSIZ_M0_width
// 					0x08 show first copy of current NUSIZ0
// 					0x07 NUSIZ0_number
// 				*/
// 				Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
// 				if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
// 				
// 				TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
// 				TIA_M0_counter_reset = 0;
// 			}

// 			TIA_M1_counter++;
// 			if(TIA_M1_counter == 160){
// 				
// 				TIA_M1_counter = TIA_M1_counter_reset;
// 				/*
// 					0x80 = handle hand rendered graphics
// 					
// 					0x40 TIA_ENAM1
// 					0x30 NUSIZ_M1_width
// 					0x08 show first copy of current NUSIZ1
// 					0x07 NUSIZ1_number
// 				*/
// 				Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
// 				if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
// 				
// 				TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
// 				TIA_M1_counter_reset = 0;
// 			}

// 			TIA_BL_counter++;
// 			if(TIA_BL_counter == 160){
// 				
// 				TIA_BL_counter = TIA_BL_counter_reset;
// 				/*
// 					0x08 = handle hand rendered graphics
// 					
// 					0x04 TIA_ENABL_new or TIA_ENABL_old
// 					0x03 CTRLPF_BL_width
// 				*/
// 				Pointer_Index_BL = CTRLPF_BL_width;
// 				if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
// 				else Pointer_Index_BL |= TIA_ENABL_new;
// 				
// 				TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
// 				TIA_BL_counter_reset = 0;
// 			}

// 			// TODO: add support for disabling objects and for PAL colour loss
// 			
// 			// collisions aren't possible during VBLANK
// 			if(!(TIA_Mask_Objects & 0x40)) TIACollide |= TIA_Collision_Table[(TIA_Pixel_State & 0x3f)];

// 			if(TIA_Do_Output){
// 				// disable colours if VBLANK and let user disable objects
// 				TIA_Pixel_State &= TIA_Mask_Objects;
// 				// playfield doesn't use score colouring mode when it has display priority
// 				if(CTRLPF_Score) *DisplayPointer =
// 					TIA_Colour_Table[TIA_Score_Priority_Table[(LoopCount - 68) / 80][TIA_Pixel_State]];
// 				else *DisplayPointer =
// 					TIA_Colour_Table[TIA_Priority_Table[CTRLPF_Priority][TIA_Pixel_State]];
// 				
// 				DisplayPointer++;
// 			}
// 		}
// 	}
// 	TIA_Last_Pixel = (RClock * 3) + TIA_Delayed_Write;
//	}


db TIA_Priority[0x10000];
db TIA_Pixels[160];
dd TIA_Colours[16] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
dw *TIA_Pixel_PTR;
dd *MyDisplayPointer;

void nTIALineTo(void){

	QueueSoundBytes();	//  put another 2 bytes in the sound queue

	// CFirst and MaxLines ? These need to be set in position.c from tiawrite.c
	if((ScanLine < TopLine) || (ScanLine >= BottomLine)) TIA_Do_Output = 0;
	else TIA_Do_Output = 1;
	do_Instruction_line();
	
//	if((RClock < (2 * CYCLESPERSCANLINE)) && (TIA_Last_Pixel < (RClock * 3))) CatchUpPixels();
	if(TIA_Last_Pixel < (RClock * 3)) CatchUpPixels();
	TIA_Last_Pixel = TIA_Last_Pixel % (CYCLESPERSCANLINE * 3);

/*
	int i;
	if(TIA_Do_Output){
		MyDisplayPointer = (dd*) DisplayPointer;
		TIA_Pixel_PTR = (dw*) &TIA_Pixels[0];
		for(i = 0; i < 80; i++){
			*MyDisplayPointer = TIA_Colours[TIA_Priority[*TIA_Pixel_PTR]];
			*TIA_Pixel_PTR &= 0x4040;
			TIA_Pixel_PTR++;
			MyDisplayPointer++;
		}
		DisplayPointer = (dw*) MyDisplayPointer;
	}
*/
}
