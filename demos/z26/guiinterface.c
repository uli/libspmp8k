/*
** guiinterface.c --  menu
*/

int interface_current = 0;	/* currently selected GUI option */
int exit_interface = 0;		/* exit interface menu */

char theme_data[52];
char show_fps_data[52];
char enable_keyboard_data[52];
char enable_mouse_data[52];
char enable_joystick_data[52];
char enable_stelladaptor_data[52];
char mouse_rude_data[52];
char grab_input_data[52];

void set_theme_string() {
	switch(theme/16)
	{
	case 0: sprintf(theme_data, "%d-silver", theme/16);	break;
	case 1: sprintf(theme_data, "%d-bronze", theme/16);	break;
	case 2: sprintf(theme_data, "%d-rose", theme/16);	break;
	case 3: sprintf(theme_data, "%d-violet", theme/16);	break;
	case 4: sprintf(theme_data, "%d-blue", theme/16);	break;
	case 5: sprintf(theme_data, "%d-aqua", theme/16);	break;
	case 6: sprintf(theme_data, "%d-green", theme/16);	break;
	case 7: sprintf(theme_data, "%d-gold", theme/16);	break;
	default: sprintf(theme_data, "%d-unknown", theme/16);	break;
	}
}


/* Handlers. You may be more familiar with the term `callback': that's what these are. */


void hand_enable_keyboard() {
	KeyboardEnabled = !KeyboardEnabled;
	set_yesno_string(enable_keyboard_data, KeyboardEnabled);
}

void hand_enable_mouse() {
	MouseEnabled = !MouseEnabled;
	set_yesno_string(enable_mouse_data, MouseEnabled);
}

void hand_enable_joystick() {
	UserJoystickEnabled = !UserJoystickEnabled;
	set_yesno_string(enable_joystick_data, UserJoystickEnabled);
	JoystickEnabled = UserJoystickEnabled;
	gui_SetVideoMode();	/* because joystick detection happens when we set video mode */
}

void hand_enable_stelladaptor() {
	StelladaptorEnabled = !StelladaptorEnabled;
	set_yesno_string(enable_stelladaptor_data, StelladaptorEnabled);
}

void hand_mouse_rude() {
	MouseRude = !MouseRude;
	set_yesno_string(mouse_rude_data, MouseRude);
}

void hand_grab_input() {
	GrabInput = !GrabInput;
	set_yesno_string(grab_input_data, GrabInput);
}

void hand_show_fps() {
	ShowLineCount = !ShowLineCount;
	set_yesno_string(show_fps_data, ShowLineCount);
}

void hand_theme_inc() {
	if (theme == 0x70) theme = 0;
	else theme += 0x10;
	set_theme_string();
}

void hand_theme_dec() {
	if (theme == 0) theme = 0x70;
	else theme -= 0x10;
	set_theme_string();
}

void hand_interface_exit() {
	exit_interface = 1;
}

gui_entry interface_gui_items[] = {
	{ " Show FPS...........: %s ", show_fps_data, 0, hand_show_fps, hand_show_fps },
	{ " Theme..............: %s ", theme_data, 0, hand_theme_inc, hand_theme_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Mouse Rude.........: %s ", mouse_rude_data, 0, hand_mouse_rude, hand_mouse_rude },
	{ " Grab Input.........: %s ", grab_input_data, 0, hand_grab_input, hand_grab_input },
	{ " ", NULL, 0, NULL, NULL },
	{ " Enable Keyboard....: %s ", enable_keyboard_data, 0, hand_enable_keyboard, hand_enable_keyboard },
	{ " Enable Mouse.......: %s ", enable_mouse_data, 0, hand_enable_mouse, hand_enable_mouse },
	{ " Enable Joystick....: %s ", enable_joystick_data, 0, hand_enable_joystick, hand_enable_joystick },
	{ " Enable Stelladaptor: %s ", enable_stelladaptor_data, 0, hand_enable_stelladaptor, hand_enable_stelladaptor },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_interface_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void interface_gui() {
	SDL_Event ev;
	int action;
	
	set_yesno_string(enable_keyboard_data, KeyboardEnabled);
	set_yesno_string(enable_mouse_data, MouseEnabled);
	set_yesno_string(enable_joystick_data, UserJoystickEnabled);
	set_yesno_string(enable_stelladaptor_data, StelladaptorEnabled);
	set_yesno_string(show_fps_data, ShowLineCount);
	set_yesno_string(mouse_rude_data, MouseRude);
	set_yesno_string(grab_input_data, GrabInput);
	set_theme_string();
	
	exit_interface = 0;
	while( !exit_interface ) {
		status_timer = 0;

		sprintf(msg, "Interface Options");
		draw_gui(interface_gui_items, interface_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		interface_current = gui_handle_action(action, &exit_interface, interface_gui_items, interface_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
