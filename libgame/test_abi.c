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

int main(int argc, char **argv)
{
  if (argc < 7)
    return -1;

  char *fw_name = argv[1];

  /* Some functions are needed as starting points. On a real device they
     are provided by the OS in a well-defined table. We don't have such
     a table when working with firmware images, so we require the user
     to specify a couple of addresses manually. */
#define SETFUN(n, x)  printf(#x " = %08x\n", (uint32_t)(x = (void *)strtoul(argv[n + 2], NULL, 0)));
  SETFUN(0, diag_printf)
  SETFUN(1, MCatchInitGraph)
  SETFUN(2, NativeGE_fsOpen)
  SETFUN(3, NativeGE_fsRead)
  SETFUN(4, NativeGE_fsWrite)
  SETFUN(5, MCatchPaint)
  SETFUN(6, NativeGE_getKeyInput4Ntv)

  /* Load the firmware image to memory at 0x280000. We assume that the
     code in the firmware file starts at offset 0x4c. */
  int fd = open(fw_name, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }
  struct stat st;
  fstat(fd, &st);
  if (mmap((void *)0x280000, 0xa00000 - 0x280000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0) == MAP_FAILED) {
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

#define PP(x) printf(#x " %08x\n", (uint32_t)x);

  printf("g_stEmuFuncs %08x (%s ABI)\n", (uint32_t)g_stEmuFuncs, _new_emu_abi ? "new" : "old");
  PP(gDisplayDev)
  printf("compiled %s frame pointer\n", _has_frame_pointer ? "with" : "without");
  PP(_ecos_close)
  PP(_ecos_read)
  PP(_ecos_write)
  PP(_ecos_lseek)
  PP(_ecos_fstat)
  PP(_ecos_open)
  PP(_ecos_opendir)
  PP(_ecos_cyg_error_get_errno_p)
  PP(_ecos_cyg_fd_alloc)
  PP(_ecos_readdir_r)
  PP(_ecos_readdir)
  PP(_ecos_closedir)
  PP(_ecos_stat)
  PP(_ecos_getcwd)
  PP(_ecos_chdir)
  PP(_ecos_unlink)
  PP(_ecos_mkdir)
  PP(_ecos_rmdir)
  PP(_ecos_fsync)
  PP(SPMP_SendSignal)
  PP(cache_sync)
  PP(NativeGE_getKeyInput)
  /* We don't test every emu function, just one to make sure the right ABI
     version has been detected. */
  PP(emuIfSoundCleanup)
  return 0;
}
