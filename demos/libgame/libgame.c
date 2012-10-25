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

void (*diag_printf)(char *fmt, ...);

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

#define FW_START 0x280000
#define FW_START_P ((uint32_t *)FW_START)
#define FW_END 0xa00000
#define FW_END_P ((uint32_t *)FW_END)

void **g_stEmuFuncs = 0;
void **gDisplayDev = 0;
int (*_ecos_close)(int fd) = 0;
int (*_ecos_read)(int fd, void *buf, unsigned int count) = 0;
int (*_ecos_write)(int fd, const void *buf, unsigned int count) = 0;
int (*_ecos_lseek)(int fd, int offset, int whence) = 0;
int (*_ecos_fstat)(int fd, /* struct stat */ void *buf) = 0;
int (*_ecos_open)(const char *pathname, int flags, int mode) = 0;
DIR *(*_ecos_opendir)(const char *name) = 0;
void *_ecos_cyg_error_get_errno_p = 0;
void *_ecos_cyg_fd_alloc = 0;
int (*_ecos_readdir)(unsigned int fd, struct dirent *dirp,
                   unsigned int count) = 0;
int (*_ecos_readdir_r)(DIR *dirp, struct dirent *entry, struct dirent **result) = 0;
int (*_ecos_closedir)(DIR *dirp) = 0;

int _has_frame_pointer = -1;
uint16_t (*SPMP_SendSignal)(uint16_t cmd, void *data, uint16_t size) = 0;
int _new_emu_abi = -1;

static int is_ptr(uint32_t val) {
	return (val >= FW_START && val < FW_END);
}
static int is_branch_link(uint32_t val) {
	return (val & 0xff000000U) == 0xeb000000U;
}
static void *branch_address(uint32_t *loc) {
	int32_t diff = ((int32_t)(*loc << 8)) >> 8;
	return (void *)(loc + diff + 2);
}
static uint32_t *next_bl(uint32_t *loc) {
	uint32_t *head;
	for (head = loc; head < loc + 100; head++) {
		if (is_branch_link(*head)) {
			return head;
		}
	}
	return 0;
}
static uint32_t *next_bl_target(uint32_t *loc) {
	uint32_t *next = next_bl(loc);
	if (!next)
		return 0;
	else
		return branch_address(next);
}
static uint32_t is_prolog(uint32_t val) {
	return (_has_frame_pointer && val == 0xe1a0c00d /* MOV R12, SP */) ||
	        (!_has_frame_pointer && (val & 0xffff0000U) == 0xe92d0000 /* STMFD SP!, {...} */);
}

#ifndef TEST_BUILD
static
#endif
void libgame_detect_firmware_abi()
{
	uint32_t *head;
	uint32_t *start;
	
	/* Find g_stEmuFuncs */
	for (head = FW_START_P; head < FW_END_P; head++) {
		if (*head == (uint32_t)diag_printf) {
			uint32_t *subhead = head - 28;
			if (subhead < FW_START_P)
				continue;
			_new_emu_abi = 1;
			if (!is_ptr(*subhead)) {
				_new_emu_abi = 0;
				subhead = head - 20;
			}
			g_stEmuFuncs = (void **)subhead;
			for (; subhead < head; subhead++) {
				if (!is_ptr(*subhead)) {
					g_stEmuFuncs = NULL;
					break;
				}
			}
		}
		if (g_stEmuFuncs)
			break;
	}
	
	/* Find gDisplayDev */
	int getGameBuffWidth_found = 0;
	start = (uint32_t *)gfx_init;
	for (head = start; head < start + 100; head++) {
		if ((*head & 0xff000000U) == 0xeb000000U) {
			getGameBuffWidth_found = 1;
		}
		if (getGameBuffWidth_found && (*head & 0xffff0000U) ==
		    0xe59f0000U /* LDR Rx, [PC, #...] */) {
			gDisplayDev = (void **)*(head + (*head & 0xfff) / 4 + 2);
			break;
		}
	}
	
	/* Find eCos close and determine if built with frame pointer */
	if (!g_stEmuFuncs)
		return;
	_ecos_close = (void *)next_bl_target(g_stEmuFuncs[_new_emu_abi ? 20 : 19]);
	_ecos_closedir = (void *)_ecos_close;
	_has_frame_pointer = 0;
	if (*((uint32_t *)_ecos_close) == 0xe1a0c00d /* MOV R12, SP */) {
		_has_frame_pointer = 1;
	}
	
	/* Find eCos read/write */
	start = (uint32_t *)fs_read;
	for (head = start; head < start + 200; head++) {
		if (is_branch_link(*head)) {
			if (branch_address(head) == (uint32_t *)diag_printf)
				continue;
			_ecos_read = branch_address(head);
			break;
		}
	}
	start = (uint32_t *)fs_write;
	for (head = start; head < start + 200; head++) {
		if (is_branch_link(*head)) {
			/* skip debug output calls */
			if (branch_address(head) == (uint32_t *)diag_printf)
				continue;
			_ecos_write = branch_address(head);
			break;
		}
	}

	/* Find lseek, fstat */
	_ecos_lseek = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x4c : 0x48) / 4]);
	_ecos_fstat = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x38 : 0x34) / 4]);
	
	/* Find open */
	if (!_ecos_fstat)
		return;

	start = (uint32_t *)fs_open;
	void *previous = 0;
	for (head = start; head < start + 400; head++) {
		if (!is_branch_link(*head))
			continue;
		if (branch_address(head) == _ecos_fstat) {
			/* previous branch went to open */
			break;
		}
		previous = branch_address(head);
	}
	_ecos_open = previous;
	
	/* Find cyg_fd_alloc, cyg_error_get_errno_p; they are needed to find
	   opendir and readdir */
	if (!_ecos_open)
		return;
	start = (uint32_t *)_ecos_open;
	for (head = start; head < start + 200; head++) {
		if (is_branch_link(*head)) {
			if (!_ecos_cyg_error_get_errno_p)
				_ecos_cyg_error_get_errno_p = branch_address(head);
			else {
				_ecos_cyg_fd_alloc = branch_address(head);
				break;
			}
		}
	}
	
	for (head = FW_START_P; head < FW_END_P; head++) {
		if (is_prolog(*head)) {
			if (next_bl_target(head) == _ecos_cyg_fd_alloc) {
				_ecos_opendir = (void *)head;
				break;
			}
		}
	}

	for (head = FW_START_P; head < FW_END_P; head++) {
		if (is_prolog(*head)) {
		   	uint32_t *subhead;
		   	for (subhead = head + 1; subhead < head + 100 && !is_prolog(*subhead); subhead++) {
				if (*subhead == 0xe3a02f41 /* MOV R2, #0x104 */ &&
				    next_bl_target(subhead) == (void *)_ecos_read
				   ) {
					_ecos_readdir_r = (void *)head;
					goto out;
				}
			}
		}
	}
out:
	for (head = FW_START_P; head < FW_END_P; head++) {
		if (is_prolog(*head)) {
		   	uint32_t *subhead = next_bl(head);
		   	if (!subhead)
		   		continue;
		   	if (is_branch_link(*subhead) && branch_address(subhead) != _ecos_readdir_r) {
		   		head = subhead;
		   		continue;
			}
			subhead = next_bl(subhead + 1);
			if (branch_address(subhead) == _ecos_cyg_error_get_errno_p) {
				_ecos_readdir = (void *)head;
				break;
			}
		}
	}
	
	SPMP_SendSignal = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x28 : 0x24) / 4]);
	
	return;
}

void libgame_init(void)
{
	// setup function pointers
	diag_printf               = FUNC(0x04);
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
	/*
	* res_pause           = FUNC(0xE0);
	* res_resume          = FUNC(0xE4);
	*/
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
	
	libgame_detect_firmware_abi();
}

