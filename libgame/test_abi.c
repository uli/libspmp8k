/* test_abi.c - firmware ABI detection test routines
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

/* This program will only work on little-endian systems that allow mapping
   memory to fixed address 0x280000.  This does not include, for instance,
   Linux/AMD64 systems, which is why the run_test script compiles this with
   "-m32".  */

#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libgame.h"
extern void libgame_detect_firmware_abi(void);
extern void libgame_assign_emuif(void);
extern int _new_emu_abi;
extern int _has_frame_pointer;
char *heap_ending;
uint32_t _ram_end;

extern int ftab_length;
void **_gFunTable;

int main(int argc, char **argv)
{
    if (argc < 7)
        return -1;

    char *fw_name = argv[1];

    /* Some functions are needed as starting points. On a real device they
       are provided by the OS in a well-defined table. We don't have such a
       table when working with firmware images, so we require the user to
       specify a couple of addresses manually. */
#define SETFUN(n, x)  printf(#x " = %08x\n", (uint32_t)(x = (void *)strtoul(argv[n + 2], NULL, 0)))
    SETFUN(0, diag_printf);
    SETFUN(1, MCatchInitGraph);
    SETFUN(2, NativeGE_fsOpen);
    SETFUN(3, NativeGE_fsRead);
    SETFUN(4, NativeGE_fsWrite);
    SETFUN(5, MCatchPaint);
    SETFUN(6, NativeGE_getKeyInput4Ntv);
    SETFUN(7, _gFunTable);
    SETFUN(8, NativeGE_gamePause);

    /* Load the firmware image to memory at 0x280000. We assume that the code 
       in the firmware file starts at offset 0x4c. */
    int fd = open(fw_name, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    struct stat st;
    fstat(fd, &st);
    if (mmap((void *)0x280000, 0xa00000 - 0x280000, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    lseek(fd, 0x4c, SEEK_SET);
    if (read(fd, (void *)0x280000, st.st_size - 0x4c) < 0) {
        perror("read");
        return -1;
    }

    libgame_detect_firmware_abi();
    libgame_assign_emuif();

#define PP(x) printf(#x " %08x\n", (uint32_t)x)

    printf("gFunTable length %d\n", ftab_length);
    printf("g_stEmuFuncs %08x (%s ABI)\n", (uint32_t)g_stEmuFuncs, _new_emu_abi ? "new" : "old");
    PP(gDisplayDev);
    printf("compiled %s frame pointer\n", _has_frame_pointer ? "with" : "without");
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
    /* We don't test every emu function, just one to make sure the right ABI
       version has been detected. */
    PP(emuIfSoundCleanup);
    PP(changeARMFreq);
    PP(GetArmCoreFreq);
    PP(hal_clock_read);
    PP(cyg_current_time);
    PP(g_onoff_p);
    PP(g_stEmuAPIs);
    PP(_gameMaxBufferSize_p);
    PP(cyg_thread_create);
    PP(cyg_thread_resume);
    PP(cyg_thread_exit);
    PP(cyg_thread_delete);
    PP(cyg_thread_suspend);
    PP(cyg_thread_self);
    PP(cyg_thread_set_priority);
    PP(cyg_thread_get_priority);
    PP(cyg_scheduler_lock);
    PP(cyg_scheduler_unlock);
    PP(cyg_mutex_destroy);
    PP(cyg_mutex_init);
    PP(cyg_mutex_lock);
    PP(cyg_mutex_unlock);
    PP(cyg_mutex_release);
    PP(cyg_mutex_set_protocol);
    PP(cyg_mutex_set_ceiling);
    PP(readBlksWithLock_SD);
    PP(writeBlksWithLock_SD);
    PP(_rwstor2_msg_55a_call);
    PP(_rwstor2_msg_551_call);
    return 0;
}
