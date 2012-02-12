changecom(`/*',`*/')dnl
/*

	TIA pixel rendering loop for z26


 This C file gets generated from catchuppixels.m4, so please edit it there.

 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/

define(handleP0,`
					TIA_P0_counter++;
					if(TIA_P0_counter == 160){
						
						TIA_P0_counter = TIA_P0_counter_reset;
						/*
							0x2000 = handle hand rendered graphics
							
							0x1000 REFP0
							0x0ff0 GRP0_new or GRP0_old value
							0x0008 show first copy of current NUSIZ0
							0x0007 NUSIZ0_number
						*/
						Pointer_Index_P0 = NUSIZ0_number | TIA_REFP0;
						if(TIA_P0_counter_reset == 0) Pointer_Index_P0 |= 0x0008;
						if(TIA_VDELP0) Pointer_Index_P0 |= TIA_GRP0_old;
						else Pointer_Index_P0 |= TIA_GRP0_new;
						
						TIA_P0_Line_Pointer = TIA_P0_Table[Pointer_Index_P0];
						TIA_P0_counter_reset = 0;
					/* TODO: handle RESPM0 here */
					}')dnl
define(handleP1,`
					TIA_P1_counter++;
					if(TIA_P1_counter == 160){
						
						TIA_P1_counter = TIA_P1_counter_reset;
						/*
							0x2000 = handle hand rendered graphics
							
							0x1000 REFP1
							0x0ff0 GRP1_new or GRP1_old value
							0x0008 show first copy of current NUSIZ1
							0x0007 NUSIZ1_number
						*/
						Pointer_Index_P1 = NUSIZ1_number | TIA_REFP1;
						if(TIA_P1_counter_reset == 0) Pointer_Index_P1 |= 0x0008;
						if(TIA_VDELP1) Pointer_Index_P1 |= TIA_GRP1_old;
						else Pointer_Index_P1 |= TIA_GRP1_new;
						
						TIA_P1_Line_Pointer = TIA_P1_Table[Pointer_Index_P1];
						TIA_P1_counter_reset = 0;
					/* TODO: handle RESPM1 here */
					}')dnl
define(handleM0,`
					TIA_M0_counter++;
					if(TIA_M0_counter == 160){
						
						TIA_M0_counter = TIA_M0_counter_reset;
						/*
							0x80 = handle hand rendered graphics
							
							0x40 TIA_ENAM0
							0x30 NUSIZ_M0_width
							0x08 show first copy of current NUSIZ0
							0x07 NUSIZ0_number
						*/
						Pointer_Index_M0 = NUSIZ0_number | NUSIZ_M0_width | TIA_ENAM0;
						if(TIA_M0_counter_reset == 0) Pointer_Index_M0 |= 0x08;
						
						TIA_M0_Line_Pointer = TIA_M0_Table[Pointer_Index_M0];
						TIA_M0_counter_reset = 0;
					}')dnl
define(handleM1,`
					TIA_M1_counter++;
					if(TIA_M1_counter == 160){
						
						TIA_M1_counter = TIA_M1_counter_reset;
						/*
							0x80 = handle hand rendered graphics
							
							0x40 TIA_ENAM1
							0x30 NUSIZ_M1_width
							0x08 show first copy of current NUSIZ1
							0x07 NUSIZ1_number
						*/
						Pointer_Index_M1 = NUSIZ1_number | NUSIZ_M1_width | TIA_ENAM1;
						if(TIA_M1_counter_reset == 0) Pointer_Index_M1 |= 0x08;
						
						TIA_M1_Line_Pointer = TIA_M1_Table[Pointer_Index_M1];
						TIA_M1_counter_reset = 0;
					}')dnl
define(handleBL,`
					TIA_BL_counter++;
					if(TIA_BL_counter == 160){
						
						TIA_BL_counter = TIA_BL_counter_reset;
						/*
							0x08 = handle hand rendered graphics
							
							0x04 TIA_ENABL_new or TIA_ENABL_old
							0x03 CTRLPF_BL_width
						*/
						Pointer_Index_BL = CTRLPF_BL_width;
						if(TIA_VDELBL) Pointer_Index_BL |= TIA_ENABL_old;
						else Pointer_Index_BL |= TIA_ENABL_new;
						
						TIA_BL_Line_Pointer = TIA_BL_Table[Pointer_Index_BL];
						TIA_BL_counter_reset = 0;
					}')dnl
define(handleH1,`
				if((LoopCount & 0x03) == 1){
					/* counter at H1 = HIGH */
					if(TIA_HMOVE_Setup == 1) TIA_HMOVE_Setup = 2;
					if(TIA_HMOVE_Latches){
						if(TIA_HMP0_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x1e;
						if(TIA_HMP1_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x2e;
						if(TIA_HMM0_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x36;
						if(TIA_HMM1_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x3a;
						if(TIA_HMBL_Value == (TIA_HMOVE_Clock & 0x0f)) TIA_HMOVE_Latches &= 0x3c;
					}
				}')dnl
define(handleH2,
				if((LoopCount & 0x03) == 3){
					/* counter at H2 = HIGH */
					TIA_HMOVE_DoMove = TIA_HMOVE_Latches;
					if(TIA_HMOVE_Clock < 16) TIA_HMOVE_Clock++;
					if(TIA_HMOVE_Setup == 2){
						TIA_HMOVE_Setup = 0;
						if(TIA_HMOVE_Clock == 16) TIA_HMOVE_Clock = 0;
							/* only reset if HMOVE isn't already in process */
						TIA_HMOVE_Latches = 0x3e;
							/* enable movement for all 5 objects */
					}
				})dnl
define(handlePF,`
					if((LoopCount & 0x03) == 0){
						if(TIA_Playfield_Pixels[(((LoopCount - 68) >> 2) + TIA_REFPF_Flag)] & TIA_Playfield_Bits)
							Current_PF_Pixel = 0x01;
						else Current_PF_Pixel = 0x00;
					};')dnl
define(combineObjectsOnly,`
					TIA_Pixel_State = TIA_P0_Line_Pointer[TIA_P0_counter]
					                | TIA_P1_Line_Pointer[TIA_P1_counter]
					                | TIA_M0_Line_Pointer[TIA_M0_counter]
					                | TIA_M1_Line_Pointer[TIA_M1_counter]
					                | TIA_BL_Line_Pointer[TIA_BL_counter];')dnl
define(combineObjectsPF,`
					TIA_Pixel_State = Current_PF_Pixel
					                | TIA_P0_Line_Pointer[TIA_P0_counter]
					                | TIA_P1_Line_Pointer[TIA_P1_counter]
					                | TIA_M0_Line_Pointer[TIA_M0_counter]
					                | TIA_M1_Line_Pointer[TIA_M1_counter]
					                | TIA_BL_Line_Pointer[TIA_BL_counter];')dnl
define(handleCollisions,`
					/* TODO: add support for for PAL colour loss */
					
					TIACollide |= TIA_Collision_Table[TIA_Pixel_State];')dnl
define(handlePriorityLine,
					/* let user disable objects */
					TIA_Pixel_State &= TIA_Mask_Objects;
					/* playfield doesn't use score colouring mode when it has display priority */
					if(CTRLPF_Score) *DisplayPointer =
						TIA_Colour_Table[TIA_Score_Priority_Table[(LoopCount - 68) / 80][TIA_Pixel_State]];
					else *DisplayPointer =
						TIA_Colour_Table[TIA_Priority_Table[CTRLPF_Priority][TIA_Pixel_State]];
					
					DisplayPointer++;)dnl
define(handlePriorityMidline,				
					/* let user disable objects */
					TIA_Pixel_State &= TIA_Mask_Objects;
					/* playfield doesn't use score colouring mode when it has display priority */
					if(CTRLPF_Score){
						/*
							Due to a race condition in the TIA colour encoder the last half
							pixel of the last PF quad in the left screen half will get a
							temperature dependant mix of the P0, P1 and PF colour in score mode.
							We simulate it be setting the colour of that half pixel to PF colour.
						*/
						if(TIA_Pixel_State == 0x01)	/* only playfield active? */
							*DisplayPointer =
								/* TODO: make this endian safe */
								(TIA_Colour_Table[P0_COLOUR] & 0x00ff) | (TIA_Colour_Table[PF_COLOUR] & 0xff00);
						else *DisplayPointer =
							TIA_Colour_Table[TIA_Score_Priority_Table[(LoopCount - 68) / 80][TIA_Pixel_State]];
					}else *DisplayPointer =
						TIA_Colour_Table[TIA_Priority_Table[CTRLPF_Priority][TIA_Pixel_State]];
					
					DisplayPointer++;)dnl
define(handleVisibleDoOutput,`
				handlePF
				combineObjectsPF
				handleP0
				handleP1
				handleM0
				handleM1
				handleBL
				handleCollisions')dnl
define(handleVisibleVBlank,`
				handlePF
				handleP0
				handleP1
				handleM0
				handleM1
				handleBL
				*DisplayPointer = 0;
				DisplayPointer++;')dnl
define(handleVisibleVBlankNoOutput,`
				handlePF
				handleP0
				handleP1
				handleM0
				handleM1
				handleBL')dnl
define(handleVisibleNoOutput,`
				handlePF
				combineObjectsPF
				handleP0
				handleP1
				handleM0
				handleM1
				handleBL
				handleCollisions')dnl
define(handleInvisibleCollision,`
					if(TIA_HMOVE_DoMove){
						combineObjectsOnly
						handleCollisions
						if(TIA_HMOVE_DoMove & 0x20){					
							handleP0
						}
						if(TIA_HMOVE_DoMove & 0x10){					
							handleP1
						}
						if(TIA_HMOVE_DoMove & 0x08){					
							handleM0
						}
						if(TIA_HMOVE_DoMove & 0x04){					
							handleM1
						}
						if(TIA_HMOVE_DoMove & 0x02){					
							handleBL
						}
						TIA_HMOVE_DoMove = 0;
					}	')dnl
define(handleInvisibleVBlank,`
					if(TIA_HMOVE_DoMove){
						if(TIA_HMOVE_DoMove & 0x20){					
							handleP0
						}
						if(TIA_HMOVE_DoMove & 0x10){					
							handleP1
						}
						if(TIA_HMOVE_DoMove & 0x08){					
							handleM0
						}
						if(TIA_HMOVE_DoMove & 0x04){					
							handleM1
						}
						if(TIA_HMOVE_DoMove & 0x02){					
							handleBL
						}
						TIA_HMOVE_DoMove = 0;
					}	')dnl
define(renderBlank,`
			*DisplayPointer = 0;
			DisplayPointer++;')dnl

define(renderLoop,

		 	for(CountLoop = TIA_Last_Pixel; CountLoop < ((RClock * 3) + TIA_Delayed_Write); CountLoop++){
		 		LoopCount = CountLoop;
				if(LoopCount > 227) LoopCount -= 228;
		
				handleH1
				handleH2

				if(LoopCount > 75){
					
					if(LoopCount == 147){
						
					/*
						we're at the center of the displayed line here
						-> queue a sound byte
						-> test REFPF bit
						-> fix half a pixel of last PF pixel in score mode
					*/
			
						$1
						$4 
		
						/* The PF reflect bit gets only checked at center screen. */
						if(CTRLPF_PF_Reflect) TIA_REFPF_Flag = 40;
						else TIA_REFPF_Flag = 0;
		
					}else{
		
						$1
						$5
					}	
				}else if(LoopCount < 68){
		
					$2
						
				}else if(TIA_Display_HBlank){
		
					$3
					if(LoopCount == 75) TIA_Display_HBlank = 0;
		
					$2
		
				}else{
		
					$1
					$5
				}	
			}
			TIA_Last_Pixel = (RClock * 3) + TIA_Delayed_Write;)dnl

	if(TIA_Do_Output){
		if(TIA_VBLANK){
			renderLoop(handleVisibleVBlank,handleInvisibleVBlank,renderBlank)
		}else{
			renderLoop(handleVisibleDoOutput,handleInvisibleCollision,renderBlank,handlePriorityMidline,handlePriorityLine)
		}
	}else{
		if(TIA_VBLANK){
			renderLoop(handleVisibleVBlankNoOutput,handleInvisibleVBlank)
		}else{
			renderLoop(handleVisibleNoOutput,handleInvisibleCollision)
		}
	}
