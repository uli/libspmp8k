
/*
 * libemu.h
 * libemu for SPMP8000
 *
 * Copyright (C) 2011 Triple Oxygen
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LIBEMU_H_
#define _LIBEMU_H_

#include "../libgame/libgame.h"

typedef struct sound_params {
	uint8_t *buf;		/* + 0x0 */
	uint32_t buf_size;	/* + 0x4 */
	uint32_t rate;		/* + 0x8 */
	uint8_t depth;		/* + 0xc */
	uint8_t channels;	/* + 0xd */
	uint32_t *callback;	/* + 0x10 */ /* used only when gEmuType is 5 (flash?) */
} sound_params_t;

typedef struct graph_params {
	uint16_t *pixels;	// goes to src_ctx + 4
	uint32_t width;	// +4
	uint32_t height;	// +8
	uint32_t unknown_flag;	// +c
	uint32_t palette;	// +10
	uint32_t pad2;
	uint32_t src_clip_x;	// +18
	uint32_t src_clip_y;
	uint32_t src_clip_w;	// +1c
	uint32_t src_clip_h;
} graph_params_t;

/*
 * This address is for Cybergame only!
 * If the code is running as a native game (BIN), we have no information about
 * the emu function table offset, so it may be either hardcoded or found in
 * runtime.
 */
#define EMU_FUNC_TABLE		0x004e7438

#define EMU_FUNC_ENTRY(n)	*((void **)(EMU_FUNC_TABLE + (n)))

extern int (*emuIfGraphInit)(graph_params_t *);
extern int (*emuIfGraphShow)(void);
extern int (*emuIfGraphCleanup)(void);
/*
extern uint32_t (*emuIfGraphInit)()
emuIfGraphShow
emuIfGraphChgView
emuIfGraphCleanup
*/
extern uint32_t (*emuIfSoundInit)(sound_params_t *params);
extern uint32_t (*emuIfSoundPlay)(sound_params_t *params);
extern uint32_t (*emuIfSoundCleanup)(sound_params_t *params);
/*
emuIfKeyInit
emuIfKeyGetInput
emuIfKeyCleanup
emuIfGetCurTime
emuIfTimeDelay
emuIfFsFileOpen
emuIfFsFileGetSize
emuIfFsFileWrite
emuIfFsFileRead
emuIfFsFileGetChar
emuIfFsFileSeek
emuIfFsFileCurPos
emuIfFsFileClose
*/
extern uint32_t (*diag_printf)(char *fmt, ...);

#endif /* _LIBEMU_H_ */
