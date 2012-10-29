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
display_dev_t *gDisplayDev = 0;
int (*_ecos_close)(int fd) = 0;
int (*_ecos_read)(int fd, void *buf, unsigned int count) = 0;
int (*_ecos_write)(int fd, const void *buf, unsigned int count) = 0;
int (*_ecos_lseek)(int fd, int offset, int whence) = 0;
int (*_ecos_fstat)(int fd, struct _ecos_stat *buf) = 0;
int (*_ecos_open)(const char *pathname, int flags, int mode) = 0;
_ecos_DIR *(*_ecos_opendir)(const char *name) = 0;
void *_ecos_cyg_error_get_errno_p = 0;
void *_ecos_cyg_fd_alloc = 0;
struct _ecos_dirent *(*_ecos_readdir)(_ecos_DIR *dirp) = 0;
int (*_ecos_readdir_r)(_ecos_DIR *dirp, struct _ecos_dirent *entry, struct _ecos_dirent **result) = 0;
int (*_ecos_closedir)(_ecos_DIR *dirp) = 0;
int (*_ecos_stat)(const char *path, struct _ecos_stat *buf) = 0;
char *(*_ecos_getcwd)(char *buf, size_t size) = 0;
int (*_ecos_chdir)(const char *path) = 0;
int (*_ecos_unlink)(const char *pathname) = 0;
int (*_ecos_rmdir)(const char *pathname) = 0;
int (*_ecos_mkdir)(const char *pathname, _ecos_mode_t mode) = 0;
int (*_ecos_fsync)(int fd) = 0;
void (*cyg_thread_delay)(uint64_t /* cyg_tick_count_t */ delay) = 0;

uint16_t (*SPMP_SendSignal)(uint16_t cmd, void *data, uint16_t size) = 0;
void (*cache_sync)(void) = 0;
int (*NativeGE_getKeyInput)(key_data_t *) = 0;

/* int (*MCatchStoreImage)(void) = 0; doesn't do anything */
/* int (*MCatchDecodeImageFromCard)(void) = 0; doesn't do anything */
int (*MCatchGetColorROP)(uint32_t *rop) = 0;
int (*MCatchSetBitPlaneMask)(int read_write, uint16_t mask) = 0;
int (*MCatchGetBitPlaneMask)(int read_write, uint16_t *mask) = 0;
int (*MCatchGetDisplayScreen)(gfx_rect_t *) = 0;
int (*MCatchSetRectClip)(gfx_rect_t *) = 0;
int (*MCatchGetRectClip)(gfx_rect_t *) = 0;
int (*MCatchSetStyleMask)(gfx_rect_t *) = 0;
int (*MCatchGetStyleMask)(gfx_rect_t *) = 0;
int (*MCatchSetLineMask)(gfx_rect_t *) = 0;
int (*MCatchGetLineMask)(gfx_rect_t *) = 0;
int (*MCatchDisableFeature)(int) = 0;
int (*MCatchSetStyleLine)(uint8_t, uint8_t) = 0;
/* int (*MCatchPreviewColorkey)(void) = 0; doesn't do anything */
int (*MCatchGetFrameBuffer)(uint16_t *width, uint16_t *height) = 0;
int (*MCatchSetMutableImage)(uint8_t) = 0;
int (*MCatchSetPerPixelAlphaEq)(uint8_t) = 0; /* 0 or 1 */
int (*MCatchSetTransformation)(gfx_point2d_t *, int /* 0 to 7 */) = 0;
int (*MCatchQueryImage)(uint8_t, uint8_t /* 1 to 3 */) = 0;
int (*MCatchEnableDoubleBuffer)(int /* 0 or 1 */) = 0;
int (*MCatchGradientFill)(gfx_rect_t *, uint16_t[6], uint32_t[2]) = 0;
/* int (*MCatchUpdateScreen)(void) = 0; doesn't do anything (except produce debug output) */
int (*MCatchShowFont)(gfx_point2d_t *, int, uint8_t /* < 0x18 */, uint8_t /* < 0x18 */) = 0;
int (*MCatchModifyPalette)(uint8_t, uint8_t, uint8_t /* size */, void * /* data */) = 0;
void (*NativeGE_pauseRes)(uint8_t) = 0;
void (*NativeGE_resumeRes)(uint8_t) = 0;
int (*NativeGE_writeRecord)(const char *pathname, void *buf, uint8_t flags, _ecos_off_t offset, _ecos_size_t count) = 0;
int (*NativeGE_readRecord)(const char *pathname, void *buf, uint8_t flags, _ecos_off_t offset, _ecos_size_t count) = 0;
/* void (*NativeGE_showFPS)(void) = 0; doesn't do anything */
int (*NativeGE_gameExit)(void) = 0;
/* int (*NativeGE_getTPEvent)(void) = 0; doesn't do anything */
/* char (*NativeGE_setTPClickArea)(void) = 0; doesn't do anything */

int _has_frame_pointer = -1;	/* required to find function entry points */
int _new_emu_abi = -1;

int (*emuIfGraphInit)(graph_params_t *);
int (*emuIfGraphShow)(void);
int (*emuIfGraphChgView)(graph_params_t *);
int (*emuIfGraphCleanup)(void);
uint32_t (*emuIfSoundInit)(sound_params_t *params);
uint32_t (*emuIfSoundPlay)(sound_params_t *params);
uint32_t (*emuIfSoundCleanup)(sound_params_t *params);

int (*emuIfunknown0c)(void *) = 0;	/* sets the source buffer? */
int (*emuIfKeyInit)(void *) = 0;
int (*emuIfKeyGetInput)(void *) = 0;
int (*emuIfKeyCleanup)(void *) = 0;
uint32_t (*emuIfGetCurTime)(void) = 0; /* could be uint64_t, not sure */
void (*emuIfTimeDelay)(uint32_t) = 0;
int (*emuIfFsFileOpen)(const char *pathname, uint32_t flags) = 0;
uint32_t (*emuIfFsFileGetSize)(int fd) = 0;
int (*emuIfFsFileWrite)(int fd, const void *buf, uint32_t count) = 0;
int (*emuIfFsFileRead)(int fd, void *buf, uint32_t count) = 0;
int (*emuIfFsFileGetChar)(int fd) = 0;
int (*emuIfFsFileSeek)(int fd, _ecos_off_t offset, uint8_t whence) = 0;
_ecos_off_t (*emuIfFsFileCurPos)(int fd) = 0;
int (*emuIfFsFileClose)(int fd) = 0;
int (*emuIfkgbCevaLoader)(void) = 0;
int (*emuIfcevaImageUnload)(void) = 0;
int (*emuIfunknown5c)(void *) = 0;
int (*emuIfunknown60)(void *) = 0;
int (*emuIfunknown64)(void) = 0;
void (*emuIfunknown68)(void *) = 0;
int (*emuIfunknown6c)(int, int, int) = 0;
void (*emuIfunknown74)(int, void *, int) = 0;
int (*emuIfunknown78)(void) = 0;

/* returns true if this is a pointer into the firmware area */
static int is_ptr(uint32_t val) {
	return (val >= FW_START && val < FW_END);
}
/* returns true if val is an unconditional BL insn */
static int is_branch_link(uint32_t val) {
	return (val & 0xff000000U) == 0xeb000000U;
}
/* returns the target of the branch at loc */
static void *branch_address(uint32_t *loc) {
	int32_t diff = ((int32_t)(*loc << 8)) >> 8;
	return (void *)(loc + diff + 2);
}
/* returns the address of the next unconditional BL insn */
static uint32_t *next_bl(uint32_t *loc) {
	uint32_t *head;
	for (head = loc; head < loc + 100; head++) {
		if (is_branch_link(*head)) {
			return head;
		}
	}
	return 0;
}
/* returns the target of the next unconditional BL insn */
static uint32_t *next_bl_target(uint32_t *loc) {
	uint32_t *next = next_bl(loc);
	if (!next)
		return 0;
	else
		return branch_address(next);
}
/* Returns true if val is an instruction you would expect at the start of
   a function, being either a MOV R12, SP for firmwares compiled with
   frame pointer, or an STMFD SP!, {...} for firmwares compiled without.
   In the latter case, this technique fails on very small functions that
   save zero or one register (using STR). */
static uint32_t is_prolog(uint32_t val) {
	return (_has_frame_pointer && val == 0xe1a0c00d /* MOV R12, SP */) ||
	        (!_has_frame_pointer && (val & 0xffff0000U) == 0xe92d0000 /* STMFD SP!, {...} */);
}

/* Find a filesystem function via the filesystem method it uses. */
static uint32_t *find_fs_function(int index) {
	uint32_t *head;
	for (head = FW_START_P; head < FW_END_P; head++) {
		if (is_prolog(*head)) {
			uint32_t *subhead;
			int errno_p_found = 0;
			for (subhead = head + 1; subhead < FW_END_P; subhead++) {
				if (is_prolog(*subhead))	/* start of next function */
					break;
				if (is_branch_link(*subhead) && branch_address(subhead) == _ecos_cyg_error_get_errno_p)
					errno_p_found = 1;
				else if (errno_p_found && (*subhead & 0xfff0ffffU) == (0xe590f000U + index) /* MOV PC, [Rx, #offset] */) {
					return head;
				}
			}
		}
	}
	return 0;
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
			gDisplayDev = (display_dev_t *)*(head + (*head & 0xfff) / 4 + 2);
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
	
	/* There are only two functions that use cyg_fd_alloc(), and only
	   opendir() calls it first. */
	for (head = FW_START_P; head < FW_END_P; head++) {
		if (is_prolog(*head)) {
			if (next_bl_target(head) == _ecos_cyg_fd_alloc) {
				_ecos_opendir = (void *)head;
				break;
			}
		}
	}

	/* Find readdir_r() on the assumption that it's the only function
	   reading a block of 0x104 bytes. */
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

	/* readdir() calls readdir_r(), but so does another function, so
	   we make sure we have the right one by checking for a call to
	   cyg_error_get_errno_p(). */
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
	
	/* Find various file functions */
	_ecos_fsync = (void *)find_fs_function(0x14);
	_ecos_unlink = (void *)find_fs_function(0x18);
	_ecos_mkdir = (void *)find_fs_function(0x1c);
	_ecos_rmdir = (void *)find_fs_function(0x20);
	_ecos_stat = (void *)find_fs_function(0x34);

	/* This could go wrong because two other exported eCos functions use
	   the getinfo() method at offset 0x38 (pathconf() and
	   cyg_fs_get_attrib()), but we seem to be lucky: I could not find any
	   trace of them in the firmwares investigated. */
	_ecos_getcwd = (void *)find_fs_function(0x38);

	_ecos_chdir = (void *)find_fs_function(0x30);

	/* Find cache_sync(), the last function called by MCatchPaint(). */
	start = (uint32_t *)gfx_paint;
	int found_next_prolog = 0;
	for (head = start + 1; head < start + 200; head++) {
		if (is_branch_link(*head))
			cache_sync = branch_address(head);
		else if (is_prolog(*head)) {
			found_next_prolog = 1;
			break;
		}
	}
	if (!found_next_prolog)
		cache_sync = 0;
	
	uint32_t *getKeyFromQueue = next_bl_target((uint32_t *)get_keys);
	if (getKeyFromQueue) {
		for (head = FW_START_P; head < FW_END_P; head++) {
			if (is_prolog(*head)) {
				if (next_bl_target(head) == getKeyFromQueue &&
				    head != (void *)get_keys) {
				    	NativeGE_getKeyInput = (void *)head;
				    	break;
				}
			}
		}
	}

	return;
}

#ifndef TEST_BUILD
static
#endif
void libgame_assign_emuif(void)
{
	emuIfGraphInit		= EMU_FUNC_ENTRY(0x00);
	emuIfGraphShow		= EMU_FUNC_ENTRY(0x04);
	emuIfGraphChgView	= EMU_FUNC_ENTRY(0x08);
	if (_new_emu_abi)
	  emuIfunknown0c = EMU_FUNC_ENTRY(0x0c);
	emuIfGraphCleanup	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x10 : 0x0c);
	emuIfSoundInit		= EMU_FUNC_ENTRY(_new_emu_abi ? 0x14 : 0x10);
	emuIfSoundPlay		= EMU_FUNC_ENTRY(_new_emu_abi ? 0x18 : 0x14);
	emuIfSoundCleanup	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x1c : 0x18);
	emuIfKeyInit     	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x20 : 0x1c);
	emuIfKeyGetInput 	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x24 : 0x20);
	emuIfKeyCleanup  	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x28 : 0x24);
	emuIfGetCurTime  	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x2c : 0x28);
	emuIfTimeDelay   	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x30 : 0x2c);
	emuIfFsFileOpen  	= EMU_FUNC_ENTRY(_new_emu_abi ? 0x34 : 0x30);
	emuIfFsFileGetSize      = EMU_FUNC_ENTRY(_new_emu_abi ? 0x38 : 0x34);
	emuIfFsFileWrite        = EMU_FUNC_ENTRY(_new_emu_abi ? 0x3c : 0x38);
	emuIfFsFileRead         = EMU_FUNC_ENTRY(_new_emu_abi ? 0x40 : 0x3c);
	emuIfFsFileGetChar      = EMU_FUNC_ENTRY(_new_emu_abi ? 0x44 : 0x40);
	emuIfFsFileSeek         = EMU_FUNC_ENTRY(_new_emu_abi ? 0x48 : 0x44);
	emuIfFsFileCurPos       = EMU_FUNC_ENTRY(_new_emu_abi ? 0x4c : 0x48);
	emuIfFsFileClose        = EMU_FUNC_ENTRY(_new_emu_abi ? 0x50 : 0x4c);
	if (_new_emu_abi) {
		emuIfkgbCevaLoader   = EMU_FUNC_ENTRY(0x54);
		emuIfcevaImageUnload = EMU_FUNC_ENTRY(0x58);
		emuIfunknown5c       = EMU_FUNC_ENTRY(0x5c);
		emuIfunknown60       = EMU_FUNC_ENTRY(0x60);
		emuIfunknown64       = EMU_FUNC_ENTRY(0x64);
		emuIfunknown68       = EMU_FUNC_ENTRY(0x68);
		emuIfunknown6c       = EMU_FUNC_ENTRY(0x6c);
		/* diag_printf 0x70 */
		emuIfunknown74       = EMU_FUNC_ENTRY(0x74);
		emuIfunknown78       = EMU_FUNC_ENTRY(0x78);
	}
}

void libgame_init(void)
{
	// setup function pointers
	diag_printf               = FUNC(0x04);
	gfx_flush             = FUNC(0xc);
	gfx_paint             = FUNC(0x10);
	gfx_load_image        = FUNC(0x14);
	gfx_free_image        = FUNC(0x18);
	/* MCatchStoreImage      = FUNC(0x1c);	doesn't do anything */
	/* MCatchDecodeImageFromCard = FUNC(0x28); doesn't do anything */
	gfx_init              = FUNC(0x38);
	gfx_set_colorrop      = FUNC(0x3c);
	MCatchGetColorROP     = FUNC(0x40);
	gfx_set_fgcolor       = FUNC(0x44);
	gfx_get_fgcolor       = FUNC(0x48);
	MCatchSetBitPlaneMask     = FUNC(0x4c);
	MCatchGetBitPlaneMask     = FUNC(0x50);
	gfx_set_display_screen= FUNC(0x54);
	MCatchGetDisplayScreen    = FUNC(0x58);
	MCatchSetRectClip         = FUNC(0x5c);
	MCatchGetRectClip         = FUNC(0x60);
	MCatchSetStyleMask        = FUNC(0x64);
	MCatchGetStyleMask        = FUNC(0x68);
	MCatchSetLineMask         = FUNC(0x6c);
	MCatchGetLineMask         = FUNC(0x70);
	gfx_set_alpha         = FUNC(0x74);
	gfx_get_alpha         = FUNC(0x78);
	gfx_enable_feature    = FUNC(0x7c);
	MCatchDisableFeature      = FUNC(0x80);
	MCatchSetStyleLine        = FUNC(0x84);
	/* MCatchPreviewColorkey     = FUNC(0x88); doesn't do anything */
	gfx_set_cammmode      = FUNC(0x8c);
	gfx_set_framebuffer   = FUNC(0x90);
	MCatchGetFrameBuffer      = FUNC(0x94);
	MCatchSetMutableImage     = FUNC(0x98);
	MCatchSetPerPixelAlphaEq  = FUNC(0x9c);
	MCatchSetTransformation   = FUNC(0xa0);
	MCatchQueryImage          = FUNC(0xa4);
	MCatchEnableDoubleBuffer  = FUNC(0xa8);
	gfx_bitblt            = FUNC(0xB4);
	gfx_sprite            = FUNC(0xB8);
	MCatchGradientFill    = FUNC(0xc0);
	gfx_fillrect          = FUNC(0xc4);
	/* MCatchUpdateScreen    = FUNC(0xc8); doesn't do anything */
	MCatchShowFont        = FUNC(0xcc);
	MCatchModifyPalette   = FUNC(0xd0);

	res_init              = FUNC(0xD4);
	res_get               = FUNC(0xD8);
	res_play              = FUNC(0xDC);
	NativeGE_pauseRes         = FUNC(0xE0);
	NativeGE_resumeRes        = FUNC(0xE4);
	res_stop              = FUNC(0xE8);
	
	NativeGE_writeRecord      = FUNC(0xec);
	NativeGE_readRecord       = FUNC(0xf0);

	get_keys              = FUNC(0x100);
	/* NativeGE_showFPS          = FUNC(0x108); doesn't do anything */
	cyg_thread_delay = FUNC(0x11c);
	get_time              = FUNC(0x124);
	NativeGE_gameExit         = FUNC(0x130);
	/* NativeGE_getTPEvent       = FUNC(0x134); doesn't do anything */
	/* NativeGE_setTPClickArea   = FUNC(0x138); doesn't do anything */
	
	fs_open               = FUNC(0x13C);
	fs_read               = FUNC(0x140);
	fs_write              = FUNC(0x144);
	fs_close              = FUNC(0x148);
	fs_seek               = FUNC(0x14C);

	
//	heap_ending = (char*)0;
	heap_ending = 0;
	
	libgame_detect_firmware_abi();
	libgame_assign_emuif();
}
