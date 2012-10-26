#include <stdio.h>
#include <string.h>

#include "libgame.h"
#include "gfx_types.h"
#include "font.h"

extern void **ftab;
uint16_t fbuff[320*240];
uint16_t *(*int_get_framebuffer)(void);
uint16_t *(*int_get_shadowbuffer)(void);
void (*set_lcd_fb_format)(int);
int (*getLCDBuffFormat)(void);
int (*getLCDWidth)(void);
int (*getLCDHeight)(void);
uint16_t *(*getLCDShadowBuffer)(void);
uint16_t *(*getLCDFrameBuffer)(void);
void (*setLCDFrameBuffer)(uint16_t *fb); // educated guess
void (*lcd_flip)(void);
void (*lcd_clear)(void); // actually returns BitBlt_hw retval, but that is always 0

#define fs_fprintf(fd, x...) { \
  char buf[256]; int res; \
  sprintf(buf, x); \
  fs_write(fd, buf, strlen(buf), &res); \
}

int is_valid_arm_insn(uint32_t insn, int unconditional_only)
{
	if (insn == 0xe12fff1e) /* BX LR */
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
	    /* in practice, no function ever starts with one or two conditional
	       insns */
	    ((insn & 0xf0000000) != 0xe0000000 && unconditional_only)
	    ) {
	  return 0;
	}
	return 1;
}

int main()
{
	// initialize the game api
	libgame_init();
	int fd;

#if 0
	int res;
	fs_open("dump.bin", FS_O_CREAT|FS_O_WRONLY|FS_O_TRUNC, &fd);
	//fs_write(fd, "Huhu!\n", 6, &res);
	fs_write(fd, (void *)0x280000, 0x480000, &res);
	fs_close(fd);
	fs_open("ftab.bin", FS_O_CREAT|FS_O_WRONLY|FS_O_TRUNC, &fd);
	fs_write(fd, (void *)ftab, 0x150, &res);
	fs_close(fd);
#endif
	
	fs_open("diag.txt", FS_O_CREAT|FS_O_WRONLY|FS_O_TRUNC, &fd);
	//fs_fprintf(fd, "Framebuffer %08x, shadow buffer %08x\n", getLCDFrameBuffer(), getLCDShadowBuffer());
	//fs_fprintf(fd, "Width %d, Height %d\n", getLCDWidth(), getLCDHeight());
	//fs_fprintf(fd, "LCD format %08x\n", getLCDBuffFormat());
	fs_fprintf(fd, "g_stEmuFuncs %08x\n", (uint32_t)g_stEmuFuncs);
	fs_fprintf(fd, "gDisplayDev %08x\n", (uint32_t)gDisplayDev);
	fs_fprintf(fd, "_ecos_close %08x\n", (uint32_t)_ecos_close);
	fs_fprintf(fd, "_ecos_read %08x\n", (uint32_t)_ecos_read);
	fs_fprintf(fd, "_ecos_write %08x\n", (uint32_t)_ecos_write);
	fs_fprintf(fd, "_ecos_lseek %08x\n", (uint32_t)_ecos_lseek);
	fs_fprintf(fd, "_ecos_fstat %08x\n", (uint32_t)_ecos_fstat);
	fs_fprintf(fd, "_ecos_open %08x\n", (uint32_t)_ecos_open);
	fs_fprintf(fd, "_ecos_opendir %08x\n", (uint32_t)_ecos_opendir);
	fs_fprintf(fd, "_ecos_cyg_error_get_errno_p %08x\n", (uint32_t)_ecos_cyg_error_get_errno_p);
	fs_fprintf(fd, "_ecos_cyg_fd_alloc %08x\n", (uint32_t)_ecos_cyg_fd_alloc);
	fs_fprintf(fd, "_ecos_readdir_r %08x\n", (uint32_t)_ecos_readdir_r);
	fs_fprintf(fd, "_ecos_readdir %08x\n", (uint32_t)_ecos_readdir);
	fs_fprintf(fd, "_ecos_stat %08x\n", (uint32_t)_ecos_stat);
	fs_fprintf(fd, "SPMP_SendSignal %08x\n", (uint32_t)SPMP_SendSignal);
	//fs_close(fd);
	
	DIR *dp = _ecos_opendir(".");
	if (!dp) {
	  fs_fprintf(fd, "opendir failed\n");
	  fs_close(fd);
	  return 0;
        }
        struct dirent *de;
        while ((de = _ecos_readdir(dp))) {
          fs_fprintf(fd, "file %s\n", de->d_name);
          struct _ecos_stat st;
          if (_ecos_stat(de->d_name, &st) < 0) {
            fs_fprintf(fd, "stat failed\n");
            fs_close(fd);
            return 0;
          }
          fs_fprintf(fd, "size %d dir %d reg %d\n", st.st_size, _ECOS_S_ISDIR(st.st_mode), _ECOS_S_ISREG(st.st_mode));
        }
        _ecos_closedir(dp);
        char dir[256];
        _ecos_getcwd(dir, 256);
        fs_fprintf(fd, "cwd %s\n", dir);
        _ecos_chdir("..");
        _ecos_getcwd(dir, 256);
        fs_fprintf(fd, "cwd afer chdir %s\n", dir);
        fs_close(fd);
	return 0;
}
