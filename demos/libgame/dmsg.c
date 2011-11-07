/*
** dmsg.c
**
** basic debug console - where the special printfs go
**
**
**
*/

#include <stdlib.h>
#include <stdarg.h>
#include "libgame.h"

// the tiny font
extern uint8_t font6x8_bits[];

// actually a 4 bit buffer would also suffice
int dmsg_pos_x, dmsg_pos_y;
int dmsg_buff_width, dmsg_buff_height;
int wait_scroll;
int do_scroll;

#define STATIC_BUFF

#ifdef STATIC_BUFF
uint8_t dmsg_buff[320*240];
#else
uint8_t *dmsg_buff = NULL;
#endif
uint16_t dmsg_buff_pal[256];
int dmsg_cursor_x, dmsg_cursor_y;

gfx_loadimg_t dmsg_buff_img;
uint8_t dmsg_buff_id;

void dmsg_clear(void)
{
	memset(dmsg_buff, 0, dmsg_buff_width * dmsg_buff_height);
	dmsg_cursor_x = dmsg_cursor_y = 0;
}

void dmsg_wait(int enable)
{
	if (enable) wait_scroll = 1;
	else wait_scroll = 0;
}

int dmsg_init(int x, int y, int width, int height)
{
	// init some vars
	dmsg_pos_x       = x;
	dmsg_pos_y       = y;
	dmsg_buff_width  = width;
	dmsg_buff_height = height;
	wait_scroll      = 0;		// dont wait for key if buffer is full
	do_scroll        = 0;
	
	// setup the frame buffer
#ifndef STATIC_BUFF
	dmsg_buff = (uint8_t*)malloc(width * height);
	if (dmsg_buff == NULL) return -1;
#endif
	dmsg_buff_img.data     = dmsg_buff;
	dmsg_buff_img.width    = width;
	dmsg_buff_img.height   = height;
	dmsg_buff_img.img_type = IMG_TYPE_8BPP;
	dmsg_buff_img.unk2     = 0;
	dmsg_buff_img.pal_data = dmsg_buff_pal;
	dmsg_buff_img.pal_size = 2;
	dmsg_buff_img.unk3     = 0xffffff80;
	
	// clear the screen
	dmsg_clear();
	
	// create a palette
	dmsg_buff_pal[0] = 0;		// black
	dmsg_buff_pal[1] = 0xFFFF;	// white

	// register frame buffer to the game API
	gfx_load_image(&dmsg_buff_img, &dmsg_buff_id);
	
	gfx_set_colorrop(0xcc);
}

void dmsg_shutdown(void)
{
	if (dmsg_buff != NULL) {
		gfx_free_image(dmsg_buff_id);
#ifndef STATIC_BUFF
		free(dmsg_buff);
#endif
	}
}

void dmsg_update(void)
{
	gfx_point2d_t pos;
	gfx_rect_t    rect;
	
//	if (do_scroll == 0) {
		pos.x = dmsg_pos_x;
		pos.y = dmsg_pos_y;
		rect.x = 0;
		rect.y = 0;
		rect.width = dmsg_buff_width;
		rect.height = dmsg_buff_height;
		gfx_bitblt(dmsg_buff_id, &rect, &pos);
//	}
/*	else {
	//	if (dmsg_cursor_y > 0) {
			pos.x = dmsg_pos_x;
			pos.y = dmsg_pos_y;
			rect.x = 0;
			rect.y = dmsg_cursor_y + 8;
			rect.width = dmsg_buff_width;
			rect.height = dmsg_buff_height - dmsg_cursor_y;
			gfx_bitblt(dmsg_buff_id, &rect, &pos);
	//	}
		
		if (dmsg_cursor_y > 0) {
			pos.y = dmsg_pos_y + dmsg_cursor_y;
			rect.y = 0;
			rect.height = dmsg_cursor_y;
			gfx_bitblt(dmsg_buff_id, &rect, &pos);
		}
	}
*/
	gfx_flush();
	gfx_paint();
}

#define	FONTCOLOR_LO	0
#define	FONTCOLOR_HI	1
int dmsg_putchar(int x, int y, char c)
{
	uint8_t		pattern = 0;
	uint8_t		*fb;
	int			sx, sy;
	int			idx = c;
	
	if ((c < 32) || (c > 127)) return -1;
	
	idx = (idx - 32) << 3;
	fb = &dmsg_buff[y * dmsg_buff_width +  x];
	for (sy=0; sy<8; sy++) {
		pattern = font6x8_bits[idx];
		idx++;
		
		for (sx=0; sx<8; sx++) {
			if (pattern & 1) fb[sx] = 1;
			else fb[sx] = 0;
			pattern = pattern >> 1;
		}
		fb += dmsg_buff_width;
	}
	
	return 0;
}

int dmsg_puts(char *__s)
{
	int i;
	
	for (i=0; i<256; i++) {
		if (__s[i] == '\0') break;
		if (__s[i] == '\n') {
			dmsg_cursor_y += 8;
			dmsg_cursor_x = 0;
		}
		else {
			dmsg_putchar(dmsg_cursor_x, dmsg_cursor_y, __s[i]);
			dmsg_cursor_x += 8;
		}
		if (dmsg_cursor_x > (dmsg_buff_width-8)) {
			dmsg_cursor_x = 0;
			dmsg_cursor_y += 8;
		}
		if (dmsg_cursor_y > (dmsg_buff_height - 8)) {
			if (wait_scroll == 1) {
				key_data_t keys;
				
				// wait for 'O' press
				while (1) {
					get_keys(&keys);
					if (keys.key2 & KEY_O) break;
				}
				
				// wait for 'O' release
				while (1) {
					get_keys(&keys);
					if ((keys.key2 & KEY_O) == 0) break;
				}
			}
			
			dmsg_clear();
			dmsg_cursor_y = 0;
		}
	}

	dmsg_update();
	return 0;
}

int dmsg_printf(char *format, ...)
{
	va_list args;
	char str[256];
	
	va_start(args, format);
	vsnprintf(str, 255, format, args);
	
	dmsg_puts(str);
	
	return 0;
}
