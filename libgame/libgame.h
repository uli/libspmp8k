
#ifndef __LIBGAME_H__
#define __LIBGAME_H__

#include "gfx_types.h"
#include <stdlib.h>
#include <sys/types.h>

#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
#define	MAKE_RGB565(r, g, b) (((b & 0xf8) << 8) | ((g & 0xfc) << 3) | (r >> 3))

/* key bits for cooked (NativeGE_getKeys()) interface */
#define	GE_KEY_UP	1
#define	GE_KEY_DOWN	2
#define	GE_KEY_LEFT	4
#define	GE_KEY_RIGHT	8
#define	GE_KEY_O	(1 << 16)
#define	GE_KEY_X	(2 << 16)
#define	GE_KEY_START	(1 << 13)

/* Cybergame Joystick mapping */
#define	CYBER_KEY_2		KEY_O
#define	CYBER_KEY_3		KEY_X

/* JXD A1000 keys */
#define RAW_A1000_KEY_UP	1
#define RAW_A1000_KEY_DOWN	2
#define RAW_A1000_KEY_LEFT	4
#define RAW_A1000_KEY_RIGHT	8
#define RAW_A1000_KEY_O		16
#define RAW_A1000_KEY_X		32
#define RAW_A1000_KEY_SQUARE	64      /* guess */
#define RAW_A1000_KEY_TRIANGLE	128
#define RAW_A1000_KEY_R		256
#define RAW_A1000_KEY_L		512
#define RAW_A1000_KEY_SELECT	1024
#define RAW_A1000_KEY_START	2048

/* JXD 100 keys */
#define RAW_JXD100_KEY_UP	1
#define RAW_JXD100_KEY_DOWN	2
#define RAW_JXD100_KEY_LEFT	4
#define RAW_JXD100_KEY_RIGHT	8
#define RAW_JXD100_KEY_START	16
#define RAW_JXD100_KEY_SQUARE	32
#define RAW_JXD100_KEY_TRIANGLE	64
#define RAW_JXD100_KEY_SELECT	128
#define RAW_JXD100_KEY_X	512
#define RAW_JXD100_KEY_O	8192
#define RAW_JXD100_KEY_POWER	524288

typedef struct res_entry {
    char filename[16];
    uint8_t *res_data;
} res_entry_t;

typedef struct key_data {
    uint32_t key1;
    uint32_t key2;
} key_data_t;

// the printf (to serial)
extern void (*diag_printf) (char *fmt, ...);
extern int *g_onoff_p;

// graphics stuff
extern int (*MCatchInitGraph) (void);
extern int (*MCatchSetFrameBuffer) (int width, int height);
extern int (*MCatchSetDisplayScreen) (gfx_rect_t *rect);
extern int (*MCatchSetCameraMode) (int mode);
extern int (*MCatchSetFGColor) (uint32_t *color);
extern uint32_t (*MCatchGetFGColor) ();
extern int (*MCatchSetColorROP) (uint32_t rop);
extern int (*MCatchSetAlphaBld) (uint8_t src_alpha, uint8_t dest_alpha);
extern int (*MCatchGetAlphaBld) (uint8_t *src_alpha, uint8_t *dest_alpha);
extern int (*MCatchFillRect) (gfx_rect_t *rect);
extern int (*MCatchEnableFeature) (uint32_t feature);
extern int (*MCatchFlush) (void);
extern int (*MCatchPaint) (void);
extern int (*MCatchLoadImage) (gfx_loadimg_t *loadimg, uint8_t *imgid);
extern int (*MCatchFreeImage) (uint8_t img_id);
extern int (*MCatchBitblt) (uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);
extern int (*MCatchSprite) (uint8_t img_id, gfx_rect_t *rect, gfx_point2d_t *at);

// music & sound (has some problems)
extern int (*NativeGE_initRes) (int val, void *res_table);
extern int (*NativeGE_getRes) (char *filename, void *res_info);
extern int (*NativeGE_playRes) (uint8_t res_type, int flags, void *res_info);
extern int (*NativeGE_stopRes) (int arg);

// taken from eCos fcntl.h / unistd.h
#define FS_O_RDONLY     (1<<0)  // Open for reading only
#define FS_O_WRONLY     (1<<1)  // Open for writing only
#define FS_O_RDWR       (O_RDONLY|O_WRONLY)     // Open for reading and
                                                // writing
#define FS_O_CREAT      (1<<3)  // Create file it it does not exist
#define FS_O_EXCL       (1<<4)  // Exclusive use
#define FS_O_NOCTTY     (1<<5)  // Do not assign a controlling terminal
#define FS_O_TRUNC      (1<<6)  // Truncate

#define FS_SEEK_SET		0
#define FS_SEEK_CUR		1
#define FS_SEEK_END		2

#define	FS_STDIN_FILENO	0
#define	FS_STDOUT_FILENO	1
#define	FS_STDERR_FILENO	2

// filesystem
extern int (*NativeGE_fsOpen) (const char *filename, int flags, int *fd);

/* returns 0 (okay), 2 (error) */
extern int (*NativeGE_fsRead) (int fd, const void *buf, size_t count, int *result);
extern int (*NativeGE_fsWrite) (int fd, const void *buf, size_t count, int *result);
extern int (*NativeGE_fsClose) (int fd);
extern uint64_t (*NativeGE_fsSeek) (int fd, int offset, int whence);
#define tell(fd) (NativeGE_fsSeek(fd, 0, SEEK_CUR) >> 32)

// misc.
extern uint32_t (*NativeGE_getTime) (void);     // returns system ticks
                                                // equivalent
extern void (*NativeGE_getKeyInput4Ntv) (key_data_t * keys);

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
typedef int _ecos_size_t;

#undef st_atime
#undef st_mtime
#undef st_ctime
#define _ecos___stat_mode_DIR    (1<<0)
#define _ecos___stat_mode_REG    (1<<3)
#define _ECOS_S_ISREG(__mode)  ((__mode) & _ecos___stat_mode_REG )
#define _ECOS_S_ISDIR(__mode)  ((__mode) & _ecos___stat_mode_DIR )

struct _ecos_stat {
    _ecos_mode_t st_mode;       /* File mode */
    _ecos_ino_t st_ino;         /* File serial number */
    _ecos_dev_t st_dev;         /* ID of device containing file */
    _ecos_nlink_t st_nlink;     /* Number of hard links */
    _ecos_uid_t st_uid;         /* User ID of the file owner */
    _ecos_gid_t st_gid;         /* Group ID of the file's group */
    _ecos_off_t st_size;        /* File size (regular files only) */
    _ecos_time_t st_atime;      /* Last access time */
    _ecos_time_t st_mtime;      /* Last data modification time */
    _ecos_time_t st_ctime;      /* Last file status change time */
};

/* File access modes used for open() and fnctl() */
#define _ECOS_O_RDONLY     (1<<0)       /* Open for reading only */
#define _ECOS_O_WRONLY     (1<<1)       /* Open for writing only */
#define _ECOS_O_RDWR       (_ECOS_O_RDONLY|_ECOS_O_WRONLY)      /* Open for
                                                                   reading
                                                                   and
                                                                   writing */

/* File access mode mask */
#define _ECOS_O_ACCMODE    (_ECOS_O_RDONLY|_ECOS_O_RDWR|_ECOS_O_WRONLY)

/* open() mode flags */

#define _ECOS_O_CREAT      (1<<3)       /* Create file it it does not exist */
#define _ECOS_O_EXCL       (1<<4)       /* Exclusive use */
#define _ECOS_O_NOCTTY     (1<<5)       /* Do not assign a controlling
                                           terminal */
#define _ECOS_O_TRUNC      (1<<6)       /* Truncate */

/* File status flags used for open() and fcntl() */
#define _ECOS_O_APPEND     (1<<7)       /* Set append mode */
#define _ECOS_O_DSYNC      (1<<8)       /* Synchronized I/O data integrity
                                           writes */
#define _ECOS_O_NONBLOCK   (1<<9)       /* No delay */
#define _ECOS_O_RSYNC      (1<<10)      /* Synchronized read I/O */
#define _ECOS_O_SYNC       (1<<11)      /* Synchronized I/O file integrity
                                           writes */

typedef struct {
    void (*setBuffFormat) (int);
    int (*getBuffFormat) (void);
    int (*getWidth) (void);
    int (*getHeight) (void);
    uint16_t *(*getShadowBuffer) (void);
    void (*setFrameBuffer) (uint16_t *fb);      // educated guess
    void (*lcdFlip) (void);
    void (*lcdClear) (void);    // actually returns BitBlt_hw retval, but
                                // that is always 0
    void (*setShadowBuffer) (uint16_t *fb);     // educated guess
    uint16_t *(*getFrameBuffer) (void);
} display_dev_t;

typedef struct {
    void *_unknown;
    int (*pause) (void);
    int (*resume) (void);
    int (*exit) (void);
} emu_apis_t;

extern void **g_stEmuFuncs;
extern emu_apis_t *g_stEmuAPIs;
display_dev_t *gDisplayDev;
extern int (*_ecos_close) (int fd);
extern int (*_ecos_read) (int fd, void *buf, unsigned int count);
extern int (*_ecos_write) (int fd, const void *buf, unsigned int count);
extern int (*_ecos_lseek) (int fd, int offset, int whence);
extern int (*_ecos_fstat) (int fd, struct _ecos_stat * buf);
extern int (*_ecos_open) (const char *pathname, int flags, int mode);
extern _ecos_DIR *(*_ecos_opendir) (const char *name);
extern void *_ecos_cyg_error_get_errno_p;
extern void *_ecos_cyg_fd_alloc;
extern struct _ecos_dirent *(*_ecos_readdir) (_ecos_DIR * dirp);
extern int (*_ecos_readdir_r) (_ecos_DIR * dirp, struct _ecos_dirent * entry,
                               struct _ecos_dirent ** result);
extern int (*_ecos_closedir) (_ecos_DIR * dirp);
extern int (*_ecos_stat) (const char *path, struct _ecos_stat * buf);
extern char *(*_ecos_getcwd) (char *buf, size_t size);
extern int (*_ecos_chdir) (const char *path);
extern int (*_ecos_unlink) (const char *pathname);
extern int (*_ecos_rmdir) (const char *pathname);
extern int (*_ecos_mkdir) (const char *pathname, _ecos_mode_t mode);
extern int (*_ecos_fsync) (int fd);
extern void (*cyg_thread_delay) (uint64_t /* cyg_tick_count_t */ delay);

extern uint16_t (*SPMP_SendSignal) (uint16_t cmd, void *data, uint16_t size);
extern void (*cache_sync) (void);

/* extern int (*MCatchStoreImage) (void); doesn't do anything */
/* extern int (*MCatchDecodeImageFromCard) (void); doesn't do anything */
extern int (*MCatchGetColorROP) (uint32_t *rop);
extern int (*MCatchSetBitPlaneMask) (int read_write, uint16_t mask);
extern int (*MCatchGetBitPlaneMask) (int read_write, uint16_t *mask);
extern int (*MCatchGetDisplayScreen) (gfx_rect_t *);
extern int (*MCatchSetRectClip) (gfx_rect_t *);
extern int (*MCatchGetRectClip) (gfx_rect_t *);
extern int (*MCatchSetStyleMask) (gfx_rect_t *);
extern int (*MCatchGetStyleMask) (gfx_rect_t *);
extern int (*MCatchSetLineMask) (gfx_rect_t *);
extern int (*MCatchGetLineMask) (gfx_rect_t *);
extern int (*MCatchDisableFeature) (int);
extern int (*MCatchSetStyleLine) (uint8_t, uint8_t);
/* extern int (*MCatchPreviewColorkey) (void); doesn't do anything */
extern int (*MCatchGetFrameBuffer) (uint16_t *width, uint16_t *height);
extern int (*MCatchSetMutableImage) (uint8_t);
extern int (*MCatchSetPerPixelAlphaEq) (uint8_t);       /* 0 or 1 */
extern int (*MCatchSetTransformation) (gfx_point2d_t *, int /* 0 to 7 */ );
extern int (*MCatchQueryImage) (uint8_t, uint8_t /* 1 to 3 */ );
extern int (*MCatchEnableDoubleBuffer) (int /* 0 or 1 */ );
extern int (*MCatchGradientFill) (gfx_rect_t *, uint16_t[6], uint32_t[2]);
/* extern int (*MCatchUpdateScreen) (void); doesn't do anything (except produce debug output) */
extern int (*MCatchShowFont) (gfx_point2d_t *, int, uint8_t /* < 0x18 */ , uint8_t      /* < 
                                                                                           0x18 
                                                                                         */ );
extern int (*MCatchModifyPalette) (uint8_t, uint8_t, uint8_t /* size */ , void *        /* data 
                                                                                         */ );
extern void (*NativeGE_pauseRes) (uint8_t);
extern void (*NativeGE_resumeRes) (uint8_t);
extern int (*NativeGE_writeRecord) (const char *pathname, void *buf, uint8_t flags,
                                    _ecos_off_t offset, _ecos_size_t count);
extern int (*NativeGE_readRecord) (const char *pathname, void *buf, uint8_t flags,
                                   _ecos_off_t offset, _ecos_size_t count);
/* extern void (*NativeGE_showFPS) (void); doesn't do anything */
extern int (*NativeGE_gameExit) (void);
/* extern int (*NativeGE_getTPEvent) (void); doesn't do anything */
/* extern char (*NativeGE_setTPClickArea) (void); doesn't do anything */

extern int (*NativeGE_gamePause) (void);
extern int (*NativeGE_gameResume) (void);

typedef struct sound_params {
    uint8_t *buf;               /* + 0x0 */
    uint32_t buf_size;          /* + 0x4 */
    uint32_t rate;              /* + 0x8 */
    uint8_t depth;              /* + 0xc */
    uint8_t channels;           /* + 0xd */
    uint32_t *callback;         /* + 0x10 */ /* used only when gEmuType is 5 (flash?) */
} sound_params_t;

typedef struct graph_params {
    uint16_t *pixels;           // goes to src_ctx + 4
    uint32_t width;             // +4
    uint32_t height;            // +8
    uint32_t unknown_flag;      // +c
    uint32_t palette;           // +10
    uint32_t pad2;
    uint32_t src_clip_x;        // +18
    uint32_t src_clip_y;
    uint32_t src_clip_w;        // +1c
    uint32_t src_clip_h;
} graph_params_t;

typedef struct keymap {
    uint32_t _unknown;
    uint32_t scancode[20];
} keymap_t;

#define EMU_FUNC_ENTRY(n)	(g_stEmuFuncs[(n) / 4])

extern int (*emuIfGraphInit) (graph_params_t *);
extern int (*emuIfGraphShow) (void);
extern int (*emuIfGraphChgView) (graph_params_t *);
extern int (*emuIfGraphCleanup) (void);
extern uint32_t (*emuIfSoundInit) (sound_params_t *params);
extern uint32_t (*emuIfSoundPlay) (sound_params_t *params);
extern uint32_t (*emuIfSoundCleanup) (sound_params_t *params);

extern int (*emuIfunknown0c) (void *);  /* sets the source buffer? */
extern int (*emuIfKeyInit) (keymap_t *map);
extern uint32_t (*emuIfKeyGetInput) (keymap_t *map);
extern int (*emuIfKeyCleanup) (keymap_t *map);
extern uint32_t (*emuIfGetCurTime) (void);  /* could be uint64_t, not sure */
extern void (*emuIfTimeDelay) (uint32_t);
extern int (*emuIfFsFileOpen) (const char *pathname, uint32_t flags);
extern uint32_t (*emuIfFsFileGetSize) (int fd);
extern int (*emuIfFsFileWrite) (int fd, const void *buf, uint32_t count);
extern int (*emuIfFsFileRead) (int fd, void *buf, uint32_t count);
extern int (*emuIfFsFileGetChar) (int fd);
extern int (*emuIfFsFileSeek) (int fd, _ecos_off_t offset, uint8_t whence);
extern _ecos_off_t(*emuIfFsFileCurPos) (int fd);
extern int (*emuIfFsFileClose) (int fd);
extern int (*emuIfkgbCevaLoader) (void);
extern int (*emuIfcevaImageUnload) (void);
extern int (*emuIfunknown5c) (void *);
extern int (*emuIfunknown60) (void *);
extern int (*emuIfunknown64) (void);
extern void (*emuIfunknown68) (void *);
extern int (*emuIfunknown6c) (int, int, int);
extern void (*emuIfunknown74) (int, void *, int);
extern int (*emuIfunknown78) (void);

extern int (*GetArmCoreFreq) (void);
extern int (*changeARMFreq) (int mhz);

extern void (*hal_clock_read) (uint32_t *us);
extern uint64_t (*cyg_current_time) (void);

extern uint64_t libgame_utime(void);

extern int libgame_system_id;
#define SYS_UNKNOWN 0
#define SYS_JXD_100 1
#define SYS_JXD_1000 2
#define SYS_JXD_A1000 3
#define SYS_JXD_V3 4
#define SYS_LETCOOL 5
#define SYS_MMM 6

#endif                          // __LIBGAME_H__
