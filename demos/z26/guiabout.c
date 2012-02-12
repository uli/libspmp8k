/*
** guiabout.c --  menu
*/

int about_current = 0;	/* currently selected GUI option */
int exit_about = 0;	/* exit about menu */


void hand_about_exit() {

	exit_about = 1;
}


gui_entry about_gui_items[] = {
	{ " Exit ", NULL, 0, hand_about_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void about_gui() {
	SDL_Event ev;
	int action;

	exit_about = 0;
	while( !exit_about ) {
		status_timer = 0;

		sprintf(msg, "Version 3.02.01 -- http://www.whimsey.com/z26");
		draw_gui(about_gui_items, about_current);

		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;

		about_current = gui_handle_action(action, &exit_about, about_gui_items, about_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
