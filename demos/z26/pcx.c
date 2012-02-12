/*
** pcx.c -- write a screenshot, in .bmp format
*/

void SaveScreenshot() {
	char def_screenshot[20] = "z26_";
	char PCXFileName[20];
	struct stat st;
	static int PCXFileCounter=0;
	int got;

	do	// Look for first available filename to use.
	{
		sprintf(PCXFileName, "%s%04i.%s", def_screenshot, PCXFileCounter++, "bmp");
		got = stat(PCXFileName, &st);
	} 
	while (got == 0);

	SDL_SaveBMP(srv_screen, PCXFileName);
}

/**
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the
** GNU General Public License Version 2 (GPL).  z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/
