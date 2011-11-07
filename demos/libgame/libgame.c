/* libgame - SPMP8000 game API interface
 *
 * Copyright (C) 2010, alemaxx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "libgame.h"

void **ftab;

//extern char *heap_ending;
extern int heap_ending;

void (*_dprintf)(char *fmt, ...);

int (*gfx_init)(void *buffer, int buffsize);
int (*gfx_set_framebuffer)(int width, int height);
int (*gfx_set_display_screen)(gfx_rect_t *rect);
int (*gfx_set_cammmode)(int mode);
int (*gfx_set_fgcolor)(uint32_t *color);
uint32_t (*gfx_get_fgcolor)();
int (*gfx_set_colorrop)(uint32_t rop);
int (*gfx_set_alpha)(uint8_t src_alpha, uint8_t dest_alpha);
int (*gfx_get_alpha)(uint8_t *src_alpha, uint8_t *dest_alpha);
int (*gfx_fillrect)(gfx_rect_t *rect);
int (*gfx_enable_feature)(uint32_t feature);
int (*gfx_flush)();
int (*gfx_paint)();
int (*gfx_load_image)(gfx_loadimg_t *loadimg, uint8_t *imgid);
int (*gfx_free_image)(uint8_t img_id);
int (*gfx_bitblt)(uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);
int (*gfx_sprite)(uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);

int (*res_init)(int val, void *res_table);
int (*res_get)(char *filename,void *res_info);
int (*res_play)(uint8_t res_type, int flags, void *res_info);
int (*res_stop)(int arg);

int (*fs_open)(const char *filename, int flags, int *fd);
int (*fs_read)(int fd, const void *buf, size_t count, int *result);
int (*fs_write)(int fd, const void *buf, size_t count, int *result);
int (*fs_close)(int fd);
uint64_t (*fs_seek)(int fd, int offset, int whence);

uint32_t (*get_time)();	// returns system ticks equivalent
void (*get_keys)(key_data_t *keys);//uint64_t *keys);

#define	FUNC(n)		*(ftab + (n >> 2))

int libgame_init(void)
{
	// setup function pointers
	_dprintf               = FUNC(0x04);
	gfx_init              = FUNC(0x38);
	gfx_set_framebuffer   = FUNC(0x90);
	gfx_set_display_screen= FUNC(0x54);
	gfx_set_cammmode      = FUNC(0x8c);
	gfx_set_colorrop      = FUNC(0x3c);
	gfx_set_fgcolor       = FUNC(0x44);
	gfx_get_fgcolor       = FUNC(0x48);
	gfx_set_alpha         = FUNC(0x74);
	gfx_get_alpha         = FUNC(0x78);
	gfx_fillrect          = FUNC(0xc4);
	gfx_enable_feature    = FUNC(0x7c);
	gfx_flush             = FUNC(0xc);
	gfx_paint             = FUNC(0x10);
	gfx_load_image        = FUNC(0x14);
	gfx_free_image        = FUNC(0x18);
	gfx_bitblt            = FUNC(0xB4);
	gfx_sprite            = FUNC(0xB8);

	res_init              = FUNC(0xD4);
	res_get               = FUNC(0xD8);
	res_play              = FUNC(0xDC);
	res_stop              = FUNC(0xE8);

	fs_open               = FUNC(0x13C);
	fs_read               = FUNC(0x140);
	fs_write              = FUNC(0x144);
	fs_close              = FUNC(0x148);
	fs_seek               = FUNC(0x14C);

	get_time              = FUNC(0x124);
	get_keys              = FUNC(0x100);
	
//	heap_ending = (char*)0;
	heap_ending = 0;
}

