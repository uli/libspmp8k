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

#include <string.h>
#include "libgame.h"

void **ftab;

//extern char *heap_ending;
extern int heap_ending;

void (*diag_printf) (char *fmt, ...);
int *g_onoff_p;

int (*MCatchInitGraph) (void);
int (*MCatchSetFrameBuffer) (int width, int height);
int (*MCatchSetDisplayScreen) (gfx_rect_t *rect);
int (*MCatchSetCameraMode) (int mode);
int (*MCatchSetFGColor) (uint32_t *color);
uint32_t (*MCatchGetFGColor) ();
int (*MCatchSetColorROP) (uint32_t rop);
int (*MCatchSetAlphaBld) (uint8_t src_alpha, uint8_t dest_alpha);
int (*MCatchGetAlphaBld) (uint8_t *src_alpha, uint8_t *dest_alpha);
int (*MCatchFillRect) (gfx_rect_t *rect);
int (*MCatchEnableFeature) (uint32_t feature);
int (*MCatchFlush) ();
int (*MCatchPaint) ();
int (*MCatchLoadImage) (gfx_loadimg_t *loadimg, uint8_t *imgid);
int (*MCatchFreeImage) (uint8_t img_id);
int (*MCatchBitblt) (uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);
int (*MCatchSprite) (uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);

int (*NativeGE_initRes) (int val, void *res_table);
int (*NativeGE_getRes) (char *filename, void *res_info);
int (*NativeGE_playRes) (uint8_t res_type, int flags, void *res_info);
int (*NativeGE_stopRes) (int arg);

int (*NativeGE_fsOpen) (const char *filename, int flags, int *fd);
int (*NativeGE_fsRead) (int fd, const void *buf, size_t count, int *result);
int (*NativeGE_fsWrite) (int fd, const void *buf, size_t count, int *result);
int (*NativeGE_fsClose) (int fd);
uint64_t (*NativeGE_fsSeek) (int fd, int offset, int whence);

uint32_t (*NativeGE_getTime) ();        // returns system ticks equivalent
void (*NativeGE_getKeyInput4Ntv) (key_data_t * keys);   // uint64_t *keys);

#define	FUNC(n)		*(ftab + (n >> 2))

#define FW_START 0x280000
#define FW_START_P ((uint32_t *)FW_START)
#define FW_END 0xa00000
#define FW_END_P ((uint32_t *)FW_END)

void **g_stEmuFuncs = 0;
emu_apis_t *g_stEmuAPIs = 0;
display_dev_t *gDisplayDev = 0;
int (*_ecos_close) (int fd) = 0;
int (*_ecos_read) (int fd, void *buf, unsigned int count) = 0;
int (*_ecos_write) (int fd, const void *buf, unsigned int count) = 0;
int (*_ecos_lseek) (int fd, int offset, int whence) = 0;
int (*_ecos_fstat) (int fd, struct _ecos_stat * buf) = 0;
int (*_ecos_open) (const char *pathname, int flags, int mode) = 0;
_ecos_DIR *(*_ecos_opendir) (const char *name) = 0;
void *_ecos_cyg_error_get_errno_p = 0;
void *_ecos_cyg_fd_alloc = 0;
struct _ecos_dirent *(*_ecos_readdir) (_ecos_DIR * dirp) = 0;
int (*_ecos_readdir_r) (_ecos_DIR * dirp, struct _ecos_dirent * entry,
                        struct _ecos_dirent ** result) = 0;
int (*_ecos_closedir) (_ecos_DIR * dirp) = 0;
int (*_ecos_stat) (const char *path, struct _ecos_stat * buf) = 0;
char *(*_ecos_getcwd) (char *buf, size_t size) = 0;
int (*_ecos_chdir) (const char *path) = 0;
int (*_ecos_unlink) (const char *pathname) = 0;
int (*_ecos_rmdir) (const char *pathname) = 0;
int (*_ecos_mkdir) (const char *pathname, _ecos_mode_t mode) = 0;
int (*_ecos_fsync) (int fd) = 0;
void (*cyg_thread_delay) (uint64_t /* cyg_tick_count_t */ delay) = 0;

uint16_t (*SPMP_SendSignal) (uint16_t cmd, void *data, uint16_t size) = 0;
void (*cache_sync) (void) = 0;

/* int (*MCatchStoreImage) (void) = 0; doesn't do anything */
/* int (*MCatchDecodeImageFromCard) (void) = 0; doesn't do anything */
int (*MCatchGetColorROP) (uint32_t *rop) = 0;
int (*MCatchSetBitPlaneMask) (int read_write, uint16_t mask) = 0;
int (*MCatchGetBitPlaneMask) (int read_write, uint16_t *mask) = 0;
int (*MCatchGetDisplayScreen) (gfx_rect_t *) = 0;
int (*MCatchSetRectClip) (gfx_rect_t *) = 0;
int (*MCatchGetRectClip) (gfx_rect_t *) = 0;
int (*MCatchSetStyleMask) (gfx_rect_t *) = 0;
int (*MCatchGetStyleMask) (gfx_rect_t *) = 0;
int (*MCatchSetLineMask) (gfx_rect_t *) = 0;
int (*MCatchGetLineMask) (gfx_rect_t *) = 0;
int (*MCatchDisableFeature) (int) = 0;
int (*MCatchSetStyleLine) (uint8_t, uint8_t) = 0;
/* int (*MCatchPreviewColorkey) (void) = 0; doesn't do anything */
int (*MCatchGetFrameBuffer) (uint16_t *width, uint16_t *height) = 0;
int (*MCatchSetMutableImage) (uint8_t) = 0;
int (*MCatchSetPerPixelAlphaEq) (uint8_t) = 0;  /* 0 or 1 */
int (*MCatchSetTransformation) (gfx_point2d_t *, int /* 0 to 7 */ ) = 0;
int (*MCatchQueryImage) (uint8_t, uint8_t /* 1 to 3 */ ) = 0;
int (*MCatchEnableDoubleBuffer) (int /* 0 or 1 */ ) = 0;
int (*MCatchGradientFill) (gfx_rect_t *, uint16_t[6], uint32_t[2]) = 0;
/* int (*MCatchUpdateScreen) (void) = 0; doesn't do anything (except produce debug output) */
int (*MCatchShowFont) (gfx_point2d_t *, int, uint8_t /* < 0x18 */ , uint8_t /* < 0x18 */ ) = 0;
int (*MCatchModifyPalette) (uint8_t, uint8_t, uint8_t /* size */ , void * /* data */ ) = 0;
void (*NativeGE_pauseRes) (uint8_t) = 0;
void (*NativeGE_resumeRes) (uint8_t) = 0;
int (*NativeGE_writeRecord) (const char *pathname, void *buf, uint8_t flags, _ecos_off_t offset,
                             _ecos_size_t count) = 0;
int (*NativeGE_readRecord) (const char *pathname, void *buf, uint8_t flags, _ecos_off_t offset,
                            _ecos_size_t count) = 0;
/* void (*NativeGE_showFPS) (void) = 0; doesn't do anything */
int (*NativeGE_gameExit) (void) = 0;
/* int (*NativeGE_getTPEvent) (void) = 0; doesn't do anything */
/* char (*NativeGE_setTPClickArea) (void) = 0; doesn't do anything */

int (*NativeGE_gamePause) (void) = 0;
int (*NativeGE_gameResume) (void) = 0;

int _has_frame_pointer = -1;    /* required to find function entry points */
int _new_emu_abi = -1;

int (*emuIfGraphInit) (graph_params_t *);
int (*emuIfGraphShow) (void);
int (*emuIfGraphChgView) (graph_params_t *);
int (*emuIfGraphCleanup) (void);
uint32_t (*emuIfSoundInit) (sound_params_t *params);
uint32_t (*emuIfSoundPlay) (sound_params_t *params);
uint32_t (*emuIfSoundCleanup) (sound_params_t *params);

int (*emuIfunknown0c) (void *) = 0;     /* sets the source buffer? */
int (*emuIfKeyInit) (keymap_t *) = 0;
uint32_t (*emuIfKeyGetInput) (keymap_t *) = 0;
int (*emuIfKeyCleanup) (keymap_t *) = 0;
uint32_t (*emuIfGetCurTime) (void) = 0; /* could be uint64_t, not sure */
void (*emuIfTimeDelay) (uint32_t) = 0;
int (*emuIfFsFileOpen) (const char *pathname, uint32_t flags) = 0;
uint32_t (*emuIfFsFileGetSize) (int fd) = 0;
int (*emuIfFsFileWrite) (int fd, const void *buf, uint32_t count) = 0;
int (*emuIfFsFileRead) (int fd, void *buf, uint32_t count) = 0;
int (*emuIfFsFileGetChar) (int fd) = 0;
int (*emuIfFsFileSeek) (int fd, _ecos_off_t offset, uint8_t whence) = 0;
_ecos_off_t(*emuIfFsFileCurPos) (int fd) = 0;
int (*emuIfFsFileClose) (int fd) = 0;
int (*emuIfkgbCevaLoader) (void) = 0;
int (*emuIfcevaImageUnload) (void) = 0;
int (*emuIfunknown5c) (void *) = 0;
int (*emuIfunknown60) (void *) = 0;
int (*emuIfunknown64) (void) = 0;
void (*emuIfunknown68) (void *) = 0;
int (*emuIfunknown6c) (int, int, int) = 0;
void (*emuIfunknown74) (int, void *, int) = 0;
int (*emuIfunknown78) (void) = 0;

int (*GetArmCoreFreq) (void) = 0;
int (*changeARMFreq) (int mhz) = 0;     /* A1000 accepts 5 - 297 MHz */

void (*hal_clock_read) (uint32_t *us) = 0;
uint64_t (*cyg_current_time) (void) = 0;

/* returns true if this is a pointer into the firmware area */
static int is_ptr(uint32_t val)
{
    return (val >= FW_START && val < FW_END);
}

/* returns true if val is an unconditional BL insn */
static int is_branch_link(uint32_t val)
{
    return (val & 0xff000000U) == 0xeb000000U;
}

/* returns the target of the branch at loc */
static void *branch_address(uint32_t *loc)
{
    int32_t diff = ((int32_t)(*loc << 8)) >> 8;
    return (void *)(loc + diff + 2);
}

/* returns the address of the next unconditional BL insn */
static uint32_t *next_bl(uint32_t *loc)
{
    uint32_t *head;
    for (head = loc; head < loc + 100; head++) {
        if (is_branch_link(*head)) {
            return head;
        }
    }
    return 0;
}

/* returns the target of the next unconditional BL insn */
static uint32_t *next_bl_target(uint32_t *loc)
{
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
static uint32_t is_prolog(uint32_t val)
{
    return (_has_frame_pointer && val == 0xe1a0c00d /* MOV R12, SP */ ) ||
        (!_has_frame_pointer && (val & 0xffff0000U) == 0xe92d0000 /* STMFD SP!, {...} */ );
}

/* Find a filesystem function via the filesystem method it uses. */
static uint32_t *find_fs_function(int index)
{
    uint32_t *head;
    for (head = FW_START_P; head < FW_END_P; head++) {
        if (is_prolog(*head)) {
            if (head == (void *)_ecos_open) {
                /* open() is misdetected as fsync() on the JXD100 because it
                   seems to have an inlined copy of (or just very similar
                   code to) fsync(). */
                head++;         /* skip second STMFD */
                continue;
            }
            uint32_t *subhead;
            int errno_p_found = 0;
            /* skip two insns (some functions start with two(!) STMFDs) */
            for (subhead = head + 2; subhead < FW_END_P; subhead++) {
                if (is_prolog(*subhead))        /* start of next function */
                    break;
                if (is_branch_link(*subhead) &&
                    branch_address(subhead) == _ecos_cyg_error_get_errno_p)
                    errno_p_found = 1;
                else if (errno_p_found &&
                         /* MOV PC, [Rx, #offset] */
                         (*subhead & 0xfff0ffffU) == (0xe590f000U + index)) {
                    return head;
                }
            }
        }
    }
    return 0;
}

/* Is this a PC-relative load? */
static int is_ldr_pc(uint32_t val)
{
    return (val & 0xffff0000U) == 0xe59f0000U;
}

/* Get address of PC-relative load. */
static void *ldr_pc_address(uint32_t *head)
{
    return (void *)(*(head + (*head & 0xfff) / 4 + 2));
}

/* Check if mem points to a given string within firmware bounds. */
static int string_starts_with(uint32_t *mem, const char *string)
{
    if (mem < FW_START_P || mem > FW_END_P)
        return 0;
    return !strncmp((const char *)mem, string, strlen(string));
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
    start = (uint32_t *)MCatchInitGraph;
    for (head = start; head < start + 100; head++) {
        if (is_branch_link(*head)) {
            getGameBuffWidth_found = 1;
        }
        if (getGameBuffWidth_found && is_ldr_pc(*head)) {
            gDisplayDev = (display_dev_t *) ldr_pc_address(head);
            break;
        }
    }

    /* Find eCos close()/closedir().  */
    if (g_stEmuFuncs) {
        _ecos_close = (void *)next_bl_target(g_stEmuFuncs[_new_emu_abi ? 20 : 19]);

        /* In theory, closedir() is different from close(), but in practice it
           simply forwards its argument to close(). Since it's a very small
           function, it's really tricky to find, so we don't bother and just
           use close() in its place. */
        _ecos_closedir = (void *)_ecos_close;

        /* Determine if built with frame pointer. */
        _has_frame_pointer = 0;
        if (*((uint32_t *)_ecos_close) == 0xe1a0c00d /* MOV R12, SP */ ) {
            _has_frame_pointer = 1;
        }
    }

    /* Find eCos read/write */
    start = (uint32_t *)NativeGE_fsRead;
    for (head = start; head < start + 200; head++) {
        if (is_branch_link(*head)) {
            if (branch_address(head) == (uint32_t *)diag_printf)
                continue;
            _ecos_read = branch_address(head);
            break;
        }
    }
    start = (uint32_t *)NativeGE_fsWrite;
    for (head = start; head < start + 200; head++) {
        if (is_branch_link(*head)) {
            /* skip debug output calls */
            if (branch_address(head) == (uint32_t *)diag_printf)
                continue;
            _ecos_write = branch_address(head);
            break;
        }
    }

    if (g_stEmuFuncs) {
        /* Find lseek, fstat */
        _ecos_lseek = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x4c : 0x48) / 4]);
        _ecos_fstat = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x38 : 0x34) / 4]);
    }

    /* Find open */
    if (_ecos_fstat) {
        start = (uint32_t *)NativeGE_fsOpen;
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
    }

    /* Find cyg_fd_alloc, cyg_error_get_errno_p; they are needed to find
       opendir and readdir */
    if (_ecos_open) {
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
    }

    /* There are only two functions that use cyg_fd_alloc(), and only
       opendir() calls it first. */
    if (_has_frame_pointer != -1 && _ecos_cyg_fd_alloc) {
        for (head = FW_START_P; head < FW_END_P; head++) {
            if (is_prolog(*head)) {
                if (next_bl_target(head) == _ecos_cyg_fd_alloc) {
                    _ecos_opendir = (void *)head;
                    break;
                }
            }
        }
    }

    if (_has_frame_pointer != -1 && _ecos_read) {
        /* Find readdir_r() on the assumption that it's the only function
           reading a block of 0x104 bytes. */
        for (head = FW_START_P; head < FW_END_P; head++) {
            if (is_prolog(*head)) {
                uint32_t *subhead;
                for (subhead = head + 1; subhead < head + 100 && !is_prolog(*subhead); subhead++) {
                    if (*subhead == 0xe3a02f41 /* MOV R2, #0x104 */  &&
                        next_bl_target(subhead) == (void *)_ecos_read) {
                        _ecos_readdir_r = (void *)head;
                        goto out;
                    }
                }
            }
        }
    }
out:

    /* readdir() calls readdir_r(), but so does another function, so we make
       sure we have the right one by checking for a call to
       cyg_error_get_errno_p(). */
    if (_has_frame_pointer != -1 && _ecos_readdir_r && _ecos_cyg_error_get_errno_p) {
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
    }

    if (g_stEmuFuncs)
        SPMP_SendSignal = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x28 : 0x24) / 4]);

    /* Find various file functions */
    if (_has_frame_pointer != -1 && _ecos_cyg_error_get_errno_p) {
        _ecos_fsync = (void *)find_fs_function(0x14);
        _ecos_unlink = (void *)find_fs_function(0x18);
        _ecos_mkdir = (void *)find_fs_function(0x1c);
        _ecos_rmdir = (void *)find_fs_function(0x20);
        _ecos_stat = (void *)find_fs_function(0x34);
        /* This could go wrong because two other exported eCos functions use the
           getinfo() method at offset 0x38 (pathconf() and cyg_fs_get_attrib()),
           but we seem to be lucky: I could not find any trace of them in the
           firmwares investigated. */
        _ecos_getcwd = (void *)find_fs_function(0x38);

        _ecos_chdir = (void *)find_fs_function(0x30);
    }


    /* Find cache_sync(), the last function called by MCatchPaint(). */
    if (_has_frame_pointer != -1) {
        start = (uint32_t *)MCatchPaint;
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
    }

    /* Find changeARMFreq. */
    if (_has_frame_pointer != -1) {
        for (head = FW_START_P; head < FW_END_P; head++) {
            if (is_prolog(*head)) {
                uint32_t *subhead = next_bl(head);
                if (!subhead)
                    continue;
                GetArmCoreFreq = branch_address(subhead);
                subhead = next_bl(subhead + 1);
                if (!subhead)
                    continue;
                if (branch_address(subhead) != GetArmCoreFreq) {
                    GetArmCoreFreq = 0;
                    continue;
                }
                uint32_t *subsubhead = next_bl(subhead + 1);
                if (!subsubhead)
                    continue;
                if (branch_address(subsubhead) != diag_printf) {
                    GetArmCoreFreq = 0;
                    continue;
                }
                for (; subhead < subsubhead; subhead++) {
                    if (is_ldr_pc(*subhead) &&
                        string_starts_with(ldr_pc_address(subhead), "[changeARMFreq]")) {
                        changeARMFreq = (void *)head;
                        goto out2;
                    }
                }
                GetArmCoreFreq = 0;
            }
        }
    }
out2:

    if (g_stEmuFuncs)
        cyg_current_time = (void *)next_bl_target(g_stEmuFuncs[(_new_emu_abi ? 0x2c : 0x28) / 4]);

    /* Find hal_clock_read(). We search for the function that calls
       getTimerVLR_WDT(), which is a very small function, so we match for the 
       first two instructions. This is far from perfect, but so far only two
       varieties have been sighted. */
    if (_has_frame_pointer != -1) {
        for (head = FW_START_P; head < FW_END_P; head++) {
            if (is_prolog(*head)) {
                uint32_t *subhead = next_bl_target(head);
                if (subhead &&
                    (/* MOV R3, #0x1000; ADD R3, R3, #0x9000000c */
                     (subhead[0] == 0xe3a03a01 && subhead[1] == 0xe28332c9) ||
                     /* MOV R0, R0, LSL#5; LDR R3, =0x9000100c */
                     (subhead[0] == 0xe1a00280 && is_ldr_pc(subhead[1]) &&
                      ldr_pc_address(subhead + 1) == (void *)0x9000100c))) {
                    hal_clock_read = (void *)head;
                    break;
                }
            }
        }
    }
    
    /* Find g_onoff (debug flag). */
    start = (uint32_t *)diag_printf;
    for (head = start; head < start + 50; head++) {
        if (is_ldr_pc(*head)) {
            g_onoff_p = ldr_pc_address(head);
            break;
        }
    }

    /* Find g_stEmuAPIs (event callback structure). */
    if (g_stEmuFuncs) {
        for (head = FW_START_P; head < FW_END_P; head++) {
            if (is_ldr_pc(*head) && ldr_pc_address(head) == g_stEmuFuncs) {
                head++;
                if (is_ldr_pc(*head) &&
                    (head[1] & 0xffff0fffU) == 0xe3a0060aU /* MOV Rx, #0xa00000 */) {
                    g_stEmuAPIs = ldr_pc_address(head);
                    break;
                }
            }
        }
    }
    if (g_stEmuAPIs) {
        NativeGE_gamePause = g_stEmuAPIs->pause;
        NativeGE_gameResume = g_stEmuAPIs->resume;
    }

    return;
}

uint64_t libgame_utime(void)
{
    uint64_t t;
    uint32_t u;
    if (hal_clock_read) {
        do {
            t = cyg_current_time();
            hal_clock_read(&u);
        } while (t != cyg_current_time());
        return t * 10000 + u;
    }
    else
        return cyg_current_time() * 10000;
}

#ifndef TEST_BUILD
static
#endif
void libgame_assign_emuif(void)
{
    emuIfGraphInit = EMU_FUNC_ENTRY(0x00);
    emuIfGraphShow = EMU_FUNC_ENTRY(0x04);
    emuIfGraphChgView = EMU_FUNC_ENTRY(0x08);
    if (_new_emu_abi)
        emuIfunknown0c = EMU_FUNC_ENTRY(0x0c);
    emuIfGraphCleanup = EMU_FUNC_ENTRY(_new_emu_abi ? 0x10 : 0x0c);
    emuIfSoundInit = EMU_FUNC_ENTRY(_new_emu_abi ? 0x14 : 0x10);
    emuIfSoundPlay = EMU_FUNC_ENTRY(_new_emu_abi ? 0x18 : 0x14);
    emuIfSoundCleanup = EMU_FUNC_ENTRY(_new_emu_abi ? 0x1c : 0x18);
    emuIfKeyInit = EMU_FUNC_ENTRY(_new_emu_abi ? 0x20 : 0x1c);
    emuIfKeyGetInput = EMU_FUNC_ENTRY(_new_emu_abi ? 0x24 : 0x20);
    emuIfKeyCleanup = EMU_FUNC_ENTRY(_new_emu_abi ? 0x28 : 0x24);
    emuIfGetCurTime = EMU_FUNC_ENTRY(_new_emu_abi ? 0x2c : 0x28);
    emuIfTimeDelay = EMU_FUNC_ENTRY(_new_emu_abi ? 0x30 : 0x2c);
    emuIfFsFileOpen = EMU_FUNC_ENTRY(_new_emu_abi ? 0x34 : 0x30);
    emuIfFsFileGetSize = EMU_FUNC_ENTRY(_new_emu_abi ? 0x38 : 0x34);
    emuIfFsFileWrite = EMU_FUNC_ENTRY(_new_emu_abi ? 0x3c : 0x38);
    emuIfFsFileRead = EMU_FUNC_ENTRY(_new_emu_abi ? 0x40 : 0x3c);
    emuIfFsFileGetChar = EMU_FUNC_ENTRY(_new_emu_abi ? 0x44 : 0x40);
    emuIfFsFileSeek = EMU_FUNC_ENTRY(_new_emu_abi ? 0x48 : 0x44);
    emuIfFsFileCurPos = EMU_FUNC_ENTRY(_new_emu_abi ? 0x4c : 0x48);
    emuIfFsFileClose = EMU_FUNC_ENTRY(_new_emu_abi ? 0x50 : 0x4c);
    if (_new_emu_abi) {
        emuIfkgbCevaLoader = EMU_FUNC_ENTRY(0x54);
        emuIfcevaImageUnload = EMU_FUNC_ENTRY(0x58);
        emuIfunknown5c = EMU_FUNC_ENTRY(0x5c);
        emuIfunknown60 = EMU_FUNC_ENTRY(0x60);
        emuIfunknown64 = EMU_FUNC_ENTRY(0x64);
        emuIfunknown68 = EMU_FUNC_ENTRY(0x68);
        emuIfunknown6c = EMU_FUNC_ENTRY(0x6c);
        /* diag_printf 0x70 */
        emuIfunknown74 = EMU_FUNC_ENTRY(0x74);
        emuIfunknown78 = EMU_FUNC_ENTRY(0x78);
    }
}

int libgame_system_id;

static void libgame_detect_system(void)
{
    if (_ecos_stat) {
        struct _ecos_stat st;
        if (!_ecos_stat("/Rom/IMAGE/GAME/SPDF.BIN", &st))
            libgame_system_id = SYS_JXD_100;
        else if (!_ecos_stat("/Rom/RESOURCE/ARECORD/DEFBTN.BMP", &st))
            libgame_system_id = SYS_JXD_V3;
        else if (!_ecos_stat("/Rom/RESOURCE/XML/APPHOMENOMONTH.XML", &st))
            libgame_system_id = SYS_JXD_1000;   /* XXX: also A1000, need to
                                                   come up with sth better */
        else if (!_ecos_stat("/Rom/HOME/IMAGE/EXPLORER.BMP", &st) ||
                 !_ecos_stat("/Rom/HOME/IMAGE/HNWFNYPM_EXPLORER.BMP", &st))
            libgame_system_id = SYS_MMM;
        else
            libgame_system_id = SYS_UNKNOWN;
    }
    else
        libgame_system_id = SYS_UNKNOWN;
}

void libgame_init(void)
{
    // setup function pointers
    diag_printf = FUNC(0x04);
    MCatchFlush = FUNC(0xc);
    MCatchPaint = FUNC(0x10);
    MCatchLoadImage = FUNC(0x14);
    MCatchFreeImage = FUNC(0x18);
    /* MCatchStoreImage = FUNC(0x1c); doesn't do anything */
    /* MCatchDecodeImageFromCard = FUNC(0x28); doesn't do anything */
    MCatchInitGraph = FUNC(0x38);
    MCatchSetColorROP = FUNC(0x3c);
    MCatchGetColorROP = FUNC(0x40);
    MCatchSetFGColor = FUNC(0x44);
    MCatchGetFGColor = FUNC(0x48);
    MCatchSetBitPlaneMask = FUNC(0x4c);
    MCatchGetBitPlaneMask = FUNC(0x50);
    MCatchSetDisplayScreen = FUNC(0x54);
    MCatchGetDisplayScreen = FUNC(0x58);
    MCatchSetRectClip = FUNC(0x5c);
    MCatchGetRectClip = FUNC(0x60);
    MCatchSetStyleMask = FUNC(0x64);
    MCatchGetStyleMask = FUNC(0x68);
    MCatchSetLineMask = FUNC(0x6c);
    MCatchGetLineMask = FUNC(0x70);
    MCatchSetAlphaBld = FUNC(0x74);
    MCatchGetAlphaBld = FUNC(0x78);
    MCatchEnableFeature = FUNC(0x7c);
    MCatchDisableFeature = FUNC(0x80);
    MCatchSetStyleLine = FUNC(0x84);
    /* MCatchPreviewColorkey = FUNC(0x88); doesn't do anything */
    MCatchSetCameraMode = FUNC(0x8c);
    MCatchSetFrameBuffer = FUNC(0x90);
    MCatchGetFrameBuffer = FUNC(0x94);
    MCatchSetMutableImage = FUNC(0x98);
    MCatchSetPerPixelAlphaEq = FUNC(0x9c);
    MCatchSetTransformation = FUNC(0xa0);
    MCatchQueryImage = FUNC(0xa4);
    MCatchEnableDoubleBuffer = FUNC(0xa8);
    MCatchBitblt = FUNC(0xB4);
    MCatchSprite = FUNC(0xB8);
    MCatchGradientFill = FUNC(0xc0);
    MCatchFillRect = FUNC(0xc4);
    /* MCatchUpdateScreen = FUNC(0xc8); doesn't do anything */
    MCatchShowFont = FUNC(0xcc);
    MCatchModifyPalette = FUNC(0xd0);

    NativeGE_initRes = FUNC(0xD4);
    NativeGE_getRes = FUNC(0xD8);
    NativeGE_playRes = FUNC(0xDC);
    NativeGE_pauseRes = FUNC(0xE0);
    NativeGE_resumeRes = FUNC(0xE4);
    NativeGE_stopRes = FUNC(0xE8);

    NativeGE_writeRecord = FUNC(0xec);
    NativeGE_readRecord = FUNC(0xf0);

    NativeGE_getKeyInput4Ntv = FUNC(0x100);
    /* NativeGE_showFPS = FUNC(0x108); doesn't do anything */
    cyg_thread_delay = FUNC(0x11c);
    NativeGE_getTime = FUNC(0x124);
    NativeGE_gameExit = FUNC(0x130);
    /* NativeGE_getTPEvent = FUNC(0x134); doesn't do anything */
    /* NativeGE_setTPClickArea = FUNC(0x138); doesn't do anything */

    NativeGE_fsOpen = FUNC(0x13C);
    NativeGE_fsRead = FUNC(0x140);
    NativeGE_fsWrite = FUNC(0x144);
    NativeGE_fsClose = FUNC(0x148);
    NativeGE_fsSeek = FUNC(0x14C);

    heap_ending = 0;

    libgame_detect_firmware_abi();
    libgame_assign_emuif();
    libgame_detect_system();
}
