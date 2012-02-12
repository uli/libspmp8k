/*
	z26 command line stuff
*/

/*
	load next Starpath Rom
*/

#define SC_LOAD0_SAVE 515330    // where in ROM buffer to save Load 0

void cli_LoadNextStarpath(void)
{
    int i,j, LoadCount, LoadNum;
	int pageadr, pagebyte, pagecount;
	db *p;
	db *q;

	SC_StartAddress=0;
	LoadCount=61;
	LoadNum=SC_ControlByte;
	for (i = 1; i < 61; i++)
	{
		if (CartRom[i*8448 + 0x2005] == LoadNum) LoadCount=i;
	}
	LoadNum=LoadCount;

	if (LoadNum == 61) return;

	pagecount = CartRom[LoadNum*8448 + 0x2003];

	for (i = 0; i < pagecount; i++)
	{
		pagebyte = CartRom[LoadNum*8448 + 0x2010 + i];
		pageadr = ((pagebyte & 3) * 0x800) + ((pagebyte & 0x1f) >> 2) * 256;

		p = CartRom + pageadr;
		q = CartRom + LoadNum*8448 + i*256;

		for (j = 0; j < 256; j++)
		{
			*p++ = *q++;
		}
	}
	SC_StartAddress=CartRom[LoadNum*8448+0x2001]*256+CartRom[LoadNum*8448+0x2000];
	SC_ControlByte=CartRom[LoadNum*8448+0x2002];
}

/*
	reload a Starpath file
*/

void cli_ReloadStarpath(void)
{
	int i,j;
	int pageadr, pagebyte, pagecount;
	db *p;
	db *q;

	if(CartSize == 6144)
	{
		for(i = 0; i < 6144; i++)
		{
			CartRom[i]=CartRom[SC_LOAD0_SAVE+i];
		}
		SC_StartAddress=CartRom[0x17fd]*256+CartRom[0x17fc];
		SC_ControlByte=0x0d;
	}
	else
	{
		pagecount = CartRom[0x2003];

		for (i = 0; i < pagecount; i++)
		{
			pagebyte = CartRom[0x2010 + i];
			pageadr = ((pagebyte & 3) * 0x800) + ((pagebyte & 0x1f) >> 2) * 256;

			p = CartRom + pageadr;
			q = CartRom + SC_LOAD0_SAVE + i*256;
			for (j = 0; j < 256; j++)
			{
				*p++ = *q++;
			}
		}
	SC_StartAddress=CartRom[0x2001]*256+CartRom[0x2000];
	SC_ControlByte=CartRom[0x2002];
	}
}


#define CRC16_REV 0xA001		/* CRC-16 polynomial reversed */
#define CRC32_REV 0xA0000001	/* CRC-32 polynomial reversed */

/*
	used for generating the CRC lookup table
*/

dd crcrevhware(dd data, dd genpoly, dd accum) 
{
	int i;
	data <<= 1;
	for (i=8; i>0; i--) {
		data >>= 1;
		if ((data ^ accum) & 1)
			accum = (accum >> 1) ^ genpoly;
		else
			accum >>= 1;
	}
	return(accum);
}


/*
	update CRC
*/

void ucrc(dd data) 
{
   crc = (crc >> 8) ^ crcrevhware((crc ^ data) & 0xff,CRC32_REV,0);
}


int cli_calc_CRC(char *filename)
{
	FILE *fp;
	db *p;
	int ch;

	if (filename == NULL) return(0);
	fp = fopen(filename, "rb");
	if (fp == NULL)	return(0);
			
	p = CartRom;
	CartSize = 0;
	crc = 0;

	while ( (ch = getc(fp)) != EOF )
	{
		*p++ = (db) ch;
		ucrc(ch);
		++CartSize;
		if (CartSize > 0x80000) break;
	}

	fclose(fp);
	return(1);
}


/*
	load a ROM image

	gets called from gui also -- special ROM setup can go here
*/

int cli_LoadROM(char *filename)
{

db SCBIOS[188] = { 
0xa5,0xfa,0x85,0x81,0x4c,0x0e,0xf8,0xff,
0xff,0xff,0xa9,0x00,0x85,0x81,0xa9,0x00,
0x85,0x1b,0x85,0x1c,0x85,0x1d,0x85,0x1e,
0x85,0x1f,0x85,0x19,0x85,0x1a,0x85,0x08,
0x85,0x01,0xa9,0x10,0x85,0x21,0x85,0x02,
0xa2,0x07,0xca,0xca,0xd0,0xfd,0xa9,0x00,
0x85,0x20,0x85,0x10,0x85,0x11,0x85,0x02,
0x85,0x2a,0xa9,0x05,0x85,0x0a,0xa9,0xff,
0x85,0x0d,0x85,0x0e,0x85,0x0f,0x85,0x84,
0x85,0x85,0xa9,0xf0,0x85,0x83,0xa9,0x74,
0x85,0x09,0xa9,0x0c,0x85,0x15,0xa9,0x1f,
0x85,0x17,0x85,0x82,0xa9,0x07,0x85,0x19,
0xa2,0x08,0xa0,0x00,0x85,0x02,0x88,0xd0,
0xfb,0x85,0x02,0x85,0x02,0xa9,0x02,0x85,
0x02,0x85,0x00,0x85,0x02,0x85,0x02,0x85,
0x02,0xa9,0x00,0x85,0x00,0xca,0x10,0xe4,
0x06,0x83,0x66,0x84,0x26,0x85,0xa5,0x83,
0x85,0x0d,0xa5,0x84,0x85,0x0e,0xa5,0x85,
0x85,0x0f,0xa6,0x82,0xca,0x86,0x82,0x86,
0x17,0xe0,0x0a,0xd0,0xc3,0xa9,0x02,0x85,
0x01,0xa5,0x81,0xa2,0x1c,0xa0,0x00,0x84,
0x19,0x84,0x09,0x94,0x81,0xca,0x10,0xfb,
0xa2,0xff,0x29,0xff,0xd0,0x03,0x4c,0xf1,
0xff,0x4c,0xf0,0xff
};

	int i;

	if (cli_calc_CRC(filename) == 0)
	{
		sprintf(msg, " Game not supported.");
		srv_print(msg);
		return(0);	// not supported in C engine
	}

	if (CartSize == 2048)	/* 2K cart -- copy lower to upper */
	{
		for (i = 0; i < 2048; i++)
		{
			CartRom[2048+i] = CartRom[i];
		}
	}

	if ((CartSize == 6144)||(CartSize % 8448 == 0))
	/* Starpath image -- reload according to page table */
	{
		for (i = 0; i < 8448; i++)      /* save first SC load */
		{
			CartRom[i+SC_LOAD0_SAVE] = CartRom[i];
		}

		for (i = 0; i < 0x2000; i++)    /* fill everything with Starpath halts */
		{
			CartRom[i] = 0x52;
		}

		for (i = 0; i< 188; i++)	/* generate fake SC BIOS */
		{
			CartRom[i + 0x1800] = SCBIOS[i];
		}

		CartRom[0x1ffc]=0x0a;	   /* setup BIOS reset vector */
		CartRom[0x1ffd]=0xf8;

		cli_ReloadStarpath();
	}

	if (Lookup(BLACKLIST))
	{
		sprintf(msg, " Game not supported.");
		srv_print(msg);
		return(0);
	}

	return(1);
}


char cli_controllers[12][3] =
	{"JS","PC","KP","DC","LG","CM","KV","ML","ST","TB","AM","NC"};

int GetController(char *p)
{
	int i, LocalController;

	LocalController = -1;			// set left controller
	for (i=0; i<12; i++)
	{
		if (strncmp(p, cli_controllers[i],2) == 0)
			LocalController = i;
	}
	if (LocalController == -1)
	{
		sprintf(msg, "Unknown controller type: %s", p);
		srv_print(msg);
		SDL_Quit();
		exit(1);
	}
	return(LocalController);
}

/*
	Command Line interpreter
*/

FILE *zlog;

void cli_InterpretParm(char *p)
{
	int ch, parm;
	double fparm;
	int i;
	
	p++;
	ch = *p++;
	parm = atol(p);
	fparm = atof(p);

	switch (ch)
	{
	case 'd':  	dsp = parm;				break;	// sound processing
	case 'e':	Narrow = parm;			break;	// width adjustment
	case 'h':	Tall = parm;			break;	// height adjustment
	case 'q':  	quiet = 1;				break;	// no sound
	case 'S':	DoScanline = 1;			break;	// scanline display
	case 'C':	theme = parm & 0x70;	break;	// color theme for the GUI
	case 'o':	SimColourLoss = 1;		break;	// simulate colour loss
	case 'n':	ShowLineCount = 1;		break;	// show line count and framerate
	case '!':	DoInterlace = 1;		break;	// do interlace

	case '0':  	UserP0Diff = 1;
				IOPortB |= 64;				
				break;							// p0 hard
			
	case '1':  	UserP1Diff = 1;
				IOPortB |= 128;				
				break;							// p1 hard
			
	case 'b':  	UserConsoleBW = 1;
				IOPortB &= 0xc3;			
				break;							// black and white
	
	case 'c':  	UserPaletteNumber = parm;		// palette
				PaletteNumber = UserPaletteNumber;
				if(parm == 2) {
					IOPortB &= 0xf7;
				}
				break;

	case 'g':	UserBankswitch = parm;
				if (UserBankswitch > 23) UserBankswitch = 0xff;	// bankswitch scheme
				else if (UserBankswitch < 0) UserBankswitch = 0xff;
				BSType = UserBankswitch;
				break;
			
	case 'u':  	break;							// first line to scan
			
	case 's':	SQ_Max = parm;					// size of sound queue
				if (SQ_Max > 65536)	SQ_Max = 65536;
				if (SQ_Max < 1024)	SQ_Max = 1024;
				break;

	case '4':	UserAllowAll4 = 1;
				AllowAll4 = UserAllowAll4;
				break;							// allow all four directions
	
	case 'f':	UserDepth = parm;				// user specified scanline depth
				Depth = UserDepth;			
				break;

	case 'v':	if (parm >= 10)					// set video mode
					FullScreen = 0;
				else
					FullScreen = 1;

       			VideoMode = parm % 10;
				break;

	case ')':	LeftController = GetController(p);	// set left controller
				UserLeftController = LeftController;
				break;		

	case '(':	RightController = GetController(p);	// set right controller
				UserRightController = RightController;
				break;

	case 'w':	UserSwapPortsFlag = 1;		
				SwapPortsFlag = UserSwapPortsFlag;	
				break;							// swap controller ports

	case 'l':	Lightgun = parm;
				UserLightgun = Lightgun;
				break;						// lightgun
			
	case 'a':	LGadjust = parm;
				UserLGadjust = LGadjust;
				break;						// lightgun adjust

	case 'M':	MouseRude = parm;			break;	// mouse rude
	case 'G':	GrabInput = 1;				break;	// grab input

	case 'P':	PaddleAdjust = parm;			break;	// paddle adjust
			
	case 'p':  	UserPaddleSensitivity = parm;
				PaddleSensitivity = (parm & 0xf) << 1;	
				break;						// paddle sensitivity

	case 'm':	if (parm < 100)					// mouse adjust
				{
					MouseBaseX = parm & 3;
				} 
				else
				{
					parm = parm - 100;
					MouseBaseX = parm / 10;
					MouseBaseY = parm % 10;
					MPdirection = 1;  /* set directions for Marble Craze */
				}
				UserMouseBaseX = MouseBaseX;
				UserMouseBaseY = MouseBaseY;
				break;
			
	case 'i':	for (i = 0; i < strlen(p); i++)			// inactivate PC controller
				{
					switch (p[i])
					{
					case 'J':	JoystickEnabled = 0;		break;
					case 'M':	MouseEnabled = 0;			break;
					case 'K':	KeyboardEnabled = 0;		break;
					case 'S':	StelladaptorEnabled = 0;	break;
					case ' ':								break;
					default:
						sprintf(msg, "Bad PC controller seen: %c", p[i]);
						srv_print(msg);
						SDL_Quit();
						exit(1);
						break;
					}
				
					if (p[i] == ' ') break;
				}
				UserJoystickEnabled = JoystickEnabled;
				break;
			
	case 't':	if (parm)					// trace mode
					TraceCount = parm;
				else
					TraceCount = 0xff;			// hmmm...

				OldTraceCount = TraceCount;
				TraceEnabled = 1; // controls.c checks this
				UserTraceEnabled = TraceEnabled;

				/* -tt means `start with trace disabled, until I hit F11' */
				if(*p == 't')
					TraceCount = 0;

				zlog = fopen("z26.log", "w");
				if (zlog == NULL)
				{
					sprintf(msg, "Couldn't build log file.");
					srv_print(msg);
					TraceCount = 0;
					TraceEnabled = 0;
					OldTraceCount = 0;
				}
				break;

	default:   	sprintf(msg, "Bad switch seen: -%c", ch);
				srv_print(msg);
				SDL_Quit();
				exit(1);
	}
}

void cli_WriteParms(int argc, char *argv[])
{
	int i, ch;
	FILE *fp;
	char *p;

	fp = fopen("z26.cli", "w");
	if (fp == NULL)
	{
		sprintf(msg, "Couldn't build z26.cli file.");
		srv_print(msg);
		SDL_Quit();
		exit(1);
	}

	i = 1;
	argc--;		/* skip over the filename */

	while (argc--)
	{
		p = argv[i++];
		if (*p == '-')
		{		
			while ( (ch = *p++) )
			{
				fputc(ch, fp);
			}
			
			fputc(' ', fp);
		}
	}

	fputc(0, fp);
	fclose(fp);
}

void cli_SaveParms()
{
	FILE *fp;

	if (parmfp == NULL)
		parmfp = fopen("z26.gui", "w+");
	else
		rewind(parmfp);

	if (parmfp == NULL)
	{
		sprintf(msg, "Couldn't find z26.gui file.");
		srv_print(msg);
		SDL_Quit();
		exit(1);
	}
	
	rewind(parmfp);
	fp = parmfp;
	
	fprintf(fp, "-v");					// (-v) do video mode
	
	if (!FullScreen)		fputc('1', fp);

	fprintf(fp, "%1d ",VideoMode);

	if (UserPaletteNumber != 0xff)	fprintf(fp, "-c%1d ", UserPaletteNumber);
	if (UserBankswitch != 0xff)		fprintf(fp, "-g%d ", UserBankswitch);
	if (UserLeftController != 0xff)	fprintf(fp, "-)%s ", cli_controllers[UserLeftController]);
	if (UserRightController != 0xff) fprintf(fp, "-(%s ", cli_controllers[UserRightController]);
	if (UserAllowAll4 != 0xff)		fprintf(fp, "-4 ");
	if (UserSwapPortsFlag == 1)		fprintf(fp, "-w ");
	if (ShowLineCount)				fprintf(fp, "-n ");
	if (quiet)						fprintf(fp, "-q ");
	if (dsp != 1)					fprintf(fp, "-d%1d ", dsp);
	if (Narrow)						fprintf(fp, "-e%d ", Narrow);
	if (Tall)						fprintf(fp, "-h%d ", Tall);
	if (SimColourLoss)				fprintf(fp, "-o ");
	if (DoInterlace)				fprintf(fp, "-! ");
	if (UserDepth != 60)			fprintf(fp, "-f%d ", UserDepth);
	if (DoScanline)					fprintf(fp, "-S ");
	/*if (theme)*/					fprintf(fp, "-C%d ", theme);
	if (SQ_Max != 4096)				fprintf(fp, "-s%d ", SQ_Max);
	if (UserP0Diff)					fprintf(fp, "-0 ");
	if (UserP1Diff)					fprintf(fp, "-1 ");
	if (UserConsoleBW)				fprintf(fp, "-b ");
	if (!KeyboardEnabled)			fprintf(fp, "-iK ");
	if (!MouseEnabled)				fprintf(fp, "-iM ");
	if (!UserJoystickEnabled)		fprintf(fp, "-iJ ");
	if (!StelladaptorEnabled)		fprintf(fp, "-iS ");
	if (MouseRude)					fprintf(fp, "-M1 ");
	if (GrabInput)					fprintf(fp, "-G ");
	if (UserMouseBaseX != 0xff && UserMouseBaseY == 0xff)
		fprintf(fp, "-m%d ", UserMouseBaseX);
	if (UserMouseBaseY != 0xff)	
	{
		if (UserMouseBaseX == 0xff) UserMouseBaseX = 0;
		fprintf(fp, "-m%d ", 100+UserMouseBaseX*10+UserMouseBaseY);
	}
	if (UserPaddleSensitivity)		fprintf(fp, "-p%d ", UserPaddleSensitivity);
	if (PaddleAdjust)				fprintf(fp, "-P%d ", PaddleAdjust);
	if (UserLightgun)				fprintf(fp, "-l%d ", UserLightgun);
	if (UserLGadjust != 5)			fprintf(fp, "-a%d ", UserLGadjust);
	if (UserTraceCount == 1)		fprintf(fp, "-t ");
	else if (UserTraceCount == 0xff) fprintf(fp, "-tt ");

	fprintf(fp, "\n");	// end-of-file
	fflush(fp);
	
//	fclose(fp);
}


void cli_ReadParms(char *Filename)
{
	int ch, i;
	FILE *fp;
	char ParmString[1024];
	char *p;

	if (parmfp == NULL)
		parmfp = fopen(Filename, "r+");
	else
		rewind(parmfp);
		
	if (parmfp == NULL) return;
		
	fp = parmfp;
	
	i = 0;

	while ( (( ch = fgetc(fp)) != EOF) && (i <= 1022) )
		ParmString[i++] = ch;

	ParmString[i] = 0;
	i = 0;

	p = ParmString;

	while (*p == '-')
	{
		cli_InterpretParm(p);
		while (!isspace(*p++)) ;
	}
	
//	fclose(fp);	
}


char FileName[260];
char ROMLoaded = 0; char ROMSeen = 0;


void cli_write_CRC(char *filename)
{
	FILE *xfp;
	
	xfp = fopen("z26.crc", "a");
	if (xfp == NULL)
		srv_print("Couldn't open CRC file.");
	else
	{
		fprintf(xfp,"  0x%08x,  /* %s */\n", crc, filename);
		fclose(xfp);
	}
}


void cli_CommandLine(int argc, char *argv[])
{
	long int i;
	int cnt;
	char *p;
	FILE *fp;

	ShowFPS = 0;				/* resets flag for displaying FPS count */

	fp = fopen("z26.cli", "r");
	if (fp == NULL)				// when running from command line, if .cli file doesn't exist ...
	{
		cli_ReadParms("z26.gui");	//    ... use .gui file
	}
	else
	{
		fclose(fp);
		cli_ReadParms("z26.cli");	//    ... else use .cli file
	}

	i = 1;
	cnt = argc - 1;				/* skip over the program name */

	while(cnt--)
	{
		p = argv[i++];
		if (*p == '-')
			cli_InterpretParm(p);
		else
		{	
			strncpy(FileName,p,255);
			if (!strchr(FileName,'.'))
				strcat(FileName,".bin");
			ROMLoaded = cli_LoadROM(FileName);
			ROMSeen = 1;
			if (TraceEnabled) {
				fprintf(zlog,"Loaded ROM: %s\n\n", FileName);
				fprintf(zlog,
						"(Frame Line Cycle Clock) "
						"(P0_Pos P1_Pos M0_Pos M1_Pos BL_Pos)  "
						"flags  A X Y SP  Code  Disasm\n");
			}
		}
	}

	if (!ROMSeen)
	{
		cli_WriteParms(argc, argv);
		sprintf(msg, "\nParameters recorded.\n\nTo restore defaults\ndelete z26.cli.\n");
		srv_print(msg);
		SDL_Quit();
		exit(1);
	}

	if (!ROMLoaded)
	{
		sprintf(msg, "File not found:\n\n%s", FileName);
		srv_print(msg);
		SDL_Quit();
		exit(1);
	}
}


/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).	
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
