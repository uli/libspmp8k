/*
** sdlicon.c -- SDL service code
*/

/*
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the 
** GNU General Public License Version 2 (GPL).	z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/


SDL_Surface *z26Icon;
SDL_Color z26IconColours[4];
db z26IconShape[1024]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,3,3,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,
        4,4,3,3,3,3,3,3,3,3,3,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,
        4,4,3,3,3,3,3,3,3,3,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,
        4,4,4,4,4,4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,
        4,4,4,4,4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,
        4,4,4,4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,
        4,4,4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,4,
        4,4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,4,4,
        4,4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,4,4,4,
        4,4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,4,4,4,4,
        4,4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,3,3,4,4,4,4,4,4,4,4,4,4,
        4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,
        0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,
        0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,3,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,
        4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};

db z26IconMask[128]={
        0xff,0xff,0xff,0xfc,
        0xff,0xff,0xff,0xfc,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0x3f,0xff,0xff,0xfe,
        0x3f,0xff,0xff,0xfc,
        0x00,0x7f,0xff,0xf8,
        0x00,0xff,0xff,0xf0,
        0x01,0xff,0xff,0xe0,
        0x03,0xff,0xff,0xc0,
        0x07,0xff,0xff,0x80,
        0x0f,0xff,0xff,0x00,
        0x1f,0xff,0xfe,0x00,
        0x3f,0xff,0xfc,0x00,
        0x7f,0xff,0xff,0xfc,
        0xff,0xff,0xff,0xfc,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,
        0x3f,0xff,0xff,0xff,
        0x3f,0xff,0xff,0xff,
};

void srv_CreateIcon()
{
/*
        z26IconColours[0].r=96;         //turquise
        z26IconColours[0].g=192;
        z26IconColours[0].b=192;
        z26IconColours[1].r=0;          //blue
        z26IconColours[1].g=96;         
        z26IconColours[1].b=192;
*/
        z26IconColours[0].r=00;         //turquise
        z26IconColours[0].g=255;
        z26IconColours[0].b=255;
        z26IconColours[1].r=0;          //blue
        z26IconColours[1].g=0;         
        z26IconColours[1].b=255;
        z26IconColours[2].r=0;          //black
        z26IconColours[2].g=0;
        z26IconColours[2].b=0;
        z26IconColours[2].r=255;        //yellow (gets masked out)
        z26IconColours[2].g=255;
        z26IconColours[2].b=0;

        z26Icon = SDL_CreateRGBSurfaceFrom
                (z26IconShape, 32, 32, 8, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        SDL_SetColors(z26Icon, z26IconColours, 0, 4);
//        SDL_SetColorKey(z26Icon, SDL_SRCCOLORKEY, 3);
}
