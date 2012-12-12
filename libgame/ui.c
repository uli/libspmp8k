/* ui.c - user interface routines; part of libgame
 *
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

#include "libgame.h"
#include "text.h"
#include <string.h>
#include <stdio.h>

static int bit_count(uint32_t val)
{
    int ret = 0;
    do {
        ret += (val & 1);
        val >>= 1;
    } while (val);
    return ret;
}

int libgame_map_buttons(const char *mapfile,
                        emu_keymap_t *keymap,
                        const libgame_buttonmap_t *buttonmap)
{
    int ret = -1;

    text_init();

    /// @todo Do this without messing with the display device buffers.
    uint16_t *old_shadow = gDisplayDev->getShadowBuffer();
    gDisplayDev->setShadowBuffer(gDisplayDev->getFrameBuffer());

    emu_keymap_t save = *keymap;

    text_set_fg_color(0);
    text_set_bg_color(0xffff);

restart:
    gDisplayDev->clear();
    memset(gDisplayDev->getShadowBuffer(), 0xff, gDisplayDev->getWidth() * gDisplayDev->getHeight() * 2);
    text_render("Press button for: (DOWN to skip)", 10, 10);

    int y = 30;
    int key;
    char buf[10];

    /* Wait for keypad silence. */
    while (emuIfKeyGetInput(keymap)) {}

    const libgame_buttonmap_t *kp;
    for (kp = buttonmap; kp->name; kp++, y += 10) {
        text_render(kp->name, 10, y);
        cache_sync();
        /* Wait for keypad silence. */
        while (emuIfKeyGetInput(keymap)) {}
        /* Wait for single key press. */
        while (!(key = emuIfKeyGetInput(keymap)) || bit_count(key) != 1) {}
        cyg_thread_delay(1); /* debounce */
        if (key & keymap->scancode[EMU_KEY_DOWN]) {
            keymap->scancode[kp->index] = 0;
            text_render("skipped", gDisplayDev->getWidth() - 10 - 8 * 8, y);
            continue;
        }
        sprintf(buf, "%9d", key);
        text_render(buf, gDisplayDev->getWidth() - 10 - 10*8, y);
        keymap->scancode[kp->index] = key;
    }

    text_render("Press UP to save, DOWN to start over.", 10, y + 20);
    cache_sync();
    for (;;) {
        /* Wait for keypad silence. */
        while (emuIfKeyGetInput(keymap)) {}
        /* Wait for single key press. */
        while (!(key = emuIfKeyGetInput(keymap)) || bit_count(key) != 1) {}
        if (key & keymap->scancode[EMU_KEY_DOWN])
            goto restart;
        else if (key & keymap->scancode[EMU_KEY_UP])
            break;
    }

    if (memcmp(keymap, &save, sizeof(emu_keymap_t))) {
        FILE *fp = fopen(mapfile, "wb");
        if (fp) {
            fwrite(&keymap->scancode[4], sizeof(uint32_t), 16, fp);
            fclose(fp);
            ret = 0;
        }
        else
            ret = -1;
    }

    gDisplayDev->setShadowBuffer(old_shadow);
    return ret;
}

int libgame_load_buttons(const char *mapfile, emu_keymap_t *keymap)
{
    FILE *fp = fopen(mapfile, "rb");
    if (fp) {
        fread(&keymap->scancode[4], sizeof(uint32_t), 16, fp);
        fclose(fp);
        return 0;
    }
    else
        return -1;
}
