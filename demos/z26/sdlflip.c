/*
** sdlflip.c -- SDL page flipping code
*/

int		SDLticks = 0;
double	Ticks = 0.0;
double	FirstFlipTime = 0.0;

int	Flips = 0;
int	FPSflips = 0;
double	CurrentFPS = 0.0;
double	FPStime = 0.0;

double srv_get_microseconds()
{
	double ticks;
	
	SDLticks = SDL_GetTicks();
	ticks = (double) SDLticks;
	ticks *= 1000.0;
	return(ticks);
}

void srv_sleep_microseconds(double wait_time)
{
	SDL_Delay((dd) wait_time/1000.0);
}

void srv_reset_timing()
{
	Flips = 0;
	Ticks = 0.0;			/* reset timing calculation */
	FirstFlipTime = 0.0;
	CurrentFPS = 0.0;
	FPStime = 0.0;
	FPSflips = 0;
}

void srv_Flip()
{
	double Now = srv_get_microseconds();

	if (Ticks == 0.0)	Ticks = Now;
	if (Flips++ == 0)	FirstFlipTime = Now;
	if (FPStime == 0.0)	FPStime = Now;

	++FPSflips;

	if (Now - FPStime > 4000000.0)	/* update FPS every 4 seconds or ... */
	{	
		CurrentFPS = (FPSflips * 1000000.0)/(Now - FPStime);
		FPStime = Now;
		FPSflips = 0;
	}
	else if (Now - FPStime <= 0.0)	/* ... if there was a time quake */
	{
		CurrentFPS = 0.0;
		FPStime = Now;
		FPSflips = 0;
	}
	
	SDL_GL_SwapBuffers();
	screen_buffer_count = (screen_buffer_count + 1) & 0x03;
	
	if (DoInterlace)
	{
		switch(screen_buffer_count)
		{
			case 0:
				ScreenBuffer = RealScreenBuffer1;
				ScreenBufferPrev = RealScreenBuffer2;
			break;
			case 1:
				ScreenBuffer = RealScreenBuffer3;
				ScreenBufferPrev = RealScreenBuffer4;
			break;
			case 2:
				ScreenBuffer = RealScreenBuffer2;
				ScreenBufferPrev = RealScreenBuffer1;
			break;
			case 3:
				ScreenBuffer = RealScreenBuffer4;
				ScreenBufferPrev = RealScreenBuffer3;
			break;
		}
	}
	else
	{
		switch(screen_buffer_count)
		{
			case 0:
				ScreenBuffer = RealScreenBuffer1;
				ScreenBufferPrev = RealScreenBuffer4;
				PrevScreenBuffer = RealScreenBuffer3;
				PrevScreenBufferPrev = RealScreenBuffer2;
			break;
			case 1:
				ScreenBuffer = RealScreenBuffer2;
				ScreenBufferPrev = RealScreenBuffer1;
				PrevScreenBuffer = RealScreenBuffer4;
				PrevScreenBufferPrev = RealScreenBuffer3;
			break;
			case 2:
				ScreenBuffer = RealScreenBuffer3;
				ScreenBufferPrev = RealScreenBuffer2;
				PrevScreenBuffer = RealScreenBuffer1;
				PrevScreenBufferPrev = RealScreenBuffer4;
			break;
			case 3:
				ScreenBuffer = RealScreenBuffer4;
				ScreenBufferPrev = RealScreenBuffer3;
				PrevScreenBuffer = RealScreenBuffer2;
				PrevScreenBufferPrev = RealScreenBuffer1;
			break;
		}
	}
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).	
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
