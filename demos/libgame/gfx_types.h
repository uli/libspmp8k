
#ifndef __GFX_TYPES_H__
#define __GFX_TYPES_H__

#define	IMG_TYPE_8BPP		1
#define	IMG_TYPE_4BPP		2

#define COLOR_ROP_TRANSP	0xcc	// WILDCARD
#define COLOR_ROP_NOP		0xf0	// unknown

#ifndef _STDINT_H
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;
#endif

typedef struct gfx_point2d {
	uint16_t	x, y;
} gfx_point2d_t;

typedef struct gfx_rect {
	uint16_t	x, y;
	uint16_t	width, height;
} gfx_rect_t;

typedef struct gfx_loadimg {
	uint8_t*	data;		// 0x00	- 0x2c
	uint16_t	width;		// 0x04	- 0x28
	uint16_t	height;		// 0x06	- 0x26
	uint32_t	img_type;	// 0x08	- 0x24
	uint32_t	unk2;		// 0x0C - 0x20 (byte) - 0
	uint16_t*	pal_data;	// 0x10	- 0x1c
	uint16_t	pal_size;	// 0x14 - 0x18
	uint32_t	unk3;		// 0x18	- 0x14
} gfx_loadimg_t;

typedef struct gfx_pal {
	uint16_t	type;
	uint16_t	entries;
	uint16_t	*data;
} gfx_pal_t;

#endif // __GFX_TYPES_H__

