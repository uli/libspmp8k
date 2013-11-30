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

#include "libgame.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "adbg.h"

#define SAMPLE_LOW -32767
#define SAMPLE_HI 32767

/* 5 samples per bit at 48 kHz (i.e. 9600 Baud) works best for me. */
#define PULSE_SAMPLES 5
#define SAMPLE_RATE 48000

#define AUDIO_BUF_SIZE (SAMPLE_RATE * 1) /* 1 second buffer */

static int16_t buf[AUDIO_BUF_SIZE];
static int buf_idx = 0;

static emu_sound_params_t sp;
static uint64_t next_idle = 0;

static void wait_for_idle(void)
{
    while (next_idle > libgame_utime()) {
        /* wait */
    }
}

static void flush_buffer(void)
{
    sp.buf = (uint8_t *)buf;
    sp.buf_size = buf_idx * 2;
    emuIfSoundPlay(&sp);
    next_idle = libgame_utime() + (1000000 / SAMPLE_RATE) * buf_idx;
    buf_idx = 0;
}

static void add_samples(int16_t val, int count)
{
    if (buf_idx + count > AUDIO_BUF_SIZE)
        return;
    while (count) {
        int i;
        for (i = 0; i < PULSE_SAMPLES; i++) {
            buf[buf_idx++] = val;
        }
        count--;
    }
}

static void serialize(char c)
{
    add_samples(SAMPLE_LOW, 2);
    int i;
    for (i = 0; i < 8; i++) {
        add_samples((c & 1) ? SAMPLE_HI : SAMPLE_LOW, 1);
        c >>= 1;
    }
    add_samples(SAMPLE_HI, 1);
}

void adbg_puts(const char *s)
{
    wait_for_idle();
    /* The audio line idles at 0, so we bring it to a valid logical state
       for the length of one byte to give the receiver time to recover
       from any spurious signals it may have received. */
    add_samples(SAMPLE_HI, 11);
    while (*s) {
        serialize(*s);
        s++;
    }
    add_samples(SAMPLE_HI, 11);
    flush_buffer();
}

int adbg_printf(const char *fmt, ...)
{
    int ret;
    char str[256];
    va_list args;
    va_start(args, fmt);
    ret = vsnprintf(str, 256, fmt, args);
    va_end(args);
    adbg_puts(str);
    return ret;
}

int adbg_init(void)
{
    libgame_set_debug(0);
    sp.rate = SAMPLE_RATE;
    sp.channels = 1;
    sp.buf_size = AUDIO_BUF_SIZE;
    sp.depth = 0;
    sp.callback = 0;
    int ret = emuIfSoundInit(&sp);
    return ret;
}

void adbg_fini(void)
{
    emuIfSoundCleanup();
}
