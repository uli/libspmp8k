/*
 * Copyright (C) 2012, Ulrich Hecht <ulrich.hecht@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <libgame.h>
#include <adbg.h>

void wait_keypress(void);

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
    libgame_init();
    if (adbg_init() < 0)
        return 0;

    adbg_puts("\n");
    adbg_puts("Starting audio debug test.\n\n");

    for (;;) {
        adbg_puts("This is a debug message transmitted via line out. Do not be alarmed.\n");
        int i;
        for (i = 0; i < 10; i++)
            adbg_printf("This is %d of %d messages printed consecutively.\n", i, 10);
        adbg_puts("\nPress any key to repeat.\n\n");
        wait_keypress();
    }

    adbg_fini();
    return 0;
}
