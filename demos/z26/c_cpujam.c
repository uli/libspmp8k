/*
	jam handler 
*/

void jam(void){
	AddressBus = reg_pc - 1;
//	(*ReadROMAccess)();			// <--- *** poison *** ---
	if(Starpath && (DataBus == 0x52)) StarpathJAM();
	else set_status("Jammed");
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).  
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
