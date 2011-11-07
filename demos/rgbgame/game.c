
//#include "libgame.h"

#define	NULL	(void*)0

extern void **ftab;

#define COLOR_ROP_TRANSP	0xcc	// transparency
#define COLOR_ROP_NOP		0xf0	// nop - just paint

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;

typedef struct gfx_rect {
	uint16_t	x, y;
	uint16_t	width, height;
} gfx_rect_t;

typedef struct key_data {
	uint32_t	key1;
	uint32_t	key2;
} key_data_t;

void (*dprintf)(char *fmt, ...);

//int (*gfx_init)();
int (*gfx_init)(void *buffer, int buffsize);
int (*gfx_set_framebuffer)(int width, int height);
int (*gfx_set_display_screen)(gfx_rect_t *rect);//int with, int height);
int (*gfx_set_cammmode)(int mode);
int (*gfx_set_fgcolor)(uint32_t *color);
uint32_t (*gfx_get_fgcolor)();
int (*gfx_set_colorrop)(uint32_t rop);
int (*gfx_fillrect)(gfx_rect_t *rect);
int (*gfx_enable_feature)(uint32_t feature);
int (*gfx_flush)();
int (*gfx_paint)();

uint32_t (*get_time)();	// returns system ticks equivalent
void (*get_keys)(key_data_t *keys);//uint64_t *keys);

#define	FUNC(n)		*(ftab + (n >> 2))

#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);

int main()
{
	int i;
	gfx_rect_t rect;
	uint32_t color;
	uint16_t fbuff[320*240];
	key_data_t keys, no_keys;

	// setup function pointers
	dprintf               = FUNC(0x04);
	gfx_init              = FUNC(0x38);
	gfx_set_framebuffer   = FUNC(0x90);
	gfx_set_display_screen= FUNC(0x54);
	gfx_set_cammmode      = FUNC(0x8c);
	gfx_set_colorrop      = FUNC(0x3c);
	gfx_set_fgcolor       = FUNC(0x44);
	gfx_get_fgcolor       = FUNC(0x48);
	gfx_fillrect          = FUNC(0xc4);
	gfx_enable_feature    = FUNC(0x7c);
	gfx_flush             = FUNC(0xc);
	gfx_paint             = FUNC(0x10);
	get_time              = FUNC(0x124);
	get_keys              = FUNC(0x100);

//	dprintf("Hello World!\n");
	gfx_init(fbuff, sizeof(fbuff));

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	gfx_set_framebuffer(320, 240);
	gfx_set_display_screen(&rect);//320, 240);

	color = MAKE_RGB(255,0,0);
	gfx_enable_feature(3);
	gfx_set_fgcolor(&color);
	gfx_set_colorrop(COLOR_ROP_NOP);
	gfx_fillrect(&rect);


	rect.x = 30;
	rect.y = 30;
	rect.width = 260;
	rect.height = 180;

	color = MAKE_RGB(0,255,0);
	gfx_set_fgcolor(&color);
	gfx_set_colorrop(COLOR_ROP_NOP);
	gfx_fillrect(&rect);

	rect.x = 60;
	rect.y = 60;
	rect.width = 200;
	rect.height = 120;

	color = MAKE_RGB(0,0,255);
	gfx_set_fgcolor(&color);
	gfx_set_colorrop(COLOR_ROP_NOP);
	gfx_fillrect(&rect);
/*
	// is it used at all ?
	for (i=0; i<320; i++) fbuff[(100*320)+i] = 0;
*/
	gfx_set_colorrop(COLOR_ROP_NOP);
	gfx_flush();
	gfx_paint();


	get_keys(&no_keys);
//	no_keys.key2 &= ~0x5ff0;
	while (1) {
		get_keys(&keys);
//		keys.key2 &= ~0x5ff0;
		if (keys.key2 != no_keys.key2) break;
	}

	return 0;
}

