/*
 * Copyright (C) 2011, Triple Oxygen
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
 *
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "libgame.h"
#include "_8k8bpcm.h"

void wait_keypress(void);

ge_res_entry_t resources[] = {
    {
        .name = "8k8bpcm.wav",
        .res_data = (uint8_t *)_8k8bpcm_data
    },
    {
        .name = "TAEND",
        .res_data = NULL
    }
};

void wait_keypress(void)
{
    ge_key_data_t keys, no_keys;

    NativeGE_getKeyInput4Ntv(&no_keys);

    while (1) {
        NativeGE_getKeyInput4Ntv(&keys);
        if (keys.key2 != no_keys.key2)
            break;
    }
}

int main(void)
{
    int type;
    ge_res_info_t ri;

    libgame_init();

    diag_printf("\n");
    diag_printf("Starting audio test.\n");

    NativeGE_initRes(0, resources);

    type = NativeGE_getRes("8k8bpcm.wav", &ri);
    NativeGE_playRes(type, 1, &ri);

    diag_printf("Playing a mono 8 bit / 8000 Hz PCM wave file.\n");
    diag_printf("Press any key to next sample.\n");
    wait_keypress();

    NativeGE_stopRes(type);

    return 0;
}
