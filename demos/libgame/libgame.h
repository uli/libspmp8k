
#ifndef __LIBGAME_H__
#define __LIBGAME_H__

#include "gfx_types.h"

#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
#define	MAKE_RGB565(r, g, b) (((b & 0xf8) << 11) | ((g & 0xfc) << 5) | (r & 0x1f))

#define	KEY_UP		1
#define	KEY_DOWN	2
#define	KEY_LEFT	4
#define	KEY_RIGHT	8
#define	KEY_O		1 << 16
#define	KEY_X		2 << 16

/*
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;
*/
typedef uint32_t	size_t;
/*
#define COLOR_ROP_TRANSP	0xcc	// draws opaque
#define COLOR_ROP_NOP		0xf0
*/
typedef struct res_entry {
	char	filename[16];
	uint8_t	*res_data;
} res_entry_t;

typedef struct key_data {
	uint32_t	key1;
	uint32_t	key2;
} key_data_t;

// the printf (to serial)
extern void (*_dprintf)(char *fmt, ...);

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
int libgame_init(void);

// dmsg stuff
int dmsg_init(int x, int y, int width, int height);
void dmsg_shutdown(void);
void dmsg_wait(int enable);
void dmsg_clear(void);
int dmsg_puts(char *__s);
int dmsg_printf(char *format, ...);

#endif // __LIBGAME_H__

