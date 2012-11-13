/*
	sdlvideo.c -- SDL video code
*/

//const SDL_VideoInfo *screen_info;

#include "sdlpixcopy.c"

void ClearScreenBuffers()
{
	memset(RealScreenBuffer1, 0,	MaxLines*tiawidth);
	/*
	memset(RealScreenBuffer2, 0x80, MaxLines*tiawidth);
	memset(RealScreenBuffer3, 0,	MaxLines*tiawidth);
	memset(RealScreenBuffer4, 0x80, MaxLines*tiawidth);

	memset(texture_buffer, 0, 4*1024*1024);
	*/
}


void CreateScreen()	// need to be able to force video mode change
{
#if 0
	gl_CreateScreen();

	srv_pitch = srv_screen->pitch;

	SDL_WM_SetCaption(FileName, FileName);

	if (MouseRude|FullScreen)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
#endif
}


/*
** set up the palette
*/

void srv_SetPalette()
{
	int i;

	printf("srv_SetPalette\n");

	dd hi = Depth;
	dd med = (Depth + 100)/2;

	db red, grn, blu;
	//SDL_Color palette[256];

	MCatchFreeImage(srv_screen_handle);

	GeneratePalette();
	for ( i=0; i<128; i++)
	{
		red = PCXPalette[3*i];
		grn = PCXPalette[3*i+1];
		blu = PCXPalette[3*i+2];

		/*
		palette[i].r = red;
		palette[i].g = grn;
		palette[i].b = blu;
		*/

		srv_colortab_hi[i] =  ((red & 0xf8) << 8) | ((grn & 0xfc) << 3) | ((blu & 0xf8) >> 3);
		/*
		srv_colortab_hi[i] =  SDL_MapRGB(srv_screen->format, red, grn, blu);
		srv_colortab_med[i] = SDL_MapRGB(srv_screen->format, (red*med)/10, (grn*med)/100, (blu*med)/100);
		srv_colortab_low[i] = SDL_MapRGB(srv_screen->format, (red*hi)/100,  (grn*hi)/100,  (blu*hi)/100);
		*/
	}

	MCatchLoadImage(&srv_screen, &srv_screen_handle);
}

#if 0
void srv_InitJoystick() {
dd i;

	if (JoystickEnabled)	/* joystick support not disabled with -iJ */
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		if ((SDL_NumJoysticks()>0)&&(SDL_NumJoysticks()<17))
		{
			for (i=0; i<SDL_NumJoysticks(); i++)
			{
				JoystickTable[i]=SDL_JoystickOpen(i);
				/* Stelladaptor only has 2 buttons and 2 axes */
				if ((SDL_JoystickNumAxes(JoystickTable[i]) == 2) &&
					(SDL_JoystickNumButtons(JoystickTable[i]) == 2) &&
					(SDL_JoystickNumHats(JoystickTable[i]) == 0) &&
					(SDL_JoystickNumBalls(JoystickTable[i]) == 0) &&
					(StelladaptorEnabled))
				{
					Stelladaptor[i] = 1;
				}
				else Stelladaptor[i] = 0;
			}
//			SDL_JoystickEventState(SDL_ENABLE);
		}
		else JoystickEnabled = 0;	/* disable joystick polling */
	}

}
#endif

void srv_SetVideoMode() {

	printf("srv_SetVideoMode\n");
	/* If user gave a valid video mode, use it, or else use default */
	/*
	if (VideoMode > 5) VideoMode = 0;

	srv_CreateIcon();
	SDL_WM_SetIcon(z26Icon, z26IconMask);
	srv_InitJoystick();

	switch(VideoMode)
	{
	default:
	case 0:	 MaxLines = 256; width = 512;  height = 512;  srv_screen = small_screen; break;
	case 1:	 MaxLines = 256; width = 512;  height = 512;  srv_screen = small_screen; break;
	case 2:	 MaxLines = 256; width = 1024; height = 1024; srv_screen = large_screen; break;
	case 3:	 MaxLines = 256; width = 1024; height = 1024; srv_screen = large_screen; break;
	case 4:	 MaxLines = 256; width = 256;  height = 256;  srv_screen = tiny_screen;  break;
	case 5:	 MaxLines = 256; width = 256;  height = 256;  srv_screen = tiny_screen;  break;
	}
	*/

	/* fixed mode and geometry for spmp8k */
	VideoMode = 4;
	MaxLines = 240; width = 320;  height = 240;

	scanlinespread = height / MaxLines;

	pixelspread = width / 512;
	if (pixelspread == 0) pixelspread = 1;

	ClearScreenBuffers();
	CreateScreen();
	srv_SetPalette();

	//if(GrabInput) SDL_WM_GrabInput(SDL_GRAB_ON);

	position_game(); 	/* set game's vertical position */
}

mcatch_point2d_t pos = {
	.x = 0,
	.y = 0
};

mcatch_rect_t rect = {
	.x = 0,
	.y = 0,
	.width = SCREEN_WIDTH,
	.height = SCREEN_HEIGHT
};

/* spmp8k CopyScreen */
void srv_CopyScreen()
{
	MCatchBitblt(srv_screen_handle, &rect, &pos);

	MCatchFlush();
	MCatchPaint();
}

#if 0
void srv_CopyScreen()
{
 	void (*copy)();
	dd bpp = srv_screen->format->BytesPerPixel;
	dd i;
	dd oldpixelspread = pixelspread;

	odd++;		  // alternate startline for interlaced display

	if (tiawidth == 160) pixelspread *= 2;	// for the narrow tia engine

	Vert = (height - scanlinespread*MaxLines) / 2;
	Horiz = (width - pixelspread*tiawidth) / 2;
	if (width == 256) Horiz = (width - pixelspread*tiawidth/2) / 2;

	srv_buffer = srv_screen->pixels;
	srv_pitch = srv_screen->pitch;

	emu_pixels = ScreenBuffer;
	emu_pixels_prev = ScreenBufferPrev;
	prev_emu_pixels = PrevScreenBuffer;
	prev_emu_pixels_prev = PrevScreenBufferPrev;

	screen_pixels = srv_buffer + Horiz*bpp + (Vert)*srv_pitch;

	if (status_timer > 0)
		{
			show_transient_status();
			status_timer--;
		}
		else if (ShowLineCount && !GamePaused)
		{
			show_scanlines();
		}

	lines2draw = scanlinespread;

	copy = PixCopy32;
	if (scanlinespread == 1)
		copy = FastPixCopy32;
	else if (DoInterlace)
	{
		lines2draw = scanlinespread / 2;
		if (odd & 1) screen_pixels += lines2draw * srv_pitch;
	}
	else if (scanlinespread == 2)
		copy = PixCopy32_2;
	else if (scanlinespread == 4)
		copy = PixCopy32_4;

	for (i=0; i<MaxLines; ++i)
	{
		(*copy)();
		screen_pixels += scanlinespread * srv_pitch;
		emu_pixels += tiawidth;
		emu_pixels_prev += tiawidth;
		prev_emu_pixels += tiawidth;
		prev_emu_pixels_prev += tiawidth;
	}

	gl_DrawScreen();

	srv_Flip();

	pixelspread = oldpixelspread;
}
#endif

void gui_CopyScreen()
{
#if 0
	db oldDoInterlace = DoInterlace;
	dd oldtiawidth = tiawidth;

	DoInterlace = 0;
	tiawidth = 320;

	srv_CopyScreen();

	DoInterlace = oldDoInterlace;
	tiawidth = oldtiawidth;
#endif
}



/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.
	z26 is released subject to the terms and conditions of the
	GNU General Public License Version 2 (GPL).
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
