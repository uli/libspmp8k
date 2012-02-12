/*
** guitweaks.c --  menu
* 
* lightgun wrapping ?
*/

int tweaks_current = 0;	/* currently selected GUI option */
int exit_tweaks = 0;	/* exit timing menu */

char mpx_data[52];
char mpy_data[52];
char paddle_sensitivity_data[52];
char paddle_adjust_data[52];
char lightgun_cycles_data[52];
char lightgun_scanlines_data[52];

void set_mpx_string() {
	if (UserMouseBaseX == 0xff) sprintf(mpx_data, "Auto");
	else sprintf(mpx_data, "%d", UserMouseBaseX);
}

void hand_mpx_inc() {
	if (UserMouseBaseX == 3) UserMouseBaseX = 0xff;
	else if (UserMouseBaseX == 0xff) UserMouseBaseX = 0;
	else ++UserMouseBaseX;
	set_mpx_string();
}

void hand_mpx_dec() {
	if      (UserMouseBaseX == 0) UserMouseBaseX = 0xff;
	else if (UserMouseBaseX == 0xff) UserMouseBaseX = 3;
	else --UserMouseBaseX;
	set_mpx_string();
}

void set_mpy_string() {
	if (UserMouseBaseY == 0xff) sprintf(mpy_data, "Auto");
	else sprintf(mpy_data, "%d", UserMouseBaseY);
}

void hand_mpy_inc() {
	if (UserMouseBaseY == 3) UserMouseBaseY = 0xff;
	else if (UserMouseBaseY == 0xff) UserMouseBaseY = 0;
	else ++UserMouseBaseY;
	set_mpy_string();
}

void hand_mpy_dec() {
	if      (UserMouseBaseY == 0) UserMouseBaseY = 0xff;
	else if (UserMouseBaseY == 0xff) UserMouseBaseY = 3;
	else --UserMouseBaseY;
	set_mpy_string();
}

void set_paddle_sensitivity_string() {
	if (UserPaddleSensitivity == 0) sprintf(paddle_sensitivity_data, "Auto");
	else sprintf(paddle_sensitivity_data, "%d", UserPaddleSensitivity);
}

void hand_paddle_sensitivity_inc() {
	if (UserPaddleSensitivity == 15) UserPaddleSensitivity = 0;
	else ++UserPaddleSensitivity;
	set_paddle_sensitivity_string();
}

void hand_paddle_sensitivity_dec() {
	if      (UserPaddleSensitivity == 0) UserPaddleSensitivity = 15;
	else --UserPaddleSensitivity;
	set_paddle_sensitivity_string();
}

void set_paddle_adjust_string() {
	if (PaddleAdjust == 0) sprintf(paddle_adjust_data, "Auto");
	else sprintf(paddle_adjust_data, "%d", PaddleAdjust);
}

void hand_paddle_adjust_inc() {
	if (PaddleAdjust == 15) PaddleAdjust = 0;
	else ++PaddleAdjust;
	set_paddle_adjust_string();
}

void hand_paddle_adjust_dec() {
	if      (PaddleAdjust == 0) PaddleAdjust = 15;
	else --PaddleAdjust;
	set_paddle_adjust_string();
}

void set_lightgun_cycles_string() {
	if (UserLightgun == 0) sprintf(lightgun_cycles_data, "Auto");
	else sprintf(lightgun_cycles_data, "%d", UserLightgun);
}

void hand_lightgun_cycles_inc() {
	if (UserLightgun == 15) UserLightgun = -15;
	else ++UserLightgun;
	set_lightgun_cycles_string();
}

void hand_lightgun_cycles_dec() {
	if (UserLightgun == -15) UserLightgun = 15;
	else --UserLightgun;
	set_lightgun_cycles_string();
}

void set_lightgun_scanlines_string() {
	if (UserLGadjust == 5) sprintf(lightgun_scanlines_data, "Auto");
	else sprintf(lightgun_scanlines_data, "%d", UserLGadjust);
}

void hand_lightgun_scanlines_inc() {
	if (UserLGadjust == 15) UserLGadjust = -15;
	else ++UserLGadjust;
	set_lightgun_scanlines_string();
}

void hand_lightgun_scanlines_dec() {
	if      (UserLGadjust == -15) UserLGadjust = 15;
	else --UserLGadjust;
	set_lightgun_scanlines_string();
}

void hand_tweaks_exit() {

	exit_tweaks = 1;
}

gui_entry tweaks_gui_items[] = {
	{ " Mouse Paddle X....: %s ", mpx_data, 0, hand_mpx_inc, hand_mpx_dec },
	{ " Mouse Paddle Y....: %s ", mpy_data, 0, hand_mpy_inc, hand_mpy_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Paddle Sensitivity: %s ", paddle_sensitivity_data, 0, hand_paddle_sensitivity_inc, hand_paddle_sensitivity_dec },
	{ " Paddle Adjust.....: %s ", paddle_adjust_data, 0, hand_paddle_adjust_inc, hand_paddle_adjust_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Lightgun Cycles...: %s ", lightgun_cycles_data, 0, hand_lightgun_cycles_inc, hand_lightgun_cycles_dec },
	{ " Lightgun Scanlines: %s ", lightgun_scanlines_data, 0, hand_lightgun_scanlines_inc, hand_lightgun_scanlines_dec },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_tweaks_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void tweaks_gui() {
	SDL_Event ev;
	int action;
	
	set_mpx_string();
	set_mpy_string();
	set_paddle_sensitivity_string();
	set_paddle_adjust_string();
	set_lightgun_cycles_string();
	set_lightgun_scanlines_string();
	
	exit_tweaks = 0;
	while( !exit_tweaks ) {
		status_timer = 0;

		sprintf(msg, "Controller Tweaks");
		draw_gui(tweaks_gui_items, tweaks_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		tweaks_current = gui_handle_action(action, &exit_tweaks, tweaks_gui_items, tweaks_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
