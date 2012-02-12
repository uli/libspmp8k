/*
** guidebug.c --  menu
*/

int debug_current = 0;	/* currently selected GUI option */
int exit_debug = 0;	/* exit timing menu */

char trace_count_data[52];
char trace_enabled_data[52];

void set_debug_string() {
	if (UserTraceCount == 0)	sprintf(trace_count_data, "Disabled");
	else if (UserTraceCount == 1)	sprintf(trace_count_data, "Enabled");
	else				sprintf(trace_count_data, "Delayed");
	;
}

void hand_trace_count_inc() {
	if (UserTraceCount == 1) UserTraceCount = 0xff;
	else ++UserTraceCount;
	if (UserTraceCount == 0xff) UserTraceEnabled = 0;
	else UserTraceEnabled = 1;
	set_debug_string();
}

void hand_trace_count_dec() {
	if (UserTraceCount == 0xff) UserTraceCount = 1;
	else --UserTraceCount;
	if (UserTraceCount == 0xff) UserTraceEnabled = 0;
	else UserTraceEnabled = 1;
	set_debug_string();
}

void hand_debug_exit() {
	
	exit_debug = 1;
}

gui_entry debug_gui_items[] = {
	{ " Trace Log: %s ", trace_count_data, 0, hand_trace_count_inc, hand_trace_count_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_debug_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void debug_gui() {
	SDL_Event ev;
	int action;
	
	set_debug_string();
	
	exit_debug = 0;
	while( !exit_debug ) {
		status_timer = 0;

		sprintf(msg, "Debug Options");
		draw_gui(debug_gui_items, debug_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		debug_current = gui_handle_action(action, &exit_debug, debug_gui_items, debug_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
