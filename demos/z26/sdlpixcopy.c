/*
	sdlpixcopy.c -- pixel copy routines
*/

// general purpose routine for interlacing

void PixCopy32()
{
	dd p, i, j, l;
	dd pixel;
	db *source = emu_pixels;
	db *prev = emu_pixels_prev;
	dd *dest = (dd*) screen_pixels;
	dd *ptr = (dd*) screen_pixels;
	dd ptr_inc = srv_pitch/4 - pixelspread;

	i = tiawidth/4;
	while (i--)
	{
		if (* (dd*) source != * (dd*) prev)
		{
			p=4;
			while (p--)
			{
				j = pixelspread; ptr = dest; l = lines2draw;
				pixel = srv_colortab_hi[*source++];
				while (l--) 
				{ 
					while (j--) { *ptr++ = pixel; }; j = pixelspread; ptr += ptr_inc; 
				};
				dest += pixelspread;
			}
		}
		else
		{
			source += 4; dest += pixelspread*4;
		}
		prev += 4;
	}
}


// for fast textures -- display every other TIA pixel (160 pixels)

void FastPixCopy32()
{
	int i;
	db *source = emu_pixels;
	db *prev = emu_pixels_prev;
	dd *dest = (dd*) screen_pixels;

	i = tiawidth/4;
	while (i--)
	{
		if (* (dd*) source != * (dd*) prev)
		{
			*dest++ = srv_colortab_hi[*source++]; ++source;
			*dest++ = srv_colortab_hi[*source++]; ++source;
		}
		else
		{
			source += 4; dest += 2;
		}
		prev += 4;
	}
}


// for normal textures

void PixCopy32_2()
{
	dd p, i;
	dd pixel;
	db *source = emu_pixels;
	db *prev = emu_pixels_prev;
	dd *dest = (dd*) screen_pixels;
	dd ptr_inc = srv_pitch/4;

	i = tiawidth/4;
	while (i--)
	{
		if (* (dd*) source != * (dd*) prev)
		{
			p=4;
			while (p--)
			{
				pixel = srv_colortab_hi[*source];
				*dest = pixel;
				if (DoScanline) pixel = srv_colortab_low[*source];
				*(dest++ + ptr_inc) = pixel;
				++source;
			}
		}
		else
		{
			source += 4; dest += 4;
		}
		prev += 4;
	}
}


// for HD textures

void PixCopy32_4()
{
	dd p, i;
	dd pixel;
	db *source = emu_pixels;
	db *prev = emu_pixels_prev;
	dd *dest = (dd*) screen_pixels;
	dd *ptr = (dd*) screen_pixels;
	dd ptr_inc = srv_pitch/4 - 1;

	i = tiawidth/4;
	while (i--)
	{
		if (* (dd*) source != * (dd*) prev)
		{
			p=4;
			while (p--)
			{
				ptr = dest;
				pixel = srv_colortab_hi[*source];
				if (DoScanline) pixel = srv_colortab_med[*source];
				*ptr++ = pixel; *ptr = pixel; ptr += ptr_inc; 
				if (DoScanline) pixel = srv_colortab_hi[*source];
				*ptr++ = pixel; *ptr = pixel; ptr += ptr_inc; 
				if (DoScanline) pixel = srv_colortab_med[*source];
				*ptr++ = pixel; *ptr = pixel; ptr += ptr_inc; 
				if (DoScanline) pixel = srv_colortab_low[*source];
				*ptr++ = pixel; *ptr = pixel;
				++source;
				dest += 2;
			}
		}
		else
		{
			source += 4; dest += 8;
		}
		prev += 4;
	}
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the 
	GNU General Public License Version 2 (GPL).	
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
