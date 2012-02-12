/*
** guisound.c --  menu
*/

#define SQMIN 2048

int sound_current = 0;	/* currently selected GUI option */
int exit_sound = 0;	/* exit video menu */

char quiet_data[52];
char dsp_data[52];
char sq_data[52];

void set_dsp_string() {
	if (dsp == 0) sprintf(dsp_data, "Off");
	else sprintf(dsp_data, "%d", dsp);
}

void hand_dsp_inc() {
	if (dsp == 2) dsp = 0;
	else ++dsp;
	set_dsp_string();
}

void hand_dsp_dec() {
	if (dsp == 0) dsp = 2;
	else --dsp;
	set_dsp_string();
}

void set_sq_string() {
	if (dsp == 0) sprintf(dsp_data, "Off");
	else sprintf(sq_data, "%d", SQ_Max);
}

void hand_sq_inc() {
	if (SQ_Max >= 65536) SQ_Max = SQMIN;
	else SQ_Max += 1024;
	set_sq_string();
}

void hand_sq_dec() {
	if (SQ_Max <= SQMIN) SQ_Max = 65536;
	else SQ_Max -= 1024;
	set_sq_string();
}

void hand_quiet() {
	quiet = !quiet;
	set_yesno_string(quiet_data, quiet);
}

void hand_sound_exit() {
	exit_sound = 1;
}


gui_entry sound_gui_items[] = {
	{ " Quiet..: %s ", quiet_data, 0, hand_quiet, hand_quiet },
	{ " DSP....: %s ", dsp_data, 0, hand_dsp_inc, hand_dsp_dec },
	{ " SQ Size: %s ", sq_data, 0, hand_sq_inc, hand_sq_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_sound_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void sound_gui() {
	SDL_Event ev;
	int action;
	
	set_yesno_string(quiet_data, quiet);
	set_dsp_string();
	set_sq_string();
	
	exit_sound = 0;
	while( !exit_sound ) {
		status_timer = 0;

		sprintf(msg, "Sound Options");
		draw_gui(sound_gui_items, sound_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		sound_current = gui_handle_action(action, &exit_sound, sound_gui_items, sound_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
