/* diag.c - firmware ABI detection diagnostic program
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

#include <stdio.h>
#include <string.h>

#include <libgame.h>

extern int _has_frame_pointer;
extern int _new_emu_abi;

#define FW_START 0x280000
#define FW_END 0xa00000

#define fs_fprintf(fd, x...) { \
  char buf[256]; int res; \
  sprintf(buf, x); \
  NativeGE_fsWrite(fd, buf, strlen(buf), &res); \
}

static int is_fw_pointer(void *addr)
{
    return (addr > (void *)FW_START && addr < (void *)FW_END);
}

static int is_valid_arm_insn(uint32_t insn, int unconditional_only)
{
    if (insn == 0xe12fff1e)     /* BX LR */
        return 1;
    if (/* illegal insns */
        (insn & 0x0e1000f0) == 0x000000d0 ||
        (insn & 0x0e1000f0) == 0x000000f0 ||
        (insn & 0x0e000010) == 0x06000000 ||
        /* unpredictable insns */
        (insn & 0x0e700000) == 0x08600000 ||
        (insn & 0x0e700000) == 0x08700000 ||
        /* these are legal, but unlikely to actually occur in legit code */
        insn == 0 || insn == 0xffffffffU ||
        /* in practice, no function ever starts with one or two
           conditional insns */
        ((insn & 0xf0000000) != 0xe0000000 && unconditional_only)
       ) {
        return 0;
    }
    return 1;
}

int main()
{
    libgame_init();

    int fd;
    int res;

    NativeGE_fsOpen("dump.bin", FS_O_CREAT | FS_O_WRONLY | FS_O_TRUNC, &fd);
    NativeGE_fsWrite(fd, (void *)FW_START, FW_END - FW_START, &res);
    NativeGE_fsClose(fd);

    NativeGE_fsOpen("diag.txt", FS_O_CREAT | FS_O_WRONLY | FS_O_TRUNC, &fd);
    fs_fprintf(fd, "system ID %d\n", libgame_system_id);

#define PP(x) fs_fprintf(fd, #x " %08x (%svalid)\n", (uint32_t)x, (is_fw_pointer(x) && is_valid_arm_insn(*((uint32_t *)(x)), 1)) ? "" : "in")
    fs_fprintf(fd, "g_stEmuFuncs %08x (%s ABI)\n", (uint32_t)g_stEmuFuncs,
               _new_emu_abi ? "new" : "old");
    fs_fprintf(fd, "gDisplayDev %08x\n", (uint32_t)gDisplayDev);
    fs_fprintf(fd, "compiled %s frame pointer\n", _has_frame_pointer ? "with" : "without");
    PP(_ecos_close);
    PP(_ecos_read);
    PP(_ecos_write);
    PP(_ecos_lseek);
    PP(_ecos_fstat);
    PP(_ecos_open);
    PP(_ecos_opendir);
    PP(_ecos_cyg_error_get_errno_p);
    PP(_ecos_cyg_fd_alloc);
    PP(_ecos_readdir_r);
    PP(_ecos_readdir);
    PP(_ecos_closedir);
    PP(_ecos_stat);
    PP(_ecos_getcwd);
    PP(_ecos_chdir);
    PP(_ecos_unlink);
    PP(_ecos_mkdir);
    PP(_ecos_rmdir);
    PP(_ecos_fsync);
    PP(SPMP_SendSignal);
    PP(cache_sync);
    PP(NativeGE_getKeyInput);
    /* We don't test every emu function, just one to make sure the right ABI
       version has been detected. */
    PP(emuIfSoundCleanup);
    PP(changeARMFreq);
    PP(GetArmCoreFreq);
    // NativeGE_fsClose(fd);

    if (_ecos_fsync)
        _ecos_fsync(fd);

    /* test opendir()/readdir()/closedir() */
    if (_ecos_opendir && _ecos_readdir && _ecos_closedir) {
        _ecos_DIR *dp = _ecos_opendir(".");
        if (!dp) {
            fs_fprintf(fd, "opendir failed\n");
            NativeGE_fsClose(fd);
            return 0;
        }
        struct _ecos_dirent *de;
        while ((de = _ecos_readdir(dp))) {
            fs_fprintf(fd, "file %s", de->d_name);
            struct _ecos_stat st;
            if (!_ecos_stat) {
                fs_fprintf(fd, "stat() not available\n");
            }
            else if (_ecos_stat(de->d_name, &st) < 0) {
                fs_fprintf(fd, "stat() failed\n");
                // NativeGE_fsClose(fd);
                // return 0;
            }
            else
                fs_fprintf(fd, " size %d dir %d reg %d\n", st.st_size, _ECOS_S_ISDIR(st.st_mode),
                           _ECOS_S_ISREG(st.st_mode));
        }
        _ecos_closedir(dp);
    }

    if (_ecos_fsync)
        _ecos_fsync(fd);

    /* test getcwd() */
    char dir[256];
    if (_ecos_getcwd) {
        _ecos_getcwd(dir, 256);
        fs_fprintf(fd, "cwd %s\n", dir);
    }
    if (_ecos_chdir) {
        _ecos_chdir("..");
        _ecos_getcwd(dir, 256);
        fs_fprintf(fd, "cwd after chdir %s\n", dir);
    }

    if (_ecos_fsync)
        _ecos_fsync(fd);

    if (gDisplayDev) {
        fs_fprintf(fd, "Framebuffer %08x, shadow buffer %08x\n",
                   gDisplayDev->getFrameBuffer(), gDisplayDev->getShadowBuffer());
        fs_fprintf(fd, "Width %d, Height %d\n", gDisplayDev->getWidth(),
                   gDisplayDev->getHeight());
        fs_fprintf(fd, "LCD format %08x\n", gDisplayDev->getBuffFormat());
    }

    if (_ecos_fsync)
        _ecos_fsync(fd);

    if (GetArmCoreFreq) {
        fs_fprintf(fd, "ARM frequency %d\n", GetArmCoreFreq());
    }

    NativeGE_fsClose(fd);
    return 0;
}
