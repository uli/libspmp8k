/* text.c - bitmap font rendering routines; part of libgame
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


#include "text.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "libgame.h"
#include <string.h>
#include <stdio.h>

static uint16_t fg_color = 0xffff;
static uint16_t bg_color = 0;

static uint8_t *asc12_font = 0;
static uint8_t *asc16_font = 0;
static uint16_t *hzx12_font = 0;
static uint16_t *hzx16_font = 0;
#include "hzktable.c"

struct sunplus_font_header {
    uint8_t _pad1[0x10];
    uint32_t glyph_start;
    uint32_t _pad2;
    uint32_t glyph_offset;
    uint32_t _pad3[2];
    uint32_t glyph_bytes;
};

#define FP "/Rom/mw/fonts/SUNPLUS/"
static struct sunplus_font {
    const char *face_name;
    const char *width_name;
    int8_t *width;
    FILE *face;
    struct sunplus_font_header *header;
    uint16_t *char_data;
} sunplus_fonts[] = {
    [FONT_FACE_SONGTI]      = {FP "SONGTI.FONT", FP "SONGTI.WID", 0, 0, 0, 0},
    [FONT_FACE_SONGTI_BOLD] = {FP "SONGTI-BOLD.FONT", FP "SONGTI-BOLD.WID", 0, 0, 0, 0},
    [FONT_FACE_XINSONG]     = {FP "XINSONG.FONT", FP "XINSONG.WID", 0, 0, 0, 0},
};

static int load_sunplus_font(int font_face)
{
    struct sunplus_font *fnt = &sunplus_fonts[font_face];
    if (fnt->face)
        return 0;
    fnt->face = fopen(fnt->face_name, "rb");
    if (!fnt->face)
        return -5;
    fnt->header = malloc(sizeof(struct sunplus_font_header));
    if (!fnt->header)
        return -10;
    if (fread(fnt->header, sizeof(struct sunplus_font_header), 1, fnt->face) != 1) {
        fclose(fnt->face);
        fnt->face = 0;
        return -6;
    }
    int fd = open(fnt->width_name, O_RDONLY);
    if (!fd)
        return -7;
    struct stat st;
    fstat(fd, &st);
    fnt->width = malloc(st.st_size);
    if (!fnt->width || read(fd, fnt->width, st.st_size) != st.st_size) {
        close(fd);
        return -8;
    }
    close(fd);
    fnt->char_data = malloc(fnt->header->glyph_bytes);
    if (!fnt->char_data)
        return -9;
    return 0;
}

static int load_hzx_font(int font_size)
{
    int fd;
    struct stat st;
    if (font_size == FONT_SIZE_12 && !hzx12_font) {
        fd = open("/Rom/mw/fonts/CHINESE/HZX12", O_RDONLY);
        if (!fd) {
            return -3;
        }
        fstat(fd, &st);
        hzx12_font = malloc(st.st_size);
        if (!hzx12_font || read(fd, hzx12_font, st.st_size) != st.st_size) {
            return -4;
        }
        close(fd);
    }
    else if (font_size == FONT_SIZE_16 && !hzx16_font) {
        fd = open("/Rom/mw/fonts/CHINESE/HZX16", O_RDONLY);
        if (!fd) {
            return -3;
        }
        fstat(fd, &st);
        hzx16_font = malloc(st.st_size);
        if (!hzx16_font || read(fd, hzx16_font, st.st_size) != st.st_size) {
            return -4;
        }
        close(fd);
    }
    return 0;
}

int text_init(void)
{
    int fd = open("/Rom/mw/fonts/CHINESE/ASC12", O_RDONLY);
    if (!fd)
        return -1;
    struct stat st;
    fstat(fd, &st);
    asc12_font = malloc(st.st_size);
    if (!asc12_font || read(fd, asc12_font, st.st_size) != st.st_size)
        return -2;
    close(fd);

    fd = open("/Rom/mw/fonts/CHINESE/ASC16", O_RDONLY);
    if (!fd)
        return -1;
    fstat(fd, &st);
    asc16_font = malloc(st.st_size);
    if (!asc16_font || read(fd, asc16_font, st.st_size) != st.st_size)
        return -2;
    close(fd);

    return 0;
}

void text_free(void)
{
    if (asc12_font)
        free(asc12_font);
    asc12_font = 0;
    if (asc16_font)
        free(asc16_font);
    asc16_font = 0;
    if (hzx12_font)
        free(hzx12_font);
    hzx12_font = 0;
    if (hzx16_font)
        free(hzx16_font);
    hzx16_font = 0;

    int i;
    for (i = 0; i < FONT_FACE_MAX; i++) {
        struct sunplus_font *fnt = &sunplus_fonts[i];
        if (fnt->face)
            fclose(fnt->face);
        fnt->face = 0;
        if (fnt->width)
            free(fnt->width);
        fnt->width = 0;
        if (fnt->header)
            free(fnt->header);
        if (fnt->char_data)
            free(fnt->char_data);
    }
}

static int font_size = FONT_SIZE_12;
static int font_face = FONT_FACE_SONGTI;

void text_set_font_size(int fs)
{
    font_size = fs;
}
int text_get_font_size(void)
{
    return font_size;
}

void text_set_font_face(int face)
{
    font_face = face;
}

void text_set_fg_color(uint16_t color)
{
    fg_color = color;
}
void text_set_bg_color(uint16_t color)
{
    bg_color = color;
}

int text_draw_character(uint32_t codepoint, int x, int y)
{
    return text_draw_character_ex(gDisplayDev->getShadowBuffer(),
                             gDisplayDev->getWidth(), codepoint, x, y);
}

int text_draw_character_ex(uint16_t *buf, int width, uint32_t codepoint, int x, int y)
{
    uint16_t *fb = buf + width * y + x;
    int i, j;
    uint8_t *asc_font = 0;
    uint16_t *hzx_font;
    int hzx_double_width = 0;
    int char_width;
    
    if (codepoint >= 0x4e00 && codepoint < 0x10000) {
        hzx_double_width = 1;
        if (font_size == FONT_SIZE_12 || font_face == FONT_FACE_HZX) {
            /* convert unicode to Big5 codepoint */
            for (i = 0; i < (int)sizeof(hzk2uni) / 2; i++) {
                if (hzk2uni[i] == codepoint) {
                    codepoint = i;
                    hzx_double_width = 1;
                    break;
                }
            }
            if (i == sizeof(hzk2uni) / 2) {
                codepoint = 1;
                hzx_double_width = 0;
            }
        }
    }

    switch (font_size) {
        case FONT_SIZE_12:
            asc_font = &asc12_font[codepoint * font_size];
            hzx_font = &hzx12_font[codepoint * font_size];
            if (hzx_double_width) {
                load_hzx_font(font_size);
                char_width = 16;
            }
            else
                char_width = 8;
            break;
        case FONT_SIZE_16:
            if (font_face == FONT_FACE_HZX) {
                asc_font = &asc16_font[codepoint * font_size];
                hzx_font = &hzx16_font[codepoint * font_size];
                if (hzx_double_width) {
                    load_hzx_font(font_size);
                    char_width = 16;
                }
                else
                    char_width = 8;
            }
            else {
                load_sunplus_font(font_face);
                struct sunplus_font *fnt = &sunplus_fonts[font_face];
                struct sunplus_font_header *sp = fnt->header;
                fseek(fnt->face, (codepoint - sp->glyph_offset) * sp->glyph_bytes + sp->glyph_start, SEEK_SET);
                fread(fnt->char_data, sp->glyph_bytes, 1, fnt->face);
                hzx_font = fnt->char_data;
                char_width = fnt->width[codepoint];
                if (char_width < 0)
                    return -1;
            }
            break;
        default:
            return -1;
    }

    if ((font_face == FONT_FACE_HZX || font_size == FONT_SIZE_12) && !hzx_double_width) {
        for (i = 0; i < font_size; i++) {
            uint8_t line = asc_font[i];
            for (j = 0; j < char_width && x + j < width; j++) {
                fb[j] = (line & 0x80) ? fg_color : bg_color;
                line <<= 1;
            }
            fb += width;
        }
    }
    else {
        for (i = 0; i < font_size; i++) {
            uint16_t line = hzx_font[i];
            line = (line >> 8) | (line << 8);
            for (j = 0; j < char_width && x + j < width; j++) {
                fb[j] = (line & 0x8000) ? fg_color : bg_color;
                line <<= 1;
            }
            fb += width;
        }
    }

    return char_width;
}

int text_render(const char *t, int x, int y)
{
    return text_render_ex(gDisplayDev->getShadowBuffer(), gDisplayDev->getWidth(), t, x, y);
}

int text_render_centered(const char *t, int y)
{
    /* XXX: doesn't work correctly with double-width characters */
    return text_render(t, (gDisplayDev->getWidth() - strlen(t) * 8) / 2, y);
}

int text_render_ex(uint16_t *buf, int width, const char *t, int x, int y)
{
    int old_x = x;
    const uint8_t *text = (uint8_t *)t;
    while (*text) {
        uint32_t codepoint;
        if (*text >= 0x80) {
            if ((*text & 0x40) == 0) {
                /* invalid UTF-8 */
                text++;
                continue;
            }
            uint8_t first = text[0];
            uint8_t bytes = 0;
            while (first & 0x80) {
                bytes++;
                first <<= 1;
            }
            if (bytes > 6) {
                /* invalid UTF-8 */
                text++;
                continue;
            }
            first >>= bytes;
            int i;
            codepoint = first;
            for (i = 1; i < bytes; i++) {
                codepoint = (codepoint << 6) | (text[i] & 0x3f);
            }
            text += bytes;
        }
        else {
            codepoint = *text;
            text++;
        }
        int cw = text_draw_character_ex(buf, width, codepoint, x, y);
        if (cw > 0)	/* no error */
            x += cw;
        if (x >= width)
            break;
    }
    return x - old_x;
}
