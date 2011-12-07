
/*
 * libemu.c
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

#include "libemu.h"

uint32_t (*emuIfSoundInit)(sound_params_t *params);
uint32_t (*emuIfSoundPlay)(sound_params_t *params);
uint32_t (*emuIfSoundCleanup)(sound_params_t *params);
uint32_t (*diag_printf)(char *fmt, ...);

void libemu_init(void)
{
	emuIfSoundInit		= EMU_FUNC_ENTRY(0x10);
	emuIfSoundPlay		= EMU_FUNC_ENTRY(0x14);
	emuIfSoundCleanup	= EMU_FUNC_ENTRY(0x18);
	diag_printf		= EMU_FUNC_ENTRY(0x50);
}
