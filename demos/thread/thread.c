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
#include <stdio.h>
#include <string.h>

void wait_keypress(void);

void wait_keypress(void)
{
    ge_key_data_t keys, no_keys;

    do { 
        NativeGE_getKeyInput4Ntv(&no_keys);
        cyg_thread_delay(1);
    } while (no_keys.keys);

    while (1) {
        NativeGE_getKeyInput4Ntv(&keys);
        if (keys.keys != no_keys.keys)
            break;
        cyg_thread_delay(1);
    }
}

void draw_bar(int y, int h, uint8_t col)
{
    int w = gDisplayDev->getWidth() * 2;
    uint16_t *fb = gDisplayDev->getFrameBuffer();
    memset(fb + y * w, col, h * w);
}

volatile int one_done = 0;
volatile int two_done = 0;
volatile int exit_now = 0;

void one(cyg_addrword_t data) {
    (void)data;
    for (;;) {
        draw_bar(0, 20, 0);
        cyg_thread_delay(25);
        draw_bar(0, 20, 0xff);
        cyg_thread_delay(25);
        if (exit_now) {
            one_done = 1;
            break;
        }
    }
}

void two(cyg_addrword_t data) {
    (void)data;
    for (;;) {
        draw_bar(40, 20, 0x20);
        cyg_thread_delay(30);
        draw_bar(40, 20, 0x44);
        cyg_thread_delay(30);
        if (exit_now) {
            two_done = 1;
            break;
        }
    }
}

void end_threads(void)
{
    exit_now = 1;
    while (!(one_done && two_done))
        cyg_thread_delay(1);
}

int my_gameExit()
{
    end_threads();
    return NativeGE_gameExit();
}

int main(void)
{
    cyg_handle_t one_handle, two_handle;
    cyg_thread one_data, two_data;
    uint8_t one_stack[0x1000], two_stack[0x1000];

    libgame_init();
    g_stEmuAPIs->exit = my_gameExit;

    cyg_priority_t my_prio = cyg_thread_get_priority(cyg_thread_self());
    cyg_thread_create(my_prio, one, 0, "one", one_stack, 0x1000, &one_handle, &one_data);
    cyg_thread_create(my_prio, two, 0, "two", two_stack, 0x1000, &two_handle, &two_data);
    cyg_thread_resume(one_handle);
    cyg_thread_resume(two_handle);
    wait_keypress();
    end_threads();
    return 0;
}
