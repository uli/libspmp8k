/*
** guiutil.c --gui utility functions
*/


/* gui_action is an abstraction layer between SDL_Event and gui code.
	This allows both gui() and file_selector() (and later on whatever
	else we have) to use the same set of key/joystick bindings. See
	gui_navigate(). If we ever allow gui key remapping, gui_navigate()
	is where the map will be applied. */
	
typedef enum {
	GUI_NO_ACTION,
	GUI_UP,
	GUI_DOWN,
	GUI_PAGEUP,
	GUI_PAGEDOWN,
	GUI_HOME,
	GUI_END,
	GUI_ACTIVATE_PRI,
	GUI_ACTIVATE_SEC,
	GUI_RANDOM,
	GUI_EXIT_GUI
} gui_action;

/* gui_entry is a struct that represents a GUI menu option.

	label is the text, which must be less than GUI_WIDTH chars. It may
	contain one %s, which will be replaced by the data (uses sprintf())

	data is NULL for a menu item with no options (like `Resume Emulator'),
	or else the string value of the currently-selected option (such as
	"Yes" or "No" for e.g. `Show FPS' option). If you like, you can think
	of an item with options as a drop-down or radio list, and an item
	with no options as a button.

	Handlers are callbacks that actually perform the menu functions.
	They are declared to return void and accept no parameters.

	Each gui_entry has one primary handler (or NULL if no handler is
	desired) which will get called whenever the user presses (Enter,
	space, rightarrow, joybutton0, or joystick-right) and optionally
	a secondary handler which will get called when the user presses
	leftarrow, joybutton1, or joystick-left. If no secondary handler
	is desired, set sec_handler to NULL.

	It is possible to define a gui_entry with a NULL primary handler and
	an actual callable sec_handler. Please don't do this, it will only
	make the GUI confusing and serves no useful purpose.

	handlers must never assume that they're the only ones that can change
	a value: there are still keystrokes that can change values while the
	emulator is running (e.g. alt-enter for fullscreen/window).
*/
typedef struct {
	char *label;
	char *data;
	int disabled;
	void (*handler)();
	void (*sec_handler)();
} gui_entry;


/* Utility function for use by any menu item that's a toggle */

void set_yesno_string(char *str, int yesno) {
	if(yesno)
		sprintf(str, "Yes");
	else
		sprintf(str, "No");
}


/* Convert an SDL event to a gui_action. Using this ensures that
	our various GUI screens will have the same key/joystick/etc.
	bindings. If we ever decided to e.g. allow GUI key remapping or
	support the mouse in the GUI, we'd only have to change this one
	routine. */
gui_action gui_navigation(SDL_Event *evp) {
	if(evp->type == SDL_QUIT) {
		return GUI_EXIT_GUI;
	}
	
	if(evp->type == SDL_JOYAXISMOTION) {
		/* allow any joystick to control UI, not just first one.
			Also, any axes will work, not just 0 and 1. */
		if(evp->jaxis.axis % 2 == 0) { // even numbered axes are left/right
			/**
				// this makes the GUI hard to control with the stick,
				// disabled.
			if(evp->jaxis.value < -1024)
				return GUI_ACTIVATE_SEC;
			else if(evp->jaxis.value > 1024)
				return GUI_ACTIVATE_PRI;
			else
			*/
				return GUI_NO_ACTION;
		} else { // odd numbered axes are up/down
			if(evp->jaxis.value < -1024)
				return GUI_UP;
			else if(evp->jaxis.value > 1024)
				return GUI_DOWN;
			else
				return GUI_NO_ACTION;
		}
	}

	if(evp->type == SDL_JOYBUTTONDOWN) {
		if(evp->jbutton.button % 3 == 0) {
			return GUI_ACTIVATE_PRI;
		} else if(evp->jbutton.button % 3 == 1) {
			return GUI_ACTIVATE_SEC;
		} else {
			return GUI_EXIT_GUI;
		}
	}

	if(evp->type == SDL_KEYDOWN) {
		switch(evp->key.keysym.sym) {
			case SDLK_RETURN:
			case SDLK_SPACE:
			case SDLK_RIGHT:
			case SDLK_KP6:
				return GUI_ACTIVATE_PRI;

			case SDLK_LEFT:
			case SDLK_KP4:
				return GUI_ACTIVATE_SEC;

			case SDLK_UP:
			case SDLK_KP8:
				return GUI_UP;

			case SDLK_TAB:
			case SDLK_DOWN:
			case SDLK_KP2:
				return GUI_DOWN;

			case SDLK_BACKSPACE:
			case SDLK_ESCAPE:
				return GUI_EXIT_GUI;

			case SDLK_PAGEUP:
			case SDLK_KP9:
				return GUI_PAGEUP;

			case SDLK_PAGEDOWN:
			case SDLK_KP3:
				return GUI_PAGEDOWN;

			case SDLK_HOME:
			case SDLK_KP7:
				return GUI_HOME;

			case SDLK_END:
			case SDLK_KP1:
				return GUI_END;
				
			case SDLK_BACKSLASH:
				return GUI_RANDOM;
				
			case SDLK_EQUALS:
				SaveScreenshot();
				return GUI_NO_ACTION;

			default:
				return GUI_NO_ACTION;
		}
	}
	return GUI_NO_ACTION;	/* all other events */
}

int set_gui_height(gui_entry *menu) {
	int height = 0;
	
	for(; menu->label != NULL; menu++)
		height++;
		
	return(height);	
}

int gui_handle_action(int action, int *ptr_exit_gui, gui_entry *current_gui_items, int gui_current)
{
	int gui_height;
	gui_entry g;
	
	gui_height = set_gui_height(current_gui_items);
	g = current_gui_items[gui_current];
	switch(action) {

		case GUI_EXIT_GUI:
			*ptr_exit_gui = 1;
			break;

		case GUI_UP:
			gui_current--;
			if(gui_current < 0) gui_current = gui_height - 1;
			g = current_gui_items[gui_current];
			if(g.handler == NULL) gui_current--;
			if(gui_current < 0) gui_current = gui_height - 1;
			break;

		case GUI_DOWN:
			gui_current++;
			if(gui_current == gui_height) gui_current = 0;
			g = current_gui_items[gui_current];
			if(g.handler == NULL) gui_current++;
			if(gui_current == gui_height) gui_current = 0;
			break;

		case GUI_PAGEUP:
		case GUI_HOME:
			gui_current = 0;
			break;

		case GUI_PAGEDOWN:
		case GUI_END:
			gui_current = gui_height - 1;
			break;

		case GUI_ACTIVATE_PRI:
			if(g.handler != NULL && !g.disabled)
				g.handler();
			break;

		case GUI_ACTIVATE_SEC:
			if(g.sec_handler != NULL && !g.disabled)
				g.sec_handler();
			break;

		default:
			break;
	}
	return(gui_current);
}

/* Draw the GUI items. Called by gui() and sub-menus. Highlights the currently-selected item. */

void draw_gui(gui_entry *entries, int cur) {
	int i, x, y;
	int gui_height;

	gui_height = set_gui_height(entries);

	clrscr();
	
	y = 4;
//	y = 2 + (MaxLines/FONT_HEIGHT - set_gui_height(entries))/2;
	x = (tiawidth/FONT_WIDTH - GUI_WIDTH)/2;

	draw_msg_at(1, 1);

	for(i=0; i<gui_height; i++) {
		if(entries[i].data == NULL)
			sprintf(msg, "%s", entries[i].label);
		else
			sprintf(msg, entries[i].label, entries[i].data);

		//	fprintf(stderr, "%d %d\n", x, y);
		if(i==cur && entries[i].disabled)
			grey_hilite_msg_at(x, y+i);
		else if(i==cur)
			hilite_msg_at(x, y+i);
		else if(entries[i].disabled)
			grey_msg_at(x, y+i);
		else
			draw_msg_at(x, y+i);
	}
	gui_CopyScreen();
}


/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
