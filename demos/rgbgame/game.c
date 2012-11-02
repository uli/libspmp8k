
#include <libgame.h>

#define COLOR_ROP_TRANSP	0xcc	// transparency
#define COLOR_ROP_NOP		0xf0	// nop - just paint

#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);

int main()
{
	int i;
	gfx_rect_t rect;
	uint32_t color;
	key_data_t keys, no_keys;

//	diag_printf("Hello World!\n");
	MCatchInitGraph();

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

