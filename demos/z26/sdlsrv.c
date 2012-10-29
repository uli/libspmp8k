/*
** sdlsrv.c -- SDL service code
*/

/*
SDL_Joystick *JoystickTable[16];

SDL_Surface *srv_screen = NULL;
SDL_Surface *small_screen = NULL;
SDL_Surface *large_screen = NULL;
SDL_Surface *tiny_screen = NULL;
*/

db *srv_buffer;
dd srv_pitch;

dd Vert, Horiz;		/* offsets into window or video display where 2600 pixels are rendered in pixels*/
dd width, height;	/* dimensions of current window or video display in pixels */

db *screen_pixels;	  /* pointer to display screen */
db *emu_pixels;		 /* pointer to current emulator buffer */
db *emu_pixels_prev;	/* pointer to previous emulator buffer */
db *prev_emu_pixels;	/* previous pointer to display screen */
db *prev_emu_pixels_prev; /* previous pointer to previous emulator buffer */

db screen_buffer_count = 0;
db srv_done = 0;
dd odd = 0;		/* is the frame number odd? -- for interlaced modes */

#define PALETTE_SIZE	128

dw srv_colortab_hi[PALETTE_SIZE];		/* for mapping atari 8-bit colors to 32-bit colors */
dd srv_colortab_med[128];		/* for soft scanlines */
dd srv_colortab_low[128];

//#include "sdlicon.c"
//#include "sdlopengl.c"
//#include "sdlflip.c"
#include "sdlvideo.c"
//#include "sdlsound.c"
//#include "sdlmouse.c"

void Init_SDL()
{
	uint32_t color = 0;
	gfx_rect_t rect;

	printf("Init_SDL()\n");

	MCatchInitGraph(NULL, 0);

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	MCatchSetFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	MCatchSetDisplayScreen(&rect);

	MCatchEnableFeature(3);
	MCatchSetColorROP(COLOR_ROP_NOP);
	//MCatchSetFGColor(&color);
	//MCatchFillRect(&rect);

	//memset(srv_screen_fb, '\28', sizeof(srv_screen_fb));

	srv_screen.data		= RealScreenBuffer1; //srv_screen_fb;
	srv_screen.width	= SCREEN_WIDTH;
	srv_screen.height	= SCREEN_HEIGHT;
	srv_screen.img_type	= IMG_TYPE_8BPP;
	srv_screen.unk2		= 0;
	srv_screen.pal_data	= srv_colortab_hi;
	srv_screen.pal_size	= PALETTE_SIZE;
	srv_screen.unk3		= 0xffffff80;

	MCatchLoadImage(&srv_screen, &srv_screen_handle);
	MCatchSetColorROP(0xcc);
	/*
	Init_SoundQ();

	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0 )
	{
		printf("Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}

	screen_info = SDL_GetVideoInfo();
	screen_bpp = screen_info->vfmt->BitsPerPixel;

	gl_InitOpenGL();

	small_screen = SDL_CreateRGBSurfaceFrom(&texture_buffer, 512, 512, 32, 4*512, 0, 0, 0, 0);
	large_screen = SDL_CreateRGBSurfaceFrom(&texture_buffer, 1024, 1024, 32, 4*1024, 0, 0, 0, 0);
	tiny_screen =  SDL_CreateRGBSurfaceFrom(&texture_buffer, 256, 256, 32, 4*256, 0, 0, 0, 0);

	srv_screen = small_screen;

	screen_buffer_count = 0;
	*/
	ScreenBuffer = RealScreenBuffer1;
	//ScreenBufferPrev = RealScreenBuffer2;
	//	PrevScreenBuffer = RealScreenBuffer3;
	//	PrevScreenBufferPrev = RealScreenBuffer4;

	ClearScreenBuffers();
	printf("Init_SDL() exit\n");
}


/*
** event handler
*/

void srv_Events()
{
#if 0
	dd sc;
	SDL_Event event;

	if (JoystickEnabled) SDL_JoystickUpdate();	/* poll joysticks once per frame */

	while ( SDL_PollEvent(&event) ) {
		switch (event.type) {

			case SDL_KEYDOWN:
				sc = event.key.keysym.sym;
				KeyTable[sc] = 0x80;
				break;

			case SDL_KEYUP:
				sc = event.key.keysym.sym;
				KeyTable[sc] = 0;
				break;

			case SDL_QUIT:
				srv_done = 1;
				break;

			case SDL_JOYBUTTONDOWN:
				if (event.jbutton.which < 17)
					JoystickButton[event.jbutton.which][event.jbutton.button] = 0x80;
				break;

			case SDL_JOYBUTTONUP:
				if (event.jbutton.which < 17)
					JoystickButton[event.jbutton.which][event.jbutton.button] = 0;
				break;

			case SDL_JOYAXISMOTION:
				if (event.jaxis.which < 17)
					JoystickAxis[event.jaxis.which][event.jaxis.axis] = event.jaxis.value;
				break;
			default:
				break;
		}
	}
#endif

	NativeGE_getKeyInput4Ntv(&keys);
	/*
	KeyTable[P1Up]	  = (keys.key2 & KEY_UP) ? 0x80 : 0;
	KeyTable[P1Down]	= (keys.key2 & KEY_DOWN) ? 0x80 : 0;
	KeyTable[P1Left]	= (keys.key2 & KEY_LEFT) ? 0x80 : 0;
	KeyTable[P1Right]   = (keys.key2 & KEY_RIGHT) ? 0x80 : 0;
	KeyTable[P1Fire]	= (keys.key2 & KEY_X) ? 0x80 : 0;
	KeyTable[ResetKey]  = (keys.key2 & KEY_START) ? 0x80 : 0;
	*/
	//printf("key1 = %X | key2 = %X\n", keys.key1, keys.key2);
}


void Init_Service() {
	srv_SetVideoMode();
	//srv_sound_on();
}


void srv_Cleanup() {
	//MCatchFreeImage(srv_screen_handle);
	kv_CloseSampleFile();
	//srv_sound_off();
}

/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).	z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
