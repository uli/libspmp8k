/*
** sdlmouse.c -- SDL mouse code
*/

/*
** mouse stuff
*/

db srv_mouse_button;
int srv_micky_x, srv_micky_y;
int srv_mouse_x, srv_mouse_y;

db srv_crap_button;
int srv_crap_x, srv_crap_y;

/*
void srv_get_mouse_button_status()
{
	srv_mouse_button = SDL_GetMouseState(&srv_mouse_x, &srv_mouse_y);
}
*/

void srv_get_mouse_movement()
{
	srv_mouse_button = SDL_GetRelativeMouseState(&srv_micky_x, &srv_micky_y);
	if (MouseRude)
	{
		SDL_WarpMouse(width/2,height/2);
		srv_Events();
		srv_crap_button = SDL_GetRelativeMouseState(&srv_crap_x, &srv_crap_y);
	}
}

/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the 
** GNU General Public License Version 2 (GPL).	z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
