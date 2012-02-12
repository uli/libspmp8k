/*

	cpu memory and register handlers -- used by the CPU emulator


 z26 is Copyright 1997-2011 by John Saeger and contributors.  
 z26 is released subject to the terms and conditions of the 
 GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
 Please see COPYING.TXT for details.
*/

/*
 Hardware I/O address bits

 Dan Boris' 2600 schematics show the TIA and RIOT chips hooked up to the
 CPU with the following address lines hooked up.

   12 | 11  10  09  08 | 07  06  05  04 | 03	02  01	00 
 
    X                     X                                  TIA
    X            X        X                                  RIOT

 If the 1000h bit (bit 12) is set, it's a ROM access.  This is handled
 in banks.c and we'll never come here.

 Otherwise it's a hardware access.

 If the 200h bit is	set and the 80h bit is set then it's a RIOT access.
 If the 200h bit is not set and the 80h bit is set then it's a RAM access.
 If the 200h bit is not set and the 80h bit is not set then it's a TIA access.
*/

extern void C_NewTIA(void);

void Init_CPUhand() {
//	InputLatch[0] = InputLatch[1] = 0x80;
}

/*
	Memory Mapping - Read
*/

void ReadHardware(void){

	if((AddressBus & 0x280) == 0x280) { ReadRIOT(); }
	else if(AddressBus & 0x80) { DataBus = RiotRam[AddressBus & 0x7f]; }
	else {
		DataBus &= 0x3f;	/* TIA only sets the two topmost bits */
		if((AddressBus & 0xf) < 0x8) {
			CatchUpPixels();
//			DataBus |= ((TIACollide >> ((AddressBus & 0x7) << 1)) & 0x3) << 6;
		}else if (((AddressBus & 0xf) < 0xe) && ((AddressBus & 0xf) > 0x7)) {
			if((AddressBus & 0xf) < 0xc) {
				if(ChargeTrigger0[AddressBus & 0x3] <= ChargeCounter)
					{ DataBus |= 0x80; }
			}else{
				TestLightgunHit(RClock, ScanLine);	/* JS */
				DataBus |= InputLatch[AddressBus & 0x1];
			}
		}
/*		DataBus = DataBus | 0x3f;	 Reindeer Rescue test -- remove me */
	}
}


/*
	Memory mapping - Write. 
*/

void WriteHardware(void){

	if((AddressBus & 0x280) == 0x280) { WriteRIOT(); }
	else if(AddressBus & 0x80) { RiotRam[AddressBus & 0x7f] = DataBus; }
	else { C_NewTIA(); }
}
