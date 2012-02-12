/*
** banks.c -- z26 bank switch stuff
*/

db Starpath;
db Pitfall2;

dd RomBank = 0; /* Rom bank pointer for F8 & F16 */
/* take lower 4 bit of address and subtract this to get bank number */
dd HotspotAdjust = 0;

/* Parker Brother's ROM Slices */

dd PBSlice0 = 0;
dd PBSlice1 = 0x400;
dd PBSlice2 = 0x800;

/* Tigervision 3E, 3F and 3F+ ROM Slices */

dd TVSlice0 = 0;
dd TVSlice1 = 3*0x800;
dd ROMorRAM3E = 0;

/* M-Network RAM Slices */

dd MNRamSlice = 0; /* which 256 byte ram slice */

/* CompuMate RAM state */

dd CMRamState = 0x10; /*  RAM enabled - read/write state */

/* John Payson's (supercat) 4A50 ROM slices */

dd Bank4A50Low = 0;	/* pointer for $1000-$17ff slice */
dd ROMorRAMLow = 0;	/* 0 = ROM -- 1 = RAM at $1000-$17ff */
dd Bank4A50Middle = 0;	/* pointer for $1800-$1dff slice */
dd ROMorRAMMiddle = 0;	/* 0 = ROM -- 1 = RAM at $1800-$1dff */
dd Bank4A50High = 0;	/* pointer for $1e00-$1eff slice */
dd ROMorRAMHigh = 0;	/* 0 = ROM -- 1 = RAM at $1e00-$1eFF */
db LastDataBus4A50 = 0xff;	/* state of DataBus on last cycle */
dd LastAddressBus4A50 = 0xffff;	/* state of AddressBus on last cycle */

/*

	The read and write handlers for each bankswitching scheme

	They will be called indirectly though ReadROMAccess and WriteROMAccess
	
*/

void Copy64K(void){
	int i;
	
	for(i = 0; i < 0x2000; i++){
		ReadAccess[i + 0x2000] = ReadAccess[i];
		WriteAccess[i + 0x2000] = WriteAccess[i];
		ReadAccess[i + 0x4000] = ReadAccess[i];
		WriteAccess[i + 0x4000] = WriteAccess[i];
		ReadAccess[i + 0x6000] = ReadAccess[i];
		WriteAccess[i + 0x6000] = WriteAccess[i];
		ReadAccess[i + 0x8000] = ReadAccess[i];
		WriteAccess[i + 0x8000] = WriteAccess[i];
		ReadAccess[i + 0xa000] = ReadAccess[i];
		WriteAccess[i + 0xa000] = WriteAccess[i];
		ReadAccess[i + 0xc000] = ReadAccess[i];
		WriteAccess[i + 0xc000] = WriteAccess[i];
		ReadAccess[i + 0xe000] = ReadAccess[i];
		WriteAccess[i + 0xe000] = WriteAccess[i];
	}
}


void ReadROM2K(void){
	
	DataBus = CartRom[AddressBus & 0x7ff];
}

void ReadROM4K(void){
	
	DataBus = CartRom[AddressBus & 0xfff];
}

void WriteROM4K(void){
}

void ReadBSFE(void){

	DataBus = CartRom[(AddressBus & 0xfff) + 0x1000];
}

void ReadBS4K(void){

	DataBus = CartRom[(AddressBus & 0xfff) + RomBank];
}

void ReadHotspotBS4K(void){

	DataBus = CartRom[(AddressBus & 0xfff) + RomBank];
	RomBank = ((AddressBus & 0x0f) - HotspotAdjust) << 12;
}

void WriteHotspotBS4K(void){

	RomBank = ((AddressBus & 0x0f) - HotspotAdjust) << 12;
}

void ReadHotspotUA(void){

	RomBank = (AddressBus & 0x40) << 6;
	(* TIARIOTReadAccess[AddressBus & 0xfff])();
}

void WriteHotspotUA(void){

	RomBank = (AddressBus & 0x40) << 6;
	(* TIARIOTWriteAccess[AddressBus & 0xfff])();
}

void ReadHotspotMB(void){

	DataBus = CartRom[(AddressBus & 0xfff) + RomBank];
	RomBank = (RomBank + 0x1000) & 0xf000;
}

void WriteHotspotMB(void){

	RomBank = (RomBank + 0x1000) & 0xf000;
}

void ReadRAM128(void){

	DataBus = Ram[AddressBus & 0x7f];
}

void WriteRAM128(void){

	Ram[AddressBus & 0x7f] = DataBus;
}

void ReadRAM256(void){

	DataBus = Ram[AddressBus & 0xff];
}

void WriteRAM256(void){

	Ram[AddressBus & 0xff] = DataBus;
}

void ReadRAM1K(void){

	DataBus = Ram[AddressBus & 0x3ff];
}

void WriteRAM1K(void){

	Ram[AddressBus & 0x3ff] = DataBus;
}

void ReadCMhigh(void){
	if(CMRamState & 0x10)
		DataBus = CartRom[(AddressBus & 0xfff) + RomBank];
	else DataBus = Ram[AddressBus & 0x7ff];
}

void WriteHotspotCM(void){
	CMRamState = DataBus;
	RomBank = (CMRamState & 0x3) << 12;
	if(DataBus & 0x20) CM_Collumn = 0;
	if(DataBus & 0x40) CM_Collumn = (CM_Collumn + 1) % 10;
	(* TIARIOTWriteAccess[0x280])();
}

void WriteCMhigh(void){
	if ((CMRamState & 0x30) == 0x20)
		Ram[AddressBus & 0x7ff] = DataBus;
}


/*
	0 -- 2K+4K Atari [NoBS]
	
	4K of ROM fixed at $1000 - $1FFF
	2K ROMs are doubled to appear at $1000 - $17FF and at $1800 - $1FFF
*/
void Init4K(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadROM4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	Copy64K();
}


/*
	1 -- CommaVid [CV]
	
	2K of ROM fixed at $1800 - $1FFF
	1K of RAM fixed at $1000 - $17FF
	read from RAM at $1000 - $13FF
	write to  RAM at $1400 - $17FF
*/
void InitCV(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
	}
	for(i = 0; i < 0x800; i++){
		ReadAccess[0x1800 + i] = &ReadROM2K;
		WriteAccess[0x1800 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x400; i++){
		ReadAccess[0x1000 + i] = &ReadRAM1K;
		WriteAccess[0x1000 + i] = &ReadRAM1K;
		WriteAccess[0x1400 + i] = &WriteRAM1K;
		ReadAccess[0x1400 + i] = &WriteRAM1K;
	}
	Copy64K();
}


/*
	2 -- 8K Superchip [F8SC]
	
	2 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF8
	select bank 1 by accessing $1FF9
	128 bytes of RAM at $1000 - $10FF
	read from RAM at $1080 - $10FF
	write to  RAM at $1000 - $107F
*/
void InitF8SC(void){
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

	for(i = 0; i < 0x80; i++){
		ReadAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1000 + i] = &WriteRAM128;
		ReadAccess[0x1000 + i] = &WriteRAM128;
	}
	Copy64K();
}


/*
	3 -- 8K Parker Bros. [E0]
	
	1K of ROM fixed at $1C00 - $1FFF
	this is the last 1K (bank 7) in the ROM image
	8 1K banks of ROM switchable at $1000 - $13FF
	select bank 0 by accessing $1FE0
	select bank 1 by accessing $1FE1
	select bank 2 by accessing $1FE2
	select bank 3 by accessing $1FE3
	select bank 4 by accessing $1FE4
	select bank 5 by accessing $1FE5
	select bank 6 by accessing $1FE6
	select bank 7 by accessing $1FE7
	8 1K banks of ROM switchable at $1400 - $17FF
	select bank 0 by accessing $1FE8
	select bank 1 by accessing $1FE9
	select bank 2 by accessing $1FEA
	select bank 3 by accessing $1FEB
	select bank 4 by accessing $1FEC
	select bank 5 by accessing $1FED
	select bank 6 by accessing $1FEE
	select bank 7 by accessing $1FEF
	8 1K banks of ROM switchable at $1800 - $1BFF
	select bank 0 by accessing $1FF0
	select bank 1 by accessing $1FF1
	select bank 2 by accessing $1FF2
	select bank 3 by accessing $1FF3
	select bank 4 by accessing $1FF4
	select bank 5 by accessing $1FF5
	select bank 6 by accessing $1FF6
	select bank 7 by accessing $1FF7
	
*/
void ReadHotspotE0_B0(void){
	DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice0];
	PBSlice0 = (AddressBus & 0x7) << 10;
}
void WriteHotspotE0_B0(void){
	PBSlice0 = (AddressBus & 0x7) << 10;
}
void ReadHotspotE0_B1(void){
	DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice1];
	PBSlice1 = (AddressBus & 0x7) << 10;
}
void WriteHotspotE0_B1(void){
	PBSlice1 = (AddressBus & 0x7) << 10;
}
void ReadHotspotE0_B2(void){
	DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice2];
	PBSlice2 = (AddressBus & 0x7) << 10;
}
void WriteHotspotE0_B2(void){
	PBSlice2 = (AddressBus & 0x7) << 10;
}
void ReadBSE0_B0(void){
		DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice0];
}
void ReadBSE0_B1(void){
		DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice1];
}
void ReadBSE0_B2(void){
		DataBus = CartRom[(AddressBus & 0x3ff) + PBSlice2];
}
void ReadBSE0_B3(void){
		DataBus = CartRom[(AddressBus & 0x3ff) + 0x1c00];
}
void InitE0(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x400; i++){
		ReadAccess[0x1000 + i] = &ReadBSE0_B0;
		ReadAccess[0x1400 + i] = &ReadBSE0_B1;
		ReadAccess[0x1800 + i] = &ReadBSE0_B2;
		ReadAccess[0x1c00 + i] = &ReadBSE0_B3;
	}
	for(i = 0; i < 8; i++){
		ReadAccess[0x1fe0 + i] = &ReadHotspotE0_B0;
		WriteAccess[0x1fe0 + i] = &WriteHotspotE0_B0;
		ReadAccess[0x1fe8 + i] = &ReadHotspotE0_B1;
		WriteAccess[0x1fe8 + i] = &WriteHotspotE0_B1;
		ReadAccess[0x1ff0 + i] = &ReadHotspotE0_B2;
		WriteAccess[0x1ff0 + i] = &WriteHotspotE0_B2;
	}
	Copy64K();
}


/*
	 4 -- 8K Tigervision [3F] and
	11 -- 512K extended Tigervision [3F+]
	
	up to 256 2K banks of ROM at $1000 - $17FF
	select bank by writing bank number to any address between $0000 and $003F
	[3F]  -> 2K of ROM fixed at $1800 - $ 1FFFF (this is always the 4th 2K bank in the ROM)
	[3F+] -> 2K of ROM fixed at $1800 - $ 1FFFF (this is always the last 2K bank in the ROM)
*/
void ReadBS3Flow(void){
	DataBus = CartRom[(AddressBus & 0x7ff) + TVSlice0];
}
void ReadBS3Fhigh(void){
	DataBus = CartRom[(AddressBus & 0x7ff) + TVSlice1];
}
void WriteHotspot3F(void){
	TVSlice0 = DataBus << 11;
	(* TIARIOTWriteAccess[AddressBus & 0x3f])();
}
void Init3F(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x800; i++){
		ReadAccess[0x1000 + i] = &ReadBS3Flow;
		ReadAccess[0x1800 + i] = &ReadBS3Fhigh;
	}
	for(i = 0; i < 0x40; i++){
		WriteAccess[i] = &WriteHotspot3F;
	}
	Copy64K();
}


/*
	5 -- 8K Activision [FE] (flat)
	
	2 8K banks at $1000 - $1FFF
	banks are switched by JSR/RTS jumps to an address in the other bank.
	the real hardware waits for two consecutive accesses to the stack to do so.
	we cheat and just select the bank depending on the accessed address in ROM.
	the $F000 - $FFFF bank is in the ROM image first, the $D000 - $DFFF bank is last.
*/
void InitFE(void){
	int i, j;
	
	for(j = 0; j < 2; j++){
		for(i = 0; i < 0x1000; i++){
			ReadAccess[j * 0x2000 + i] = TIARIOTReadAccess[i];
			WriteAccess[j * 0x2000 + i] = TIARIOTWriteAccess[i];
			ReadAccess[0x3000 + i] = &ReadROM4K;
			ReadAccess[0x1000 + i] = &ReadBSFE;
			WriteAccess[j * 0x2000 + 0x1000 + i] = &WriteROM4K;
		}
	}
	for(i = 0; i < 0x4000; i++){
		ReadAccess[i + 0x4000] = ReadAccess[i];
		WriteAccess[i + 0x4000] = WriteAccess[i];
		ReadAccess[i + 0x8000] = ReadAccess[i];
		WriteAccess[i + 0x8000] = WriteAccess[i];
		ReadAccess[i + 0xc000] = ReadAccess[i];
		WriteAccess[i + 0xc000] = WriteAccess[i];
	}
}


/*
	6 -- 16K Superchip [F6SC]

	4 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF6
	select bank 1 by accessing $1FF7
	select bank 2 by accessing $1FF8
	select bank 3 by accessing $1FF9
	128 bytes of RAM at $1000 - $10FF
	read from RAM at $1080 - $10FF
	write to  RAM at $1000 - $107F
*/
void InitF6SC(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 4; i++){
		ReadAccess[0x1ff6 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff6 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 6;

	for(i = 0; i < 0x80; i++){
		ReadAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1000 + i] = &WriteRAM128;
		ReadAccess[0x1000 + i] = &WriteRAM128;
	}
	Copy64K();
}


/*
	7 -- 16K M-Network [E7]

	2K of ROM fixed at $1800 - $1FFF
	this is always the last 2K (bank 7) of the ROM image
	6 2K banks of ROM switchable at $1000 - 17FF
	select bank 0 by accessing $1FE0
	select bank 1 by accessing $1FE1
	select bank 2 by accessing $1FE2
	select bank 3 by accessing $1FE3
	select bank 4 by accessing $1FE4
	select bank 5 by accessing $1FE5
	select bank 6 by accessing $1FE6
	accessing $1FE7 selects 1K of RAM at $1000 - $17FF
	read from RAM at $1400 - $17FF
	write to  RAM at $1000 - $13FF
	there are also 4 256 bytes banks of RAM at $1800 - $19FF
	select RAM bank 0 by accessing $1FE8
	select RAM bank 1 by accessing $1FE9
	select RAM bank 2 by accessing $1FEA
	select RAM bank 3 by accessing $1FEB
	read from RAM at $1900 - $19FF
	write to  RAM at $1800 - $18FF
	
*/
void ReadBSE7(void){

	if(RomBank == (0x7 * 0x800)) DataBus = Ram[(AddressBus & 0x3ff) + 0x400];
	else DataBus = CartRom[(AddressBus & 0x7ff) + RomBank];
}

void WriteBSE7(void){

	if(RomBank == (0x7 * 0x800)) Ram[(AddressBus & 0x3ff) + 0x400] = DataBus;
}

void ReadBSE7RAM(void){

	DataBus = Ram[(AddressBus & 0xff) + MNRamSlice];
}

void WriteBSE7RAM(void){

	Ram[(AddressBus & 0xff) + MNRamSlice] = DataBus;
}

void ReadBSE7last(void){

	DataBus = CartRom[(AddressBus & 0x7ff) + 0x3800];
}

void ReadHotspotBSE7ROM(void){

	DataBus = CartRom[(AddressBus & 0x7ff) + 0x3800];
	RomBank = (AddressBus & 0x0f) << 11;
}

void WriteHotspotBSE7ROM(void){

	RomBank = (AddressBus & 0x0f) << 11;
}

void ReadHotspotBSE7RAM(void){

	DataBus = CartRom[(AddressBus & 0x7ff) + 0x3800];
	MNRamSlice = (AddressBus & 0x03) << 8;
}

void WriteHotspotBSE7RAM(void){

	MNRamSlice = (AddressBus & 0x03) << 8;
}

void InitE7(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
	}
	for(i = 0; i < 0x800; i++){
		ReadAccess[0x1000 + i] = &ReadBSE7;
		WriteAccess[0x1000 + i] = &WriteBSE7;
		ReadAccess[0x1800 + i] = &ReadBSE7last;
		WriteAccess[0x1800 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x100; i++){
		ReadAccess[0x1800 + i] = &WriteBSE7RAM;
		WriteAccess[0x1800 + i] = &WriteBSE7RAM;
		ReadAccess[0x1900 + i] = &ReadBSE7RAM;
		WriteAccess[0x1900 + i] = &ReadBSE7RAM;
	}
	for(i = 0; i < 8; i++){
		ReadAccess[0x1fe0 + i] = &ReadHotspotBSE7ROM;
		WriteAccess[0x1fe0 + i] = &WriteHotspotBSE7ROM;
	}
	for(i = 0; i < 4; i++){
		ReadAccess[0x1fe8 + i] = &ReadHotspotBSE7RAM;
		WriteAccess[0x1fe8 + i] = &WriteHotspotBSE7RAM;
	}

	Copy64K();
}


/*
	8 -- 32K Superchip [F4SC]

	8 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF4
	select bank 1 by accessing $1FF5
	select bank 2 by accessing $1FF6
	select bank 3 by accessing $1FF7
	select bank 4 by accessing $1FF8
	select bank 5 by accessing $1FF9
	select bank 6 by accessing $1FFA
	select bank 7 by accessing $1FFB
	128 bytes of RAM at $1000 - $10FF
	read from RAM at $1080 - $10FF
	write to  RAM at $1000 - $107F
*/
void InitF4SC(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 8; i++){
		ReadAccess[0x1ff4 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff4 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 4;

	for(i = 0; i < 0x80; i++){
		ReadAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1080 + i] = &ReadRAM128;
		WriteAccess[0x1000 + i] = &WriteRAM128;
		ReadAccess[0x1000 + i] = &WriteRAM128;
	}
	Copy64K();
}


/*
	9 & 20 -- 8K Atari [F8] (9 = banks swapped)

	2 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF8
	select bank 1 by accessing $1FF9
*/
void InitF8(void){
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
	Copy64K();
}


/*
	10 -- Compumate [CM]

	there are 4 4K banks selectable at $1000 - $1FFFF
	bankswitching is done though the controller ports
	INPT0: D7 = CTRL key input (0 on startup / 1 = key pressed)
	INPT1: D7 = always HIGH input (tested at startup)
	INPT2: D7 = always HIGH input (tested at startup)
	INPT3: D7 = SHIFT key input (0 on startup / 1 = key pressed)
	INPT4: D7 = keyboard row 1 input (0 = key pressed)
	INPT5: D7 = keyboard row 3 input (0 = key pressed)
	SWCHA: D7 = tape recorder I/O ?
	       D6 = 1 -> increase key collumn (0 to 9)
	       D5 = 1 -> reset key collumn to 0 (if D4 = 0)
	       D5 = 0 -> enable RAM writing (if D4 = 1)
	       D4 = 1 -> map 2K of RAM at $1800 - $1fff
	       D3 = keyboard row 4 input (0 = key pressed)
	       D2 = keyboard row 2 input (0 = key pressed)
	       D1 = bank select high bit
	       D0 = bank select low bit
	
	keyboard collumn numbering:
	collumn 0 = 7 U J M
	collumn 1 = 6 Y H N
	collumn 2 = 8 I K ,
	collumn 3 = 2 W S X
	collumn 4 = 3 E D C
	collumn 5 = 0 P ENTER SPACE
	collumn 6 = 9 O L .
	collumn 7 = 5 T G B
	collumn 8 = 1 Q A Z
	collumn 9 = 4 R F V
*/
void InitCM(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		if(i & 0x800){
			ReadAccess[0x1000 + i] = &ReadCMhigh;
			WriteAccess[0x1000 + i] = &WriteCMhigh;
		}else{
			ReadAccess[0x1000 + i] = &ReadBS4K;
			WriteAccess[0x1000 + i] = &WriteROM4K;
		}
	}
	WriteAccess[0x280] = &WriteHotspotCM;
	Copy64K();
}


/*
	12 -- 8K United Appliance Ldt. [UA]

	2 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $0220
	select bank 1 by accessing $0240
*/
void InitUA(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	ReadAccess[0x220] = &ReadHotspotUA;
	WriteAccess[0x220] = &WriteHotspotUA;
	ReadAccess[0x240] = &ReadHotspotUA;
	WriteAccess[0x240] = &WriteHotspotUA;
	Copy64K();
}


/*
	13 -- 64K Homestar Runner / Paul Slocum [EF]

	16 4K ROM banks at $1000 - $1FFF
	select bank  0 by accessing $1FE0
	select bank  1 by accessing $1FE1
	select bank  2 by accessing $1FE2
	select bank  3 by accessing $1FE3
	select bank  4 by accessing $1FE4
	select bank  5 by accessing $1FE5
	select bank  6 by accessing $1FE6
	select bank  7 by accessing $1FE7
	select bank  8 by accessing $1FE8
	select bank  9 by accessing $1FE9
	select bank 10 by accessing $1FEA
	select bank 11 by accessing $1FEB
	select bank 12 by accessing $1FEC
	select bank 13 by accessing $1FED
	select bank 14 by accessing $1FEE
	select bank 15 by accessing $1FEF
*/
void InitEF(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 16; i++){
		ReadAccess[0x1fe0 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1fe0 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 0;
	Copy64K();
}


/*
	14 -- 512K Krokodile Cart / Andrew Davie [3E]

	up to 256 2K banks of ROM at $1000 - $17FF
	select ROM bank by writing bank number to address $003F
	2K of ROM fixed at $1800 - $ 1FFFF (this is always the 4th 2K bank in the ROM)
	32 1K banks of RAM at $1000 - $17FF
	select RAM bank by writing bank number to address $003E
	read from RAM at $1000 - $13FF
	write to  RAM at $1400 - $17FF
*/
void ReadBS3Elow(void){
	if(!ROMorRAM3E) DataBus = CartRom[(AddressBus & 0x7ff) + TVSlice0];
	else DataBus = Ram[(AddressBus & 0x3ff)+ TVSlice0];
}
void WriteBS3E(void){
	Ram[(AddressBus & 0x3ff) + TVSlice0] = DataBus;
}
void WriteHotspot3E_E(void){
	TVSlice0 = DataBus << 10;
	ROMorRAM3E = 1;
	(* TIARIOTWriteAccess[0x3e])();
}
void WriteHotspot3E_F(void){
	TVSlice0 = DataBus << 11;
	ROMorRAM3E = 0;
	(* TIARIOTWriteAccess[0x3f])();
}
void Init3E(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x800; i++){
		ReadAccess[0x1000 + i] = &ReadBS3Flow;
		ReadAccess[0x1800 + i] = &ReadBS3Fhigh;
	}
	for(i = 0; i < 0x400; i++){
		ReadAccess[0x1400 + i] = &WriteBS3E;
		WriteAccess[0x1400 + i] = &WriteBS3E;
	}
	WriteAccess[0x3e] = &WriteHotspot3E_E;
	WriteAccess[0x3f] = &WriteHotspot3E_F;
	Copy64K();
}


/*
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

/* These functions are handled in starpath.c */
void InitSP(void);


/*
	16 -- 16K Atari [F6]

	4 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF6
	select bank 1 by accessing $1FF7
	select bank 2 by accessing $1FF8
	select bank 3 by accessing $1FF9
*/
void InitF6(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 4; i++){
		ReadAccess[0x1ff6 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff6 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 6;
	Copy64K();
}


/*
	17 -- 32K Atari [F4]

	8 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF4
	select bank 1 by accessing $1FF5
	select bank 2 by accessing $1FF6
	select bank 3 by accessing $1FF7
	select bank 4 by accessing $1FF8
	select bank 5 by accessing $1FF9
	select bank 6 by accessing $1FFA
	select bank 7 by accessing $1FFB
*/
void InitF4(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 8; i++){
		ReadAccess[0x1ff4 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff4 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 4;

	Copy64K();
}


/*
	18 -- 64K Megaboy [MB]

	16 4K ROM banks at $1000 - $1FFF
	select banks by repeatedly accessing $1FF0
*/
void InitMB(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	ReadAccess[0x1ff0] = &ReadHotspotMB;
	WriteAccess[0x1ff0] = &WriteHotspotMB;
	HotspotAdjust = 6;

	Copy64K();
}


/*
	19 -- 12K CBS [FA]

	3 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing $1FF8
	select bank 1 by accessing $1FF9
	select bank 2 by accessing $1FFA
	256 bytes of RAM at $1000 - $11FF
	read from RAM at $1100 - $11FF
	write to  RAM at $1000 - $10FF
*/
void InitFA(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 3; i++){
		ReadAccess[0x1ff8 + i] = &ReadHotspotBS4K;
		WriteAccess[0x1ff8 + i] = &WriteHotspotBS4K;
	}
	HotspotAdjust = 8;

	for(i = 0; i < 0x100; i++){
		ReadAccess[0x1100 + i] = &ReadRAM256;
		WriteAccess[0x1100 + i] = &ReadRAM256;
		WriteAccess[0x1000 + i] = &WriteRAM256;
		ReadAccess[0x1000 + i] = &WriteRAM256;
	}
	Copy64K();
}


/*
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
	see David Crane's U.S. Patent 4,644,495, Feb 17,1987 for details
*/

//	These functions are handled in pitfall2.c
void InitP2(void);


/*
	22 -- 4A50

	see http://www.casperkitty.com/stella/cartfmt.htm for current format description
*/

void RBank4A50(void)
{
	if(!(AddressBus & 0x1000)){
		ReadHardware();
		if(((LastDataBus4A50 & 0xe0) == 0x60) && ((LastAddressBus4A50 >= 0x1000) || (LastAddressBus4A50 < 0x200))){
			if((AddressBus & 0x0f00) == 0x0c00){
				ROMorRAMHigh = 0;
				Bank4A50High = (AddressBus & 0xff) << 8;
			}else if((AddressBus & 0x0f00) == 0x0d00){
				ROMorRAMHigh = 1;
				Bank4A50High = (AddressBus & 0x7f) << 8;
			}else if((AddressBus & 0x0f40) == 0x0e00){
				ROMorRAMLow = 0;
				Bank4A50Low = (AddressBus & 0x1f) << 11;
			}else if((AddressBus & 0x0f40) == 0x0e40){
				ROMorRAMLow = 1;
				Bank4A50Low = (AddressBus & 0xf) << 11;
			}else  if((AddressBus & 0x0f40) == 0x0f00){
				ROMorRAMMiddle = 0;
				Bank4A50Middle = (AddressBus & 0x1f) << 11;
			}else if((AddressBus & 0x0f50) == 0x0f40){
				ROMorRAMMiddle = 1;
				Bank4A50Middle = (AddressBus & 0xf) << 11;
			}else if((AddressBus & 0x0f00) == 0x0400){
				Bank4A50Low = Bank4A50Low ^ 0x800;
			}else if((AddressBus & 0x0f00) == 0x0500){
				Bank4A50Low = Bank4A50Low ^ 0x1000;
			}else if((AddressBus & 0x0f00) == 0x0800){
				Bank4A50Middle = Bank4A50Middle ^ 0x800;
			}else if((AddressBus & 0x0f00) == 0x0900){
				Bank4A50Middle = Bank4A50Middle ^ 0x1000;
			}
		}
		if((AddressBus & 0xf75) == 0x74){
			ROMorRAMHigh = 0;
			Bank4A50High = DataBus << 8;
		}else if((AddressBus & 0xf75) == 0x75){
			ROMorRAMHigh = 1;
			Bank4A50High = (DataBus & 0x7f) << 8;
		}else if((AddressBus & 0xf7c) == 0x78){
			if((DataBus & 0xf0) == 0){
				ROMorRAMLow = 0;
				Bank4A50Low = (DataBus & 0xf) << 11;			
			}else if((DataBus & 0xf0) == 0x40){
				ROMorRAMLow = 1;
				Bank4A50Low = (DataBus & 0xf) << 11;			
			}else if((DataBus & 0xf0) == 0x90){
				ROMorRAMMiddle = 0;
				Bank4A50Middle = ((DataBus & 0xf) | 0x10) << 11;							
			}else if((DataBus & 0xf0) == 0xc0){
				ROMorRAMMiddle = 1;
				Bank4A50Middle = (DataBus & 0xf) << 11;							
			}
		}
	}else{
		if((AddressBus & 0x1800) == 0x1000){
			if(ROMorRAMLow) DataBus = Ram[(AddressBus & 0x7ff) + Bank4A50Low];
			else DataBus = CartRom[(AddressBus & 0x7ff) + Bank4A50Low];
		}else if(((AddressBus & 0x1fff) >= 0x1800) && ((AddressBus & 0x1fff) <= 0x1dff)){
			if(ROMorRAMMiddle) DataBus = Ram[(AddressBus & 0x7ff) + Bank4A50Middle];
			else DataBus = CartRom[(AddressBus & 0x7ff) + Bank4A50Middle];
		}else if((AddressBus & 0x1f00) == 0x1e00){
			if(ROMorRAMHigh) DataBus = Ram[(AddressBus & 0xff) + Bank4A50High];
			else DataBus = CartRom[(AddressBus & 0xff) + Bank4A50High];
		}else if((AddressBus & 0x1f00) == 0x1f00){
			DataBus = CartRom[(AddressBus & 0xff) + 0xff00];
		  	if(((LastDataBus4A50 & 0xe0) == 0x60) && ((LastAddressBus4A50 >= 0x1000) || (LastAddressBus4A50 < 0x200)))
				Bank4A50High = (Bank4A50High & 0xf0ff) | ((AddressBus & 0x8) << 8) | ((AddressBus & 0x70) << 4);
		}
	}
	LastDataBus4A50 = DataBus;
	LastAddressBus4A50 = AddressBus & 0x1fff;
}

void WBank4A50(void)
{
	if(!(AddressBus & 0x1000)){
		WriteHardware();
		if(((LastDataBus4A50 & 0xe0) == 0x60) && ((LastAddressBus4A50 >= 0x1000) || (LastAddressBus4A50 < 0x200))){
			if((AddressBus & 0x0f00) == 0x0c00){
				ROMorRAMHigh = 0;
				Bank4A50High = (AddressBus & 0xff) << 8;
			}else if((AddressBus & 0x0f00) == 0x0d00){
				ROMorRAMHigh = 1;
				Bank4A50High = (AddressBus & 0x7f) << 8;
			}else if((AddressBus & 0x0f40) == 0x0e00){
				ROMorRAMLow = 0;
				Bank4A50Low = (AddressBus & 0x1f) << 11;
			}else if((AddressBus & 0x0f40) == 0x0e40){
				ROMorRAMLow = 1;
				Bank4A50Low = (AddressBus & 0xf) << 11;
			}else  if((AddressBus & 0x0f40) == 0x0f00){
				ROMorRAMMiddle = 0;
				Bank4A50Middle = (AddressBus & 0x1f) << 11;
			}else if((AddressBus & 0x0f50) == 0x0f40){
				ROMorRAMMiddle = 1;
				Bank4A50Middle = (AddressBus & 0xf) << 11;
			}else if((AddressBus & 0x0f00) == 0x0400){
				Bank4A50Low = Bank4A50Low ^ 0x800;
			}else if((AddressBus & 0x0f00) == 0x0500){
				Bank4A50Low = Bank4A50Low ^ 0x1000;
			}else if((AddressBus & 0x0f00) == 0x0800){
				Bank4A50Middle = Bank4A50Middle ^ 0x800;
			}else if((AddressBus & 0x0f00) == 0x0900){
				Bank4A50Middle = Bank4A50Middle ^ 0x1000;
			}
		}
		if((AddressBus & 0xf75) == 0x74){
			ROMorRAMHigh = 0;
			Bank4A50High = DataBus << 8;
		}else if((AddressBus & 0xf75) == 0x75){
			ROMorRAMHigh = 1;
			Bank4A50High = (DataBus & 0x7f) << 8;
		}else if((AddressBus & 0xf7c) == 0x78){
			if((DataBus & 0xf0) == 0){
				ROMorRAMLow = 0;
				Bank4A50Low = (DataBus & 0xf) << 11;			
			}else if((DataBus & 0xf0) == 0x40){
				ROMorRAMLow = 1;
				Bank4A50Low = (DataBus & 0xf) << 11;			
			}else if((DataBus & 0xf0) == 0x90){
				ROMorRAMMiddle = 0;
				Bank4A50Middle = ((DataBus & 0xf) | 0x10) << 11;							
			}else if((DataBus & 0xf0) == 0xc0){
				ROMorRAMMiddle = 1;
				Bank4A50Middle = (DataBus & 0xf) << 11;							
			}
		}
	}else{
		if((AddressBus & 0x1800) == 0x1000){
			if(ROMorRAMLow) Ram[(AddressBus & 0x7ff) + Bank4A50Low] = DataBus;
		}else if(((AddressBus & 0x1fff) >= 0x1800) && ((AddressBus & 0x1fff) <= 0x1dff)){
			if(ROMorRAMMiddle) Ram[(AddressBus & 0x7ff) + Bank4A50Middle] = DataBus;
		}else if((AddressBus & 0x1f00) == 0x1e00){
			if(ROMorRAMHigh) Ram[(AddressBus & 0xff) + Bank4A50High] = DataBus;
		}else if((AddressBus & 0x1f00) == 0x1f00){
		  	if(((LastDataBus4A50 & 0xe0) == 0x60) && ((LastAddressBus4A50 >= 0x1000) || (LastAddressBus4A50 < 0x200)))
				Bank4A50High = (Bank4A50High & 0xf0ff) | ((AddressBus & 0x8) << 8) | ((AddressBus & 0x70) << 4);
		}
	}
	LastDataBus4A50 = DataBus;
	LastAddressBus4A50 = AddressBus & 0x1fff;
}


/*
	23 -- 0840 EconoBanking / supercat

	2 4K ROM banks at $1000 - $1FFF
	select bank 0 by accessing %xxx0 1xxx x0xx xxxx ($0800)
	select bank 1 by accessing %xxx0 1xxx x1xx xxxx ($0840)
*/
void Init0840(void){
	int i;
	
	for(i = 0; i < 0x1000; i++){
		ReadAccess[i] = TIARIOTReadAccess[i];
		WriteAccess[i] = TIARIOTWriteAccess[i];
		ReadAccess[0x1000 + i] = &ReadBS4K;
		WriteAccess[0x1000 + i] = &WriteROM4K;
	}
	for(i = 0; i < 0x800; i++){
		ReadAccess[0x800 + i] = &ReadHotspotUA;
		WriteAccess[0x800 + i] = &WriteHotspotUA;
	}
	Copy64K();
}



void (* InitMemoryMap[24])(void) = {
	Init4K,		//   0 -- 4K Atari
	InitCV,		//   1 -- Commavid
	InitF8SC,	//   2 -- 8K superchip
	InitE0,		//   3 -- 8K Parker Bros.
	Init3F,		//   4 -- 8K Tigervision
	InitFE,		//   5 -- 8K Flat
	InitF6SC,	//   6 -- 16K superchip
	InitE7,		//   7 -- 16K M-Network
	InitF4SC,	//   8 -- 32K superchip
	InitF8,		//   9 -- 8K Atari - banks swapped
	InitCM,		//  10 -- Compumate
	Init3F,		//  11 -- 512K Tigervision
	InitUA,		//  12 -- 8K UA Ltd.
	InitEF,		//  13 -- 64K Homestar Runner / Paul Slocum
	Init3E,		//  14 -- 3E bankswitching (large 3F + 32k RAM)
	InitSP,		//  15 -- Starpath
	InitF6,		//  16 -- 16K Atari
	InitF4,		//  17 -- 32K Atari
	InitMB,		//  18 -- Megaboy
	InitFA,		//  19 -- 12K
	InitF8,		//  20 -- 8K Atari
	InitP2,		//  21 -- Pitfall2
	Init4K,		//	22 -- 4A50 / supercat
	Init0840	//	23 -- 0840 EconoBanking
};


void DetectBySize() {

	int i, j;

	if( CartSize == 480*0x400 ) {
		BSType = 14;	/* 3E bankswitching with extra RAM */
		return;
	}
		
	if( CartSize % 8448 == 0 ) { /* multiple of 8448 bytes? */
		SetStarpath(); /* Supercharger image */
		return;
	}

	if( CartSize > 0x10000 ) {
		BSType = 11; /* large TigerVision game */
		return;
	}

	switch(CartSize) {
		case 0x2000: /* 8k cart */
			{
				RomBank = 0x1000; /* need this for moonsweep and lancelot */
				BSType = 2;
				/* Superchip RAM ? */
				for(i=0; i<2; i++)
				{
					for(j=0; j<256; j++)
					{
						if(CartRom[0]!=CartRom[i*0x1000+j]) BSType = 20;
					}
				}
				break;
			}

		case 0x3000: /* 12k cart */
			{
				BSType = 19;
				break;
			}

		case 0x4000: /* 16k cart */
			{
				BSType = 6;
				/* Superchip RAM ? */
				for(i=0; i<4; i++)
				{
					for(j=0; j<256; j++)
					{
						if(CartRom[0]!=CartRom[i*0x1000+j]) BSType = 16;
					}
				}
				break;
			}

		case 0x8000: /* 32k cart */
			{
				BSType = 8;
				/* Superchip RAM ? */
				for(i=0; i<8; i++)
				{
					for(j=0; j<256; j++)
					{
						if(CartRom[0]!=CartRom[i*0x1000+j]) BSType = 17;
					}
				}
				break;
			}

		case 0x28ff: /* Pitfall II cart */
			{
				SetPitfallII();
				break;
			}

		case 0x10000:
			{
				if(((CartRom[0xfffd] & 0x1f) == 0x1f) &&
				   (CartRom[CartRom[0xfffd] * 256 + CartRom[0xfffc]] == 0x0c) &&
				   ((CartRom[CartRom[0xfffd] * 256 + CartRom[0xfffc] + 2] & 0xfe) == 0x6e))
				   /* program starts at $1Fxx with NOP $6Exx or NOP $6Fxx ? */
					BSType = 22; /* 64K cart with 4A50 bankswitching */
				else BSType = 18; /* Megaboy 64k cart */
				break;
			}

		case 6144: /* Supercharger image */
			{
				SetStarpath();
				break;
			}

		default: /* 4k (non bank-switched)? */
			break;
	}
}

/* setup bank switching scheme */
void SetupBanks() {
	int i;

	RomBank = 0;
	PBSlice0 = 0;
	PBSlice1 = 1 * 0x400;
	PBSlice2 = 2 * 0x400;
	TVSlice0 = 0;
	TVSlice1 = 3 * 0x800;
	MNRamSlice = 0;
	Pitfall2 = 0;
	Starpath = 0;
	Bank4A50Low = 0;
	ROMorRAMLow = 0;
	Bank4A50Middle = 0;
	ROMorRAMMiddle = 0;
	Bank4A50High = 0;
	ROMorRAMHigh = 0;
	LastDataBus4A50 = 0xff;
	LastAddressBus4A50 = 0xffff;

	if( BSType == 0 )
		DetectBySize();
	else if( BSType == 1 ) {
		/* CommaVid RAM games might have RAM state stored in first 2K of ROM */
		for(i=0; i<2048; i++)
			Ram[i] = CartRom[i];
	}
	else if( BSType == 10 ) {
		RomBank = 0x3000;
		//InitCompuMate();
	}

	/* make last 2k bank fixed for 3E and 3F+ games: */
	if ((BSType == 11) || (BSType == 14))
		TVSlice1 = CartSize - 2048;
	
	/* fill memory map with pointers to proper handler functions */
	(* InitMemoryMap[BSType])();
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).	
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
