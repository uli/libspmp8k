/*
** guifilelist.c -- the gui file lister
*/

dd match=0;
SDLKey matched[100];

void directory_failed()
{
	clrscr();
	srv_print("directory failed");
	SDL_Quit();
	exit(1);
}

#define MAX_ENTS 20000
#define MAX_LEN 256
#define FT_DIRS 0
#define FT_FILES 1

static int filesread = 0;
static char *file_list[MAX_ENTS+1];
static char file_names[(MAX_ENTS+1)*(MAX_LEN+1)];

void get_list(int file_type) {
	DIR *dirp;
	struct stat sstat;
	struct dirent *dent;
	int i, len;

	i = filesread;
	if(file_type == FT_DIRS) {
		file_list[i] = file_names + i*(MAX_LEN+1);
		sprintf(file_list[i], "../"); /* hardcoded `parent directory' */
		i++;
	}
		
	dirp = opendir(".");
	if(dirp == NULL)
		directory_failed();

	while(1) {
		if (i >= MAX_ENTS-1) break;
		file_list[i] = file_names + i*(MAX_LEN+1);
		file_list[i][0] = '\0';
		dent = readdir(dirp);
		if(dent == NULL) break;

		if(strlen(dent->d_name) >= MAX_LEN-1) {
			//	fprintf(stderr, "too long: %s\n", dent->d_name);
			continue; // TODO: handle longer names!
		}

		if(dent->d_name[0] == '.') {
			//	fprintf(stderr, "start with dot: %s\n", dent->d_name);
			continue; // skip over dotfiles
		}

		if(stat(dent->d_name, &sstat) != 0) {
			//	fprintf(stderr, "can't stat: %s\n", dent->d_name);
			continue; // TODO: err message (Can't stat)
		}

		if(file_type == FT_DIRS) {
			if(!S_ISDIR(sstat.st_mode)) continue;
		} else {
			if(!S_ISREG(sstat.st_mode)) continue;

			/* only looks at *.bin, *.BIN, *.a26, *.A26 files */
			if(	strstr(dent->d_name, ".bin") == NULL &&
				strstr(dent->d_name, ".BIN") == NULL &&
				strstr(dent->d_name, ".a26") == NULL &&
				strstr(dent->d_name, ".A26") == NULL)
			{
				continue;
			}
		}

		strcpy(file_list[i], dent->d_name);		

		/* Add a trailing / for dirs */
		if(file_type == FT_DIRS) {
			len = strlen(dent->d_name);
			file_list[i][len] = '/';
			file_list[i][len+1] = '\0';
		}

		i++;
	}

	filesread = i;
	return;
}

void get_file_list() {
	filesread = 0;

	get_list(FT_DIRS);
	get_list(FT_FILES);
	gui_Qsort(file_list, 0, filesread-1);
}

int draw_file_list(int cur, int window_line, int ymax) 
{
	int i, line=1;
	int xmax;
	int oldtiawidth = tiawidth;
	xmax = tiawidth / FONT_WIDTH;

	status_timer = 0;

	/* adjust window_line so cur stays between window_line and (window_line + ymax) */
	if(cur >= (window_line + ymax) ) {
		window_line = cur - ymax + 1;
	} else if(cur < window_line) {
		window_line = cur;
		if(window_line < 0) window_line = 0;
	}
	
	clrscr();
	
	for(i=window_line; i<(window_line+ymax); i++) {
		int len;

		if(i==filesread) break;

		strcpy(msg, file_list[i]);
		len = strlen(msg);

		/* if name too long, truncate with ... at end */
		/* (or ../ for a directory) */
		if(len >= xmax) {
			if(msg[len-1]=='/')
				msg[xmax-1] = '/';
			else
				msg[xmax-1] = ':';

//			msg[xmax-3] = '.';
//			msg[xmax-2] = '.';
			msg[xmax]   = '\0';
		}

		if(i==cur) {
			hilite_msg_at(0, line);
			if (match) hilite_char_at(msg[match-1], match, line);
		}
		else
			draw_msg_at(0, line);

		line++;
	}
	
	gui_CopyScreen();
	
	tiawidth = oldtiawidth;
	return window_line;
}


int find_next_rom(int curfile, SDLKey k) {
	int i,m,p;
	int matchflag, dirflag;

	if( (k<'a' || k>'z') && (k<'0' || k>'9') )
	{
		match = 0;
		return curfile; // only letters/numbers
	}
	
	matched[match++] = tolower(k);
	
	for (i=0; i<filesread; i++)
	{
		if(file_list[i][0] == '\0') continue;
		
		dirflag = 0;
		for (p=0; p<64; p++)
			if (file_list[i][p] == '/') dirflag = 1;
		if (dirflag) continue;

		matchflag = 1;
		for (m=0; m<match; m++)
		{
			if (tolower(file_list[i][m]) != matched[m])
			{
				matchflag = 0;
				break;
			}
		}
		if (matchflag)
		{
			clrscr();
			return i;
		}
	}
	match=0;
	return curfile;
}

/* 
** File selector.
**
**	returns 1 if user picked a file, 0 if he aborted.
**
**	result is filled with the filename.
*/

int window_line = 0;
int curfile = 0;
int first_filelist = 1;

int file_selector(char *result) {
	SDL_Event ev;
	int picked = 0;
//	int max_width = tiawidth/FONT_WIDTH;
	int max_height = MaxLines/FONT_HEIGHT; //-1;
	int k = 0;

	if (PaletteNumber != UserPaletteNumber)
	{
		PaletteNumber = UserPaletteNumber;
		gui_SetVideoMode();
	}
	clrscr();

	/* filesread is a global that gets updated by get_list */
	if (first_filelist) 
	{
		get_file_list();
		first_filelist = 0;
	}

	while(!picked) {
		int action;

		window_line = draw_file_list(curfile, window_line, max_height-2);
		SDL_WaitEvent(&ev);

		action = gui_navigation( &ev );

//		if(action == GUI_NO_ACTION)
//			continue;

		switch(action) 
		{
		char type;

		case GUI_EXIT_GUI:
			clrscr();
			return 0; /* abort */

		case GUI_ACTIVATE_SEC:
			match = 0;
			curfile++;
			if(curfile == filesread) curfile = 0;
			clrscr();
			type = file_list[curfile][strlen(file_list[curfile])-1];
			if(type == '/')	/* if it's a dir, cd to it and get new file list. */
			{
				if (chdir(file_list[curfile]) == 0)
				{
					curfile = 0;
					get_file_list();
				}
			} 
			else		/* if it's a file, return it */
			{
				strcpy(result, file_list[curfile]);
				picked = 1;
			}
			break;
		
		
		case GUI_ACTIVATE_PRI:
			match = 0;
			clrscr();
			type = file_list[curfile][strlen(file_list[curfile])-1];
			if(type == '/')	/* if it's a dir, cd to it and get new file list. */
			{
				if (chdir(file_list[curfile]) == 0)
				{
					curfile = 0;
					get_file_list();
				}
			} 
			else		/* if it's a file, return it */
			{
				strcpy(result, file_list[curfile]);
				picked = 1;
			}
			break;

		case GUI_UP:
			match = 0;
			curfile--;
			if(curfile < 0) curfile = filesread - 1;
			break;

		case GUI_DOWN:
			match = 0;
			curfile++;
			if(curfile == filesread) curfile = 0;
			break;

		case GUI_PAGEDOWN:
			match = 0;
			if(filesread > max_height-1) {
				curfile += max_height-1;
				if(curfile >= filesread)
					curfile = 0;
				window_line = curfile;
				clrscr();
			}
			break;

		case GUI_PAGEUP:
			match = 0;
			if(filesread > max_height-1) {
				curfile -= max_height-1;
				if(curfile < 0)
					curfile = filesread-1;
				window_line = curfile;
				clrscr();
				break;
			}

		case GUI_HOME:
			match = 0;
			curfile = 0;
			window_line = 0;
			clrscr();
			break;

		case GUI_END:
			match = 0;
			curfile = filesread-1;
			window_line = curfile;
			clrscr();
			break;
			
		case GUI_RANDOM:
			match = 0;
			curfile = rand() % filesread;
			window_line = curfile;
			type = file_list[curfile][strlen(file_list[curfile])-1];
			
			while (type == '/')	/* if it's a dir, find another file */
			{
				curfile = rand() % filesread;
				window_line = curfile;
				type = file_list[curfile][strlen(file_list[curfile])-1];
			} 
			
			
			
			clrscr();
			break;


		default:
			if(ev.type == SDL_KEYDOWN)
			{
				k = ev.key.keysym.sym;
				curfile = window_line = find_next_rom(curfile, k);
			}
			break;
		}

		SDL_WM_SetCaption(file_list[curfile], file_list[curfile]);

	}
	
	return picked;
}

/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
