
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

//int (*MCatchInitGraph)();
int (*MCatchInitGraph)(void *buffer, int buffsize);
int (*MCatchSetFrameBuffer)(int width, int height);
int (*MCatchSetDisplayScreen)(gfx_rect_t *rect);//int with, int height);
int (*MCatchSetCameraMode)(int mode);
int (*MCatchSetFGColor)(uint32_t *color);
uint32_t (*MCatchGetFGColor)();
int (*MCatchSetColorROP)(uint32_t rop);
int (*MCatchFillRect)(gfx_rect_t *rect);
int (*MCatchEnableFeature)(uint32_t feature);
int (*MCatchFlush)();
int (*MCatchPaint)();

uint32_t (*NativeGE_getTime)();	// returns system ticks equivalent
void (*NativeGE_getKeyInput4Ntv)(key_data_t *keys);//uint64_t *keys);

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
	MCatchInitGraph              = FUNC(0x38);
	MCatchSetFrameBuffer   = FUNC(0x90);
	MCatchSetDisplayScreen= FUNC(0x54);
	MCatchSetCameraMode      = FUNC(0x8c);
	MCatchSetColorROP      = FUNC(0x3c);
	MCatchSetFGColor       = FUNC(0x44);
	MCatchGetFGColor       = FUNC(0x48);
	MCatchFillRect          = FUNC(0xc4);
	MCatchEnableFeature    = FUNC(0x7c);
	MCatchFlush             = FUNC(0xc);
	MCatchPaint             = FUNC(0x10);
	NativeGE_getTime              = FUNC(0x124);
	NativeGE_getKeyInput4Ntv              = FUNC(0x100);

//	dprintf("Hello World!\n");
	MCatchInitGraph(fbuff, sizeof(fbuff));

	rect.x = 0;
	rect.y = 0;
	rect.width = 320;
	rect.height = 240;

	MCatchSetFrameBuffer(320, 240);
	MCatchSetDisplayScreen(&rect);//320, 240);

	color = MAKE_RGB(255,0,0);
	MCatchEnableFeature(3);
	MCatchSetFGColor(&color);
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFillRect(&rect);

	rect.x = 30;
	rect.y = 30;
	rect.width = 260;
	rect.height = 180;

	color = MAKE_RGB(128,0,0);
	MCatchEnableFeature(3);
	MCatchSetFGColor(&color);
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFillRect(&rect);

	rect.x = 60;
	rect.y = 60;
	rect.width = 200;
	rect.height = 120;

	color = MAKE_RGB(0,0,255);
	MCatchSetFGColor(&color);
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFillRect(&rect);
///*
	// is it used at all ?
	for (i=0; i<320; i++) fbuff[(100*320)+i] = 0;
//*/
	MCatchSetColorROP(COLOR_ROP_NOP);
	MCatchFlush();
	MCatchPaint();

	int fd = open("file.bin", FS_O_CREAT|FS_O_WRONLY, 0644);
	write(fd, "Huhu!\n", 6);
	close(fd);
	
	NativeGE_getKeyInput4Ntv(&no_keys);
//	no_keys.key2 &= ~0x5ff0;
	while (1) {
		NativeGE_getKeyInput4Ntv(&keys);
//		keys.key2 &= ~0x5ff0;
		if (keys.key2 != no_keys.key2) break;
		uint16_t *fb = (uint16_t *)0x1d80000;
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
		fb = (uint16_t *)0x1da0000;
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
		fb = (uint16_t *)0x1dc0000;
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
		fb = (uint16_t *)0x1de0000;
		for (i = 0; i < 65536; i++) {
			fb[i] = i;
		}
	}

	return 0;
}

