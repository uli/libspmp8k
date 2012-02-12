/*
	z26 -- an Atari 2600 emulator
*/

#define Z26_RELEASE "z26 -- An Atari 2600 Emulator"

void QueueSoundBytes();
void position_game();
//void srv_print();
void srv_Events();
void show_scanlines();
void show_transient_status();
void set_status(char *status);
void gui();
void SDL_Quit();

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <dirent.h>		// not in vc++ -- must be supplied
#include <sys/stat.h>
#include <time.h>
#include "libgame.h"

#ifdef LINUX
#include <unistd.h>		// for chdir
#endif


// needed for vc++ -- works on gcc

int strcasecmp(const char *s1, const char *s2)
{
	while(tolower(*s1) == tolower(*s2++))
	{
		if(*s1++ == '\0') return 0;
	}

	return *(unsigned char *)s1 - *(unsigned char *)(s2 - 1);
}

FILE *parmfp = NULL;	// parameter file pointer

/*
#include "SDL.h"		// not in vc++ or gcc -- must be supplied
#include "SDL_audio.h"
#include "SDL_opengl.h"
*/

typedef unsigned int			dd;		/* define double */
typedef unsigned short int		dw;		/* define word */
typedef unsigned char			db;		/* define byte */

#include "globals.c"
#include "ct.c"
#include "carts.c"
#include "cli.c"
#include "kidvid.c"
#include "palette.c"
#include "controls.c"
#include "sdlsrv.c"
#include "text.c"
//#include "pcx.c"
#include "position.c"
//#include "gui.c"
#include "2600core.c"

int total_ticks;
double seconds;

// Cybergame only
void (*overclock)(void) = (void *)0x0044a960;

int main()
{
	libgame_init();

	// overclock();

	srand(get_time());
	def_LoadDefaults();
	LaunchedFromCommandline = 0;

	strcpy(FileName, "2600.rom");

	ROMLoaded = cli_LoadROM(FileName);
	ROMSeen = 1;

	if (!ROMLoaded)
	{
		sprintf(msg, "File not found:\n\n%s", FileName);
		printf(msg);
		srv_print(msg);
		exit(1);
	}

	/*
	if (argc == 1)
	{
		StartInGUI = 1;
		GamePaused = 1;
		cli_ReadParms("z26.gui");
	}
	else
	{
		LaunchedFromCommandline = 1;
		cli_CommandLine(argc, argv);
	}
	*/

	Init_SDL();

	c_emulator();		   /* call emulator */

	/*
	if(GrabInput)
		SDL_WM_GrabInput(SDL_GRAB_OFF);

	if(TraceEnabled && (zlog != NULL)) {
		fprintf(zlog, "Exiting emulator with status %d\n", MessageCode);
		fflush(zlog);
		fclose(zlog);
	}

	srv_sound_off();
	*/
	SDL_Quit();

	if(parmfp) {
		fflush(parmfp);
		fclose(parmfp);
	}

	return MessageCode;
}

void SDL_Quit()
{
	gfx_free_image(srv_screen_handle);
}
/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.
	z26 is released subject to the terms and conditions of the
	GNU General Public License Version 2 (GPL).
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
