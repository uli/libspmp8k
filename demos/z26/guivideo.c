/*
	guivideo.c -- video menu
*/

int video_current = 0;	/* currently selected GUI option */
int exit_video = 0;		/* exit video menu */

char startline_data[52];
char screen_data[52];
char line_data[52];
char narrow_data[52];
char Tall_data[52];
char vid_mode_data[52];
char Depth_data[52];
char colourloss_data[52];
char eight_bit_data[52];
char width_data[52];


void set_screen_string() {
	if (FullScreen)		sprintf(screen_data, "Full Screen");
	else				sprintf(screen_data, "Window");
}

void set_line_string() {
	if (DoInterlace) 		sprintf(line_data, "Interlaced");
	else if (DoScanline)	sprintf(line_data, "Scanline");
	else 					sprintf(line_data, "Normal");
}

void set_width_string() {
	sprintf(width_data, "%d", Narrow);
}

void set_height_string() {
	sprintf(Tall_data, "%d", Tall);
}

void set_videomode_string() {
	switch (VideoMode)
	{
	case 0:	 sprintf(vid_mode_data, "Sharp");		break;
	case 1:	 sprintf(vid_mode_data, "Blurry");		break;
	case 2:  sprintf(vid_mode_data, "HD Sharp");	break;
	case 3:  sprintf(vid_mode_data, "HD Blurry");	break;
	case 4:  sprintf(vid_mode_data, "Fast Sharp");	break;
	case 5:  sprintf(vid_mode_data, "Fast Blurry");	break;
	default: sprintf(vid_mode_data, "whoops");		break;
	}
}

void set_Depth_string() {
	sprintf(Depth_data, "%d", UserDepth);
}

void hand_screen_inc() {
	if (FullScreen) FullScreen = 0; else FullScreen = 1;

	sprintf(msg, " Restart z26 for change to take effect.");
	srv_print(msg);

	set_screen_string();
}

void hand_screen_dec() {
	if (FullScreen) FullScreen = 0; else FullScreen = 1;
	
	sprintf(msg, " Restart z26 for change to take effect.");
	srv_print(msg);

	set_screen_string();
}

void hand_line_inc() {
	if (DoInterlace)		{ DoInterlace = 0; DoScanline = 0; }	// interlace -> normal
	else if (DoScanline)	{ DoScanline = 0; DoInterlace = 1; }	// scanline -> interlace
	else					{ DoScanline = 1; }						// normal -> scanline
	set_line_string();
	gui_SetVideoMode();
}

void hand_line_dec() {
	if (DoInterlace)		{ DoInterlace = 0; DoScanline = 1; }	// interlace -> scanline
	else if (DoScanline)	{ DoScanline = 0; DoInterlace = 0; }	// scanline -> normal
	else 					{ DoInterlace = 1; }					// normal -> interlace
	set_line_string();
	gui_SetVideoMode();
}

void hand_narrow_inc() {
int	maxstretch = (screen_width * 3) / 40;

	if (++Narrow > maxstretch) --Narrow;
	
	set_width_string();
	gui_SetVideoMode();
}

void hand_narrow_dec() {
	if (Narrow == -200) return;
	else		 --Narrow;
	set_width_string();
	gui_SetVideoMode();
}

void hand_height_inc() {
int	vsize = (screen_height/MaxLines)*MaxLines;

	if ((vsize + ++Tall*4) > screen_height) --Tall;
	
	set_height_string();
	gui_SetVideoMode();
}

void hand_height_dec() {
	if (Tall == -200) return;
	else		 --Tall;
	set_height_string();
	gui_SetVideoMode();
}

void hand_colourloss() {
	SimColourLoss = !SimColourLoss;
	set_yesno_string(colourloss_data, SimColourLoss);
}

void hand_Depth_inc() {
	if (UserDepth >= 100) UserDepth = 0;
	else ++UserDepth;
	set_Depth_string();
	Depth = UserDepth;
	srv_SetPalette();
}

void hand_Depth_dec() {
	if (UserDepth == 0) UserDepth = 100;
	else --UserDepth;
	set_Depth_string();
	Depth = UserDepth;
	srv_SetPalette();
}

void hand_vid_inc()
{
	if (++VideoMode > 5) VideoMode = 0;
	gui_SetVideoMode();
	set_videomode_string();
}

void hand_vid_dec()
{
	if (VideoMode == 0) 
		VideoMode = 5;
	else
		VideoMode -= 1;

	gui_SetVideoMode();
	set_videomode_string();
}

void hand_video_exit() {
	exit_video = 1;
}

gui_entry video_gui_items[] = {
	{ " Display........: %s", screen_data, 0, hand_screen_inc, hand_screen_dec },
	{ " Texture........: %s", vid_mode_data, 0, hand_vid_inc, hand_vid_dec },
	{ " Width..........: %s ", width_data, 0, hand_narrow_inc, hand_narrow_dec },
	{ " Height.........: %s ", Tall_data, 0, hand_height_inc, hand_height_dec },
	{ " Line Mode......: %s", line_data, 0, hand_line_inc, hand_line_dec },
	{ " Scanline Depth.: %s ", Depth_data, 0, hand_Depth_inc, hand_Depth_dec },
	{ " Colour Loss....: %s ", colourloss_data, 0, hand_colourloss, hand_colourloss },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_video_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};

/* Main entry point. Called by gui() */

void video_gui() {
	SDL_Event ev;
	int action;
	
	set_yesno_string(narrow_data, Narrow);
	set_yesno_string(colourloss_data, SimColourLoss);
	set_screen_string();
	set_line_string();
	set_videomode_string();
	set_width_string();
	set_height_string();
	set_Depth_string();
	
	exit_video = 0;
	while( !exit_video ) {
		status_timer = 0;
		
		sprintf(msg, "Video Options");
		draw_gui(video_gui_items, video_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		video_current = gui_handle_action(action, &exit_video, video_gui_items, video_current);
	}
}

/**
	z26 is Copyright 1997-2011 by John Saeger and contributors.  
	z26 is released subject to the terms and conditions of the
	GNU General Public License Version 2 (GPL).  
	z26 comes with no warranty.
	Please see COPYING.TXT for details.
*/
