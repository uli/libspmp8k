
#include "libgame.h"
#include "gfx_types.h"
#include "font.h"

#define	NULL	(void*)0

#ifndef HAVE_NEWLIB
extern int sprintf(char *__s, const char *format, ...);
#endif

void draw_string(uint8_t font_id, uint16_t x, uint16_t y, char *str);
void draw_string_centered(uint8_t font_id, uint16_t y, char *str);

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
	gfx_init(NULL, 0);

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	gfx_set_framebuffer(320, 240);
	gfx_set_display_screen(&rect);//320, 240);

	// pure black
	color = MAKE_RGB(0,0,0);
	gfx_enable_feature(3);
	gfx_set_fgcolor(&color);
	gfx_set_colorrop(COLOR_ROP_NOP);
	gfx_fillrect(&rect);

	// load an image
	if (gfx_load_image(&font_img, &font_id) != 0) return;

	draw_string_centered(font_id, 50, "KEY DEMO");
	gfx_set_colorrop(COLOR_ROP_TRANSP);
	gfx_flush();
	gfx_paint();

	color = MAKE_RGB(0, 0, 0);
	okeys.key1 = 0;
	okeys.key2 = 1;
	i=0;
	while (1) {
		get_keys(&keys);

		if (keys.key1 != okeys.key1) {
			rect.x = 0;
			rect.y = 100;
			rect.width = 320;
			rect.height = 16;
			gfx_set_colorrop(0xcc);
			gfx_fillrect(&rect);
			sprintf(key1, "KEY1: %08X", keys.key1);
			draw_string_centered(font_id, 100, key1);
			okeys.key1 = keys.key1;
			gfx_flush();
			gfx_paint();
		}
		
		if (keys.key2 != okeys.key2) {
			rect.x = 0;
			rect.y = 140;
			rect.width = 320;
			rect.height = 16;
			gfx_set_colorrop(0xcc);
			gfx_fillrect(&rect);
			sprintf(key2, "KEY2: %08X", keys.key2);
			draw_string_centered(font_id, 120, key2);
			okeys.key2 = keys.key2;
			gfx_flush();
			gfx_paint();
		}
	}

	gfx_free_image(font_id);

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
			gfx_set_colorrop(0xcc);
			gfx_sprite(font_id, &rect, &pos);
			pos.x += 16;
		}
		i++;
	}
}
