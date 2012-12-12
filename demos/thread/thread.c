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
#include <string.h>

/* Wait for a key press. */
void wait_keypress(void)
{
    ge_key_data_t keys, no_keys;

    do { 
        NativeGE_getKeyInput4Ntv(&no_keys);
        /* Important: Never do busy waiting! eCos is a real-time operating
           system, and as long as a thread is busy, no other thread with
           the same or lower priority will run. If you have to poll, add
           a call to cyg_thread_delay() once in a while to make sure
           your other threads won't starve. */
        cyg_thread_delay(1);
    } while (no_keys.keys);

    while (1) {
        NativeGE_getKeyInput4Ntv(&keys);
        if (keys.keys != no_keys.keys)
            break;
        cyg_thread_delay(1);
    }
}

/* Draw a color bar on the screen. */
void draw_bar(int y, int h, uint8_t col)
{
    int w = gDisplayDev->getWidth() * 2;
    uint16_t *fb = gDisplayDev->getFrameBuffer();
    memset(fb + y * w, col, h * w);
}

/* The eCos documentation warns against using cyg_thread_delete() because
   it is "dangerous" to use. Instead, you are supposed to send the thread
   a message telling it to quit. We use these global variables for that
   purpose. */
volatile int one_done = 0;	/* Set to 1 by thread "one" when it has finished. */
volatile int two_done = 0;	/* Ditto for thread "two". */
volatile int exit_now = 0;	/* Set to 1 to signal threads to quit. */

/* Thread "one". The "data" parameter contains whatever you have passed
   to cyg_thread_create() when constructing the thread. We don't use it
   here. */
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

/* Thread "two". */
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

/* Signal threads to quit and wait until they have done so. */
void end_threads(void)
{
    exit_now = 1;
    while (!(one_done && two_done))
        cyg_thread_delay(1);
}

/* eCos won't clean up after you, so it's important to make sure that the
   threads you create will be terminated irrespective of the way your
   program is terminated. The default exit callback (NativeGE_gameExit())
   only terminates the main thread, so it is important to install a
   handler that reaps all threads that you have created yourself. */
int my_gameExit()
{
    end_threads();
    return NativeGE_gameExit();
}

int main(void)
{
    /* Thread handles used to control threads. */
    cyg_handle_t one_handle, two_handle;
    /* Memory used by eCos to store information about the thread. eCos tries
       to avoid dynamically allocating memory, which means that we have to
       do it... */
    cyg_thread one_data, two_data;
    /* Thread stacks. According to the documentation, cyg_thread_create()
       can dynamically allocate a stack if passed a NULL pointer, but that
       always led to a system crash for me. */
    uint8_t one_stack[0x1000], two_stack[0x1000];

    libgame_init();

    /* Install exit handler, see my_gameExit(). */
    g_stEmuAPIs->exit = my_gameExit;

    /* Create threads with the same priority as the man thread. */
    cyg_priority_t my_prio = cyg_thread_get_priority(cyg_thread_self());
    cyg_thread_create(my_prio,		/* thread priority; lower number means higher priority */
                      one,		/* thread's main function */
                      0,		/* data passed to thread's main function */
                      "one",		/* thread name (optional) */
                      one_stack,	/* memory for thread's stack */
                      0x1000,		/* size of thread's stack */
                      &one_handle,	/* where the thread handle is returned */
                      &one_data		/* memory used by OS for thread info */
                     );
    cyg_thread_create(my_prio, two, 0, "two", two_stack, 0x1000, &two_handle, &two_data);

    /* Threads are created in a suspended state. */
    cyg_thread_resume(one_handle);
    cyg_thread_resume(two_handle);

    wait_keypress();
    end_threads();
    return 0;
}
