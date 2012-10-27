
#ifndef __LIBGAME_H__
#define __LIBGAME_H__

#include "gfx_types.h"
#include <stdlib.h>
#include <sys/types.h>

#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
#define	MAKE_RGB565(r, g, b) (((b & 0xf8) << 8) | ((g & 0xfc) << 3) | (r >> 3))

#define	KEY_UP		1
#define	KEY_DOWN	2
#define	KEY_LEFT	4
#define	KEY_RIGHT	8
#define	KEY_O		(1 << 16)
#define	KEY_X		(2 << 16)
#define	KEY_START	(1 << 13)

/* Cybergame Joystick mapping */
#define	KEY_2		KEY_O
#define	KEY_3		KEY_X

typedef struct res_entry {
	char	filename[16];
	uint8_t	*res_data;
} res_entry_t;

typedef struct key_data {
	uint32_t	key1;
	uint32_t	key2;
} key_data_t;

// the printf (to serial)
extern void (*diag_printf)(char *fmt, ...);

// graphics stuff
extern int (*gfx_init)(void *buffer, int buffsize);
extern int (*gfx_set_framebuffer)(int width, int height);
extern int (*gfx_set_display_screen)(gfx_rect_t *rect);
extern int (*gfx_set_cammmode)(int mode);
extern int (*gfx_set_fgcolor)(uint32_t *color);
extern uint32_t (*gfx_get_fgcolor)();
extern int (*gfx_set_colorrop)(uint32_t rop);
extern int (*gfx_set_alpha)(uint8_t src_alpha, uint8_t dest_alpha);
extern int (*gfx_get_alpha)(uint8_t *src_alpha, uint8_t *dest_alpha);
extern int (*gfx_fillrect)(gfx_rect_t *rect);
extern int (*gfx_enable_feature)(uint32_t feature);
extern int (*gfx_flush)(void);
extern int (*gfx_paint)(void);
extern int (*gfx_load_image)(gfx_loadimg_t *loadimg, uint8_t *imgid);
extern int (*gfx_free_image)(uint8_t img_id);
extern int (*gfx_bitblt)(uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);
extern int (*gfx_sprite)(uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);

// music & sound (has some problems)
extern int (*res_init)(int val, void *res_table);
extern int (*res_get)(char *filename,void *res_info);
extern int (*res_play)(uint8_t res_type, int flags, void *res_info);
extern int (*res_stop)(int arg);

// taken from eCos fcntl.h / unistd.h
#define FS_O_RDONLY     (1<<0)					// Open for reading only
#define FS_O_WRONLY     (1<<1)					// Open for writing only
#define FS_O_RDWR       (O_RDONLY|O_WRONLY)	// Open for reading and writing
#define FS_O_CREAT      (1<<3)					// Create file it it does not exist
#define FS_O_EXCL       (1<<4)					// Exclusive use
#define FS_O_NOCTTY     (1<<5)					// Do not assign a controlling terminal
#define FS_O_TRUNC      (1<<6)					// Truncate

#define FS_SEEK_SET		0
#define FS_SEEK_CUR		1
#define FS_SEEK_END		2

#define	FS_STDIN_FILENO	0
#define	FS_STDOUT_FILENO	1
#define	FS_STDERR_FILENO	2

// filesystem
extern int (*fs_open)(const char *filename, int flags, int *fd);
extern int (*fs_read)(int fd, const void *buf, size_t count, int *result);		// returns 0-okay / 2-error
extern int (*fs_write)(int fd, const void *buf, size_t count, int *result);
extern int (*fs_close)(int fd);
extern uint64_t (*fs_seek)(int fd, int offset, int whence);
#define tell(fd) (fs_seek(fd, 0, SEEK_CUR) >> 32)

// misc.
extern uint32_t (*get_time)(void);			// returns system ticks equivalent
extern void (*get_keys)(key_data_t *keys);//uint64_t *keys);

// ### initializes the function pointers
// ### (must be called before everything else!)
void libgame_init(void);

// dmsg stuff
int dmsg_init(int x, int y, int width, int height);
void dmsg_shutdown(void);
void dmsg_wait(int enable);
void dmsg_clear(void);
int dmsg_puts(char *__s);
int dmsg_printf(char *format, ...);

typedef void _ecos_DIR;
#define _ecos_NAME_MAX 256
struct _ecos_dirent {
  /* no d_type :( */
  char d_name[_ecos_NAME_MAX + 1];
};

typedef short _ecos_dev_t;
typedef unsigned int _ecos_ino_t;
typedef unsigned int _ecos_mode_t;
typedef unsigned short _ecos_nlink_t;
typedef int _ecos_time_t;
typedef unsigned short _ecos_uid_t;
typedef unsigned short _ecos_gid_t;
typedef int _ecos_off_t;

#undef st_atime
#undef st_mtime
#undef st_ctime
#define _ecos___stat_mode_DIR    (1<<0)
#define _ecos___stat_mode_REG    (1<<3)
#define _ECOS_S_ISREG(__mode)  ((__mode) & _ecos___stat_mode_REG )
#define _ECOS_S_ISDIR(__mode)  ((__mode) & _ecos___stat_mode_DIR )

struct _ecos_stat {
    _ecos_mode_t  st_mode;     /* File mode */
    _ecos_ino_t   st_ino;      /* File serial number */
    _ecos_dev_t   st_dev;      /* ID of device containing file */
    _ecos_nlink_t st_nlink;    /* Number of hard links */
    _ecos_uid_t   st_uid;      /* User ID of the file owner */
    _ecos_gid_t   st_gid;      /* Group ID of the file's group */
    _ecos_off_t   st_size;     /* File size (regular files only) */
    _ecos_time_t  st_atime;    /* Last access time */
    _ecos_time_t  st_mtime;    /* Last data modification time */
    _ecos_time_t  st_ctime;    /* Last file status change time */
};

/* File access modes used for open() and fnctl() */
#define _ECOS_O_RDONLY     (1<<0)   /* Open for reading only */
#define _ECOS_O_WRONLY     (1<<1)   /* Open for writing only */
#define _ECOS_O_RDWR       (_ECOS_O_RDONLY|_ECOS_O_WRONLY) /* Open for reading and writing */

/* File access mode mask */
#define _ECOS_O_ACCMODE    (_ECOS_O_RDONLY|_ECOS_O_RDWR|_ECOS_O_WRONLY)

/* open() mode flags */

#define _ECOS_O_CREAT      (1<<3)    /* Create file it it does not exist */
#define _ECOS_O_EXCL       (1<<4)    /* Exclusive use */
#define _ECOS_O_NOCTTY     (1<<5)    /* Do not assign a controlling terminal */
#define _ECOS_O_TRUNC      (1<<6)    /* Truncate */

/* File status flags used for open() and fcntl() */
#define _ECOS_O_APPEND     (1<<7)    /* Set append mode */
#define _ECOS_O_DSYNC      (1<<8)    /* Synchronized I/O data integrity writes */
#define _ECOS_O_NONBLOCK   (1<<9)    /* No delay */
#define _ECOS_O_RSYNC      (1<<10)   /* Synchronized read I/O */
#define _ECOS_O_SYNC       (1<<11)   /* Synchronized I/O file integrity writes */


typedef struct {
    void (*setBuffFormat)(int);
    int (*getBuffFormat)(void);
    int (*getWidth)(void);
    int (*getHeight)(void);
    uint16_t *(*getShadowBuffer)(void);
    void (*setFrameBuffer)(uint16_t *fb); // educated guess
    void (*lcdFlip)(void);
    void (*lcdClear)(void); // actually returns BitBlt_hw retval, but that is always 0
    void (*setShadowBuffer)(uint16_t *fb); // educated guess
    uint16_t *(*getFrameBuffer)(void);
} display_dev_t;

extern void **g_stEmuFuncs;
display_dev_t *gDisplayDev;
extern int (*_ecos_close)(int fd);
extern int (*_ecos_read)(int fd, void *buf, unsigned int count);
extern int (*_ecos_write)(int fd, const void *buf, unsigned int count);
extern int (*_ecos_lseek)(int fd, int offset, int whence);
extern int (*_ecos_fstat)(int fd, struct _ecos_stat *buf);
extern int (*_ecos_open)(const char *pathname, int flags, int mode);
extern _ecos_DIR *(*_ecos_opendir)(const char *name);
extern void *_ecos_cyg_error_get_errno_p;
extern void *_ecos_cyg_fd_alloc;
extern struct _ecos_dirent *(*_ecos_readdir)(_ecos_DIR *dirp);
extern int (*_ecos_readdir_r)(_ecos_DIR *dirp, struct _ecos_dirent *entry, struct _ecos_dirent **result);
extern int (*_ecos_closedir)(_ecos_DIR *dirp);
extern int (*_ecos_stat)(const char *path, struct _ecos_stat *buf);
extern char *(*_ecos_getcwd)(char *buf, size_t size);
extern int (*_ecos_chdir)(const char *path);
extern int (*_ecos_unlink)(const char *pathname);
extern int (*_ecos_rmdir)(const char *pathname);
extern int (*_ecos_mkdir)(const char *pathname, _ecos_mode_t mode);
extern int (*_ecos_fsync)(int fd);
extern void (*cyg_thread_delay)(uint64_t /* cyg_tick_count_t */ delay);

extern uint16_t (*SPMP_SendSignal)(uint16_t cmd, void *data, uint16_t size);
extern void (*cache_sync)(void);
extern int (*NativeGE_getKeyInput)(key_data_t *);

#endif // __LIBGAME_H__

