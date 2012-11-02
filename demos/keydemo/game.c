
#include <stdio.h>
#include <string.h>

#include "libgame.h"
#include "gfx_types.h"
#include "font.h"

//#define	NULL	(void*)0

#ifndef HAVE_NEWLIB
extern int sprintf(char *__s, const char *format, ...);
#endif

void draw_string(uint8_t font_id, uint16_t x, uint16_t y, char *str);
void draw_string_centered(uint8_t font_id, uint16_t y, char *str);
uint16_t *(*int_get_framebuffer)(void);
uint16_t *(*int_get_shadowbuffer)(void);
void **itab = (void **)0x5a1e48;

int main()
{
	int i, j;
	gfx_point2d_t at;
	gfx_rect_t rect;
	uint32_t color;
	gfx_pal_t pal;
	uint8_t   font_id, res_type;
	uint8_t   img_id;
	uint32_t  res_data[12];
	char key1[32];
	char key2[32];
	int fd, err;
	key_data_t keys, okeys;

	// initialize the game api
	libgame_init();
	int_get_shadowbuffer = itab[0];
	int_get_framebuffer = itab[1];
	MCatchInitGraph();

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	MCatchSetFrameBuffer(320, 240);
	MCatchSetDisplayScreen(&rect);//320, 240);

	// pure black
	color = MAKE_RGB(255,0,0);
	MCatchEnableFeature(3);
	MCatchSetFGColor(&color);
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFillRect(&rect);

	// load an image
	if (MCatchLoadImage(&font_img, &font_id) != 0) return 0;

	draw_string_centered(font_id, 50, "KEY DEMO");
	//MCatchSetColorROP(COLOR_ROP_TRANSP);
	MCatchFlush();
	MCatchPaint();

#if 0
	int res;
	NativeGE_fsOpen("file.bin", FS_O_CREAT|FS_O_WRONLY, &fd);
	//NativeGE_fsWrite(fd, "Huhu!\n", 6, &res);
	NativeGE_fsWrite(fd, (void *)0x280000, 0x480000, &res);
	NativeGE_fsClose(fd);
	NativeGE_fsOpen("ftab.bin", FS_O_CREAT|FS_O_WRONLY, &fd);
	NativeGE_fsWrite(fd, (void *)ftab, 0x150, &res);
	NativeGE_fsClose(fd);
#endif
	
	color = MAKE_RGB(0, 255, 0);
	okeys.key1 = 0;
	okeys.key2 = 1;
	i=0;
	while (1) {
		NativeGE_getKeyInput4Ntv(&keys);

		if (keys.key1 != okeys.key1) {
			rect.x = 0;
			rect.y = 100;
			rect.width = 320;
			rect.height = 16;
			MCatchSetColorROP(0xcc);
			MCatchFillRect(&rect);
			sprintf(key1, "KEY1: %08X", keys.key1);
			draw_string_centered(font_id, 100, key1);
			okeys.key1 = keys.key1;
			MCatchFlush();
			MCatchPaint();
		}
		
		if (keys.key2 != okeys.key2) {
			rect.x = 0;
			rect.y = 140;
			rect.width = 320;
			rect.height = 16;
			MCatchSetColorROP(0xcc);
			MCatchFillRect(&rect);
			sprintf(key2, "KEY2: %08X", keys.key2);
			draw_string_centered(font_id, 120, key2);
			okeys.key2 = keys.key2;
			MCatchFlush();
			MCatchPaint();
		}
		MCatchFlush();
		MCatchPaint();
		uint16_t *fb = int_get_framebuffer();
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
		fb = int_get_shadowbuffer();
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
	}

	MCatchFreeImage(font_id);

	return 0;
}

#ifndef HAVE_NEWLIB
int strlen(char *str)
{
	int ret=0;
	while (str[ret] != 0) ret++;
	return ret;
}
#endif

void draw_string_centered(uint8_t font_id, uint16_t y, char *str)
{
	uint16_t x;
	int length;

	length = strlen(str);
	x = (320 - (length << 4)) >> 1;
	draw_string(font_id, x, y, str);
}

void draw_string(uint8_t font_id, uint16_t x, uint16_t y, char *str)
{
	gfx_point2d_t pos;
	gfx_rect_t    rect;
	int i, nchar;

	pos.x = x;
	pos.y = y;
	rect.width = 16;
	rect.height = 16;
	i = 0;

	while (str[i] != 0) {
		if ((str[i] > 0x1f) && (str[i] < 0x5c)) {
			// 16 chars per row, 4 rows
			nchar = str[i] -0x20;
			rect.x = (nchar & 0xf) << 4;
			rect.y = nchar & 0x30;
			MCatchSetColorROP(0xcc);
			MCatchSprite(font_id, &rect, &pos);
			pos.x += 16;
		}
		i++;
	}
}
