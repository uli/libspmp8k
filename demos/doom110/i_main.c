// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_main.c,v 1.4 1997/02/03 22:45:10 b1 Exp $";

#ifdef SPMP8
#include "libgame.h"
#endif

#include "doomdef.h"

#include "m_argv.h"
#include "d_main.h"

//#include "loading.h"

#include "rls_text.h"

int
main
( int		argc,
  char**	argv ) 
{ 
#ifndef SPMP8
    myargc = argc; 
    myargv = argv; 
 
    D_DoomMain (); 
#else
	uint32_t color;
	mcatch_rect_t rect;
	ge_key_data_t keys;
	
	myargc = 0;
    myargv = NULL;
	
	// initialize the game api
	libgame_init();
	MCatchInitGraph();

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	MCatchSetFrameBuffer(320, 240);
	MCatchSetDisplayScreen(&rect);
/*	
	// clear the background
	if (loading_img.data == NULL) {
		color = MAKE_RGB(255,0,255);
	}
	else if (loading_img.pal_data == NULL) {
		color = MAKE_RGB(0,0,255);
	}
	else color = MAKE_RGB(255,255,255);
*/	
	MCatchEnableFeature(3);
	MCatchSetFGColor(&color);
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFillRect(&rect);
/*
	// loading screen
	uint8_t loading_id;
	
	if (MCatchLoadImage(&loading_img, &loading_id) != 0) return;
	{
		mcatch_point2d_t at;
		mcatch_rect_t rect;
		
		MCatchSetColorROP(0xcc);
		
		at.x = 8;
		at.y = 8;
		rect.x = 8;
		rect.y = 8;
		rect.width=300;
		rect.height= 200;
		
	//	MCatchBitblt(loading_id, &rect, &at);
		MCatchSprite(loading_id, &rect, &at);
	}
*/
//#ifdef DEBUG
	// initialize the dmsg
	dmsg_init(0, 0, 320, 240);
//	dmsg_puts("Debug Console\n");
	dmsg_wait(0);
//#endif
	dmsg_puts(rls_text1);
	dmsg_puts(rls_text2);
/*
	MCatchFlush();
	MCatchPaint();
*/
	D_DoomMain();

#ifdef DEBUG
	dmsg_shutdown();
#endif
#endif
    return 0;
} 
