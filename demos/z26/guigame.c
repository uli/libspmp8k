/*
** guigame.c -- game menu
*/

int game_current = 0;	/* currently selected GUI option */
int exit_game = 0;	/* exit video menu */

char *controllers[] = {
	"Joystick  ",
	"Paddle    ",
	"Keypad    ",
	"Driving   ",
	"LightGun  ",
	"Compumate ",
	"KidVid    ",
	"MindLink  ",
	"ST Mouse  ",
	"TrakBall  ",
	"AmigaMouse",
	"None      "
};

char bank_data[52];
char palette_data[52];
char left_control_data[52];
char right_control_data[52];
char allowall4_data[52];
char swapports_data[52];
char p0diff_data[52];
char p1diff_data[52];
char consoleBW_data[52];


void set_p0diff_string() {
	switch(UserP0Diff) {
		case 0:
			sprintf(p0diff_data, "Easy");
			break;
		case 1:
			sprintf(p0diff_data, "Hard");
			break;
	}
}

void set_p1diff_string() {
	switch(UserP1Diff) {
		case 0:
			sprintf(p1diff_data, "Easy");
			break;
		case 1:
			sprintf(p1diff_data, "Hard");
			break;
	}
}

void set_consoleBW_string() {
	switch(UserConsoleBW) {
		case 0:
			sprintf(consoleBW_data, "Color");
			break;
		case 1:
			sprintf(consoleBW_data, "B/W");
			break;
	}
}

void set_swapports_string() {
	switch(UserSwapPortsFlag) {
		case 0:
			sprintf(swapports_data, "Off");
			break;
		case 1:
			sprintf(swapports_data, "On");
			break;
		case 0xff:
			sprintf(swapports_data, "Auto");
			break;
	}
}

void set_allowall4_string() {
	switch(UserAllowAll4) {
		case 0:
			sprintf(allowall4_data, "Off");
			break;
		case 1:
			sprintf(allowall4_data, "On");
			break;
		case 0xff:
			sprintf(allowall4_data, "Auto");
			break;
	}
}

void set_palette_string() {
	switch(UserPaletteNumber) {
		case 0:
			sprintf(palette_data, "NTSC");
			break;
		case 1:
			sprintf(palette_data, "PAL");
			break;
		case 2:
			sprintf(palette_data, "SECAM");
			break;
		case 0xff:
			sprintf(palette_data, "Auto");
			break;
	}
}

void set_bank_string() {
	switch (UserBankswitch)
	{
	case 0: sprintf(bank_data, "%d-2K+4K Atari [NoBS]", UserBankswitch);	break;
	case 1: sprintf(bank_data, "%d-CommaVid [CV]", UserBankswitch);	break;
	case 2: sprintf(bank_data, "%d-8K Superchip [F8SC]", UserBankswitch);	break;
	case 3:	sprintf(bank_data, "%d-8K Parker Bros. [E0]", UserBankswitch);	break;
	case 4: sprintf(bank_data, "%d-8K Tigervision [3F]", UserBankswitch);	break;
	case 5: sprintf(bank_data, "%d-8K Activision [FE] (flat)", UserBankswitch);	break;
	case 6: sprintf(bank_data, "%d-16K Superchip [F6SC]", UserBankswitch);	break;
	case 7: sprintf(bank_data, "%d-16K M-Network [E7]", UserBankswitch); 	break;
	case 8: sprintf(bank_data, "%d-32K Superchip [F4SC]", UserBankswitch);	break;
	case 9: sprintf(bank_data, "%d-8K Atari swapped [F8]", UserBankswitch);	break;
	case 10: sprintf(bank_data, "%d-Compumate [CM]", UserBankswitch);	break;
	case 11: sprintf(bank_data, "%d-512K Tigervision+ [3F+]", UserBankswitch);	break;
	case 12: sprintf(bank_data, "%d-8K United Appliance [UA]", UserBankswitch);	break;
	case 13: sprintf(bank_data, "%d-64K HS Runner [EF]", UserBankswitch);	break;
	case 14: sprintf(bank_data, "%d-512K Krokodile Cart [3E]", UserBankswitch);	break;
	case 15: sprintf(bank_data, "%d-Starpath [AR]", UserBankswitch);	break;
	case 16: sprintf(bank_data, "%d-16K Atari [F6]", UserBankswitch);	break;
	case 17: sprintf(bank_data, "%d-32K Atari [F4]", UserBankswitch); 	break;
	case 18: sprintf(bank_data, "%d-64K Megaboy [MB]", UserBankswitch);	break;
	case 19: sprintf(bank_data, "%d-12K CBS [FA]", UserBankswitch);	break;
	case 20: sprintf(bank_data, "%d-8K Atari [F8]", UserBankswitch);	break;
	case 21: sprintf(bank_data, "%d-8K+DPC Pitfall2 [P2]", UserBankswitch);	break;
	case 22: sprintf(bank_data, "%d-4A50 / supercat", UserBankswitch);	break;
	case 23: sprintf(bank_data, "%d-0840 EconoBanking", UserBankswitch);	break;
	case 0xff: sprintf(bank_data, "Auto");	break;
	default: sprintf(bank_data, "%d-unknown", UserBankswitch);	break;
	}
}

void hand_palette_inc() {
	if      (UserPaletteNumber == 0xff) UserPaletteNumber = 0;
	else if (UserPaletteNumber == 2) UserPaletteNumber = 0xff;
	else    UserPaletteNumber++;
	
	if (UserPaletteNumber == 0xff) PaletteNumber = 0;
	else PaletteNumber = UserPaletteNumber;
	
	gui_SetVideoMode();
	set_palette_string();
}

void hand_palette_dec() {
	if      (UserPaletteNumber == 0xff) UserPaletteNumber = 2;
	else if (UserPaletteNumber == 0) UserPaletteNumber = 0xff;
	else    UserPaletteNumber--;
	
	if (UserPaletteNumber == 0xff) PaletteNumber = 0;
	else PaletteNumber = UserPaletteNumber;

	gui_SetVideoMode();
	set_palette_string();
}

void hand_bank_inc() {
	if (UserBankswitch == 0xff) UserBankswitch = 0;
	else if (UserBankswitch == 23) UserBankswitch = 0xff;
	else ++UserBankswitch;
	set_bank_string();
}

void hand_bank_dec() {
	if (UserBankswitch == 0) UserBankswitch = 0xff;
	else if (UserBankswitch == 0xff) UserBankswitch = 23;
	else --UserBankswitch;
	set_bank_string();
}

void hand_allowall4_inc() {
	if (UserAllowAll4 == 0xff) UserAllowAll4 = 0;
	else if (UserAllowAll4 == 1) UserAllowAll4 = 0xff;
	else ++UserAllowAll4;
	set_allowall4_string();
}

void hand_allowall4_dec() {
	if (UserAllowAll4 == 0xff) UserAllowAll4 = 1;
	else if (UserAllowAll4 == 0) UserAllowAll4 = 0xff;
	else --UserAllowAll4;
	set_allowall4_string();
}

void hand_swapports_inc() {
	if (UserSwapPortsFlag == 0xff) UserSwapPortsFlag = 0;
	else if (UserSwapPortsFlag == 1) UserSwapPortsFlag = 0xff;
	else ++UserSwapPortsFlag;
	set_swapports_string();
}

void hand_swapports_dec() {
	if (UserSwapPortsFlag == 0xff) UserSwapPortsFlag = 1;
	else if (UserSwapPortsFlag == 0) UserSwapPortsFlag = 0xff;
	else --UserSwapPortsFlag;
	set_swapports_string();
}

void hand_p0diff() {
	UserP0Diff = !UserP0Diff;
	set_p0diff_string();
}

void hand_p1diff() {
	UserP1Diff = !UserP1Diff;
	set_p1diff_string();
}

void hand_consoleBW() {
	UserConsoleBW = !UserConsoleBW;
	set_consoleBW_string();
}

int dec_ctrl(int c) {
	if(c == 0xff)
		c = 11;
	else if(c == 0)
		c = 0xff;
	else
		c--;

	return c;
}

int inc_ctrl(int c) {
	if(c == 0xff)
		c = 0;
	else
		c++;

	if(c == 12) c = 0xff;

	return c;
}

void set_ctrlr_string(char *buf, int c) {
	if(c == 0xff)
		sprintf(buf, "Auto");
	else
		sprintf(buf, "%s", controllers[c]);
}

void hand_lctrl_dec() {
	UserLeftController = dec_ctrl(UserLeftController);
	set_ctrlr_string(left_control_data, UserLeftController);
}

void hand_lctrl_inc() {
	UserLeftController = inc_ctrl(UserLeftController);
	set_ctrlr_string(left_control_data, UserLeftController);
}

void hand_rctrl_dec() {
	UserRightController = dec_ctrl(UserRightController);
	set_ctrlr_string(right_control_data, UserRightController);
}

void hand_rctrl_inc() {
	UserRightController = inc_ctrl(UserRightController);
	set_ctrlr_string(right_control_data, UserRightController);
}


// write out the crc

void hand_crc()
{
	if (file_list[curfile])
	{
		cli_calc_CRC(file_list[curfile]);
		cli_write_CRC(file_list[curfile]);
		clrscr();
		sprintf(msg, "%s", "CRC written to z26.crc" );
		draw_long_msg_at_color(0, 4, 86, 80);
		gui_CopyScreen();
		SDL_Delay(1000);
//		draw_gui(current_gui_items, gui_current);
		gui_CopyScreen();
	}
	else
	{
		
		clrscr();
		sprintf(msg, "%s", "No file selected" );
		draw_long_msg_at_color(0, 4, 86, 80);
		gui_CopyScreen();
		SDL_Delay(1000);
//		draw_gui(current_gui_items, gui_current);
		gui_CopyScreen();
	}
}

void hand_game_exit() {
	exit_game = 1;
}

gui_entry game_gui_items[] = {
	{ " P0 Difficulty...: %s ", p0diff_data, 0, hand_p0diff, hand_p0diff },
	{ " P1 Difficulty...: %s ", p1diff_data, 0, hand_p1diff, hand_p1diff },
	{ " Console B/W.....: %s ", consoleBW_data, 0, hand_consoleBW, hand_consoleBW },
	{ " ", NULL, 0, NULL, NULL },
	{ " Bankswitch......: %s", bank_data, 0, hand_bank_inc, hand_bank_dec },
	{ " Palette.........: %s ", palette_data, 0, hand_palette_inc, hand_palette_dec },
	{ " Left Controller.: %s", left_control_data, 0, hand_lctrl_inc, hand_lctrl_dec },
	{ " Right Controller: %s", right_control_data, 0, hand_rctrl_inc, hand_rctrl_dec },
	{ " Swap Ports......: %s ", swapports_data, 0, hand_swapports_inc, hand_swapports_dec },
	{ " Allow All 4 Dir.: %s ", allowall4_data, 0, hand_allowall4_inc, hand_allowall4_dec },
//	{ " Write CRC ", NULL, 0, hand_crc, NULL },
	{ " ", NULL, 0, NULL, NULL },
	{ " Exit ", NULL, 0, hand_game_exit, NULL },
	{ NULL, NULL, 0, NULL, NULL } // last element must always be this
};


/* Main entry point. Called by gui() */

void game_gui() {
	SDL_Event ev;
	int action;
	
	set_bank_string();
	set_ctrlr_string(left_control_data, UserLeftController);
	set_ctrlr_string(right_control_data, UserRightController);
	set_palette_string();
	set_allowall4_string();
	set_swapports_string();
	set_p0diff_string();
	set_p1diff_string();
	set_consoleBW_string();
	
	exit_game = 0;
	while( !exit_game ) {
		status_timer = 0;

		if (!cli_calc_CRC(file_list[curfile]))
			sprintf(msg, "Game Options: couldn't get CRC");
		else
			sprintf(msg, "Game Options:  %d  0x%x", CartSize, crc);
			
		draw_gui(game_gui_items, game_current);
		
		SDL_WaitEvent(&ev);	/* TODO: respond to SDL_QUIT events */
		action = gui_navigation(&ev);
		if(action == GUI_NO_ACTION) continue;
		
		game_current = gui_handle_action(action, &exit_game, game_gui_items, game_current);
	}
}



/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
