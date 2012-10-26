#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libgame.h"
extern void libgame_detect_firmware_abi(void);
extern int _new_emu_abi;
extern int _has_frame_pointer;
char *heap_ending;

int main(int argc, char **argv)
{
  if (argc < 5)
    return -1;

  char *fw_name = argv[1];

  /* Some functions are needed as starting points. On a real device they
     are provided by the OS in a well-defined table. We don't have such
     a table when working with firmware images, so we require the user
     to specify a couple of addresses manually. */
#define SETFUN(n, x)  printf(#x " = %08x\n", (uint32_t)(x = (void *)strtoul(argv[n + 2], NULL, 0)));
  SETFUN(0, diag_printf)
  SETFUN(1, gfx_init)
  SETFUN(2, fs_open)
  SETFUN(3, fs_read)
  SETFUN(4, fs_write)

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

  printf("g_stEmuFuncs %08x (%s ABI)\n", (uint32_t)g_stEmuFuncs, _new_emu_abi ? "new" : "old");
  printf("gDisplayDev %08x\n", (uint32_t)gDisplayDev);
  printf("compiled %s frame pointer\n", _has_frame_pointer ? "with" : "without");
  printf("_ecos_close %08x\n", (uint32_t)_ecos_close);
  printf("_ecos_read %08x\n", (uint32_t)_ecos_read);
  printf("_ecos_write %08x\n", (uint32_t)_ecos_write);
  printf("_ecos_lseek %08x\n", (uint32_t)_ecos_lseek);
  printf("_ecos_fstat %08x\n", (uint32_t)_ecos_fstat);
  printf("_ecos_open %08x\n", (uint32_t)_ecos_open);
  printf("_ecos_opendir %08x\n", (uint32_t)_ecos_opendir);
  printf("_ecos_cyg_error_get_errno_p %08x\n", (uint32_t)_ecos_cyg_error_get_errno_p);
  printf("_ecos_cyg_fd_alloc %08x\n", (uint32_t)_ecos_cyg_fd_alloc);
  printf("_ecos_readdir_r %08x\n", (uint32_t)_ecos_readdir_r);
  printf("_ecos_readdir %08x\n", (uint32_t)_ecos_readdir);
  printf("_ecos_closedir %08x\n", (uint32_t)_ecos_closedir);
  printf("_ecos_stat %08x\n", (uint32_t)_ecos_stat);
  printf("_ecos_getcwd %08x\n", (uint32_t)_ecos_getcwd);
  printf("_ecos_chdir %08x\n", (uint32_t)_ecos_chdir);
  printf("_ecos_unlink %08x\n", (uint32_t)_ecos_unlink);
  printf("_ecos_mkdir %08x\n", (uint32_t)_ecos_mkdir);
  printf("_ecos_rmdir %08x\n", (uint32_t)_ecos_rmdir);
  printf("SPMP_SendSignal %08x\n", (uint32_t)SPMP_SendSignal);
  return 0;
}
