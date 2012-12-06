/// @file

#ifndef __LIBGAME_H__
#define __LIBGAME_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mcatch_types.h"
#include <stdlib.h>
#include <sys/types.h>

/* Doxygen workaround: If you don't do this, all hooks will show up in the
   first group. */
/// @name Hooks
/// @{
#ifdef DOXYGEN
#define DOXYGEN_SUCKS
#endif
/// @}

/// @addtogroup utility Utility functions
/// @{
#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
#define	MAKE_RGB565(r, g, b) (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3))

extern uint64_t libgame_utime(void);
extern int libgame_chdir_game(void);

/// Enable/disable debug output.
/// Safe way of enabling/disabling debug output. Does nothing if the
/// required hook is not available.
extern void libgame_set_debug(int onoff);
/// @}

/***************************************************************************/

/// @defgroup nativege NativeGE interface

/***************************************************************************/

/// @addtogroup ge_key NativeGE user input
/// @ingroup nativege
/// @{

/// @name NativeGE_getKeyInput4Ntv() key masks
/// @{
#define	GE_KEY_UP	1
#define	GE_KEY_DOWN	2
#define	GE_KEY_LEFT	4
#define	GE_KEY_RIGHT	8
#define	GE_KEY_O	(1 << 16)
#define	GE_KEY_X	(2 << 16)
#define	GE_KEY_START	(1 << 13)
/// @}

/// @name Cybergame key mapping
/// @{
#define	CYBER_KEY_2		GE_KEY_O
#define	CYBER_KEY_3		GE_KEY_X
/// @}

typedef struct ge_key_data {
    uint32_t key1;
    uint32_t key2;
} ge_key_data_t;

typedef struct ge_tp_event {
    uint32_t type;
    uint32_t key;	/* not set by NativeGE_getTPEvent() (on the A16) */
    uint16_t x;
    uint16_t y;
} ge_tp_event_t;

/// @name Hooks
/// @{
extern void (*NativeGE_getKeyInput4Ntv) (ge_key_data_t * keys);
extern int (*NativeGE_getTPEvent) (ge_tp_event_t *);
/* extern char (*NativeGE_setTPClickArea) (void); doesn't do anything */
/// @}

/// @}

/***************************************************************************/

/// @addtogroup debug OS debug interface.
/// @{

/// @name Hooks
/// @{
/// Print debug output to serial port.
extern void (*diag_printf) (char *fmt, ...);
/// @}

/// Enable/disable debug output.
extern int *g_onoff_p;
///Pointer to debug character out function.
///     Can be used to redirect debug output.
extern void (**_diag_putc) (char);
/// @}

/***************************************************************************/

/// @addtogroup ge_gfx NativeGE graphics
/// @ingroup nativege
/// @{

/// @name Hooks
/// @{
extern int (*MCatchInitGraph) (void);
extern int (*MCatchSetFrameBuffer) (int width, int height);
extern int (*MCatchSetDisplayScreen) (mcatch_rect_t *rect);
extern int (*MCatchSetCameraMode) (int mode);
extern int (*MCatchSetFGColor) (uint32_t *color);
extern uint32_t (*MCatchGetFGColor) ();
extern int (*MCatchSetColorROP) (uint32_t rop);
extern int (*MCatchSetAlphaBld) (uint8_t src_alpha, uint8_t dest_alpha);
extern int (*MCatchGetAlphaBld) (uint8_t *src_alpha, uint8_t *dest_alpha);
extern int (*MCatchFillRect) (mcatch_rect_t *rect);
extern int (*MCatchEnableFeature) (uint32_t feature);
extern int (*MCatchFlush) (void);
extern int (*MCatchPaint) (void);
extern int (*MCatchLoadImage) (mcatch_loadimg_t *loadimg, uint8_t *imgid);
extern int (*MCatchFreeImage) (uint8_t img_id);
extern int (*MCatchBitblt) (uint8_t img_id, mcatch_rect_t *rect, mcatch_point2d_t *at);
extern int (*MCatchSprite) (uint8_t img_id, mcatch_rect_t *rect, mcatch_point2d_t *at);

/* extern int (*MCatchStoreImage) (void); doesn't do anything */
/* extern int (*MCatchDecodeImageFromCard) (void); doesn't do anything */
extern int (*MCatchGetColorROP) (uint32_t *rop);
extern int (*MCatchSetBitPlaneMask) (int read_write, uint16_t mask);
extern int (*MCatchGetBitPlaneMask) (int read_write, uint16_t *mask);
extern int (*MCatchGetDisplayScreen) (mcatch_rect_t *);
extern int (*MCatchSetRectClip) (mcatch_rect_t *);
extern int (*MCatchGetRectClip) (mcatch_rect_t *);
extern int (*MCatchSetStyleMask) (mcatch_rect_t *);
extern int (*MCatchGetStyleMask) (mcatch_rect_t *);
extern int (*MCatchSetLineMask) (mcatch_rect_t *);
extern int (*MCatchGetLineMask) (mcatch_rect_t *);
extern int (*MCatchDisableFeature) (int);
extern int (*MCatchSetStyleLine) (uint8_t, uint8_t);
/* extern int (*MCatchPreviewColorkey) (void); doesn't do anything */
extern int (*MCatchGetFrameBuffer) (uint16_t *width, uint16_t *height);
extern int (*MCatchSetMutableImage) (uint8_t);
extern int (*MCatchSetPerPixelAlphaEq) (uint8_t);       /* 0 or 1 */
extern int (*MCatchSetTransformation) (mcatch_point2d_t *, int /* 0 to 7 */);
extern int (*MCatchQueryImage) (uint8_t, uint8_t /* 1 to 3 */);
extern int (*MCatchEnableDoubleBuffer) (int /* 0 or 1 */);
extern int (*MCatchGradientFill) (mcatch_rect_t *, uint16_t[6], uint32_t[2]);
/* extern int (*MCatchUpdateScreen) (void); doesn't do anything (except produce debug output) */
extern int (*MCatchShowFont) (mcatch_point2d_t *, int, uint8_t /* < 0x18 */,
                              uint8_t /* < 0x18 */);
extern int (*MCatchModifyPalette) (uint8_t, uint8_t, uint8_t size, void *data);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_res NativeGE audio
/// @ingroup nativege
/// @{
typedef struct ge_res_entry {
    char filename[16];
    uint8_t *res_data;
} ge_res_entry_t;

/// @name Hooks
/// @{
extern int (*NativeGE_initRes) (int val, void *res_table);
extern int (*NativeGE_getRes) (char *filename, void *res_info);
extern int (*NativeGE_playRes) (uint8_t res_type, int flags, void *res_info);
extern int (*NativeGE_stopRes) (int arg);

extern void (*NativeGE_pauseRes) (uint8_t);
extern void (*NativeGE_resumeRes) (uint8_t);

extern uint16_t (*NativeGE_SPUCommand) (uint16_t, uint32_t);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_control NativeGE thread control
/// @ingroup nativege
/// @{

/// @name Hooks
/// @{
extern int (*NativeGE_gameExit) (void);
extern int (*NativeGE_gamePause) (void);
extern int (*NativeGE_gameResume) (uint32_t);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_misc NativeGE miscellaneous
/// @ingroup nativege
/// @{

/// @name Hooks
/// @{
extern uint32_t (*NativeGE_getTime) (void);     /* returns system ticks
                                                   multiplied by 10 */
/* extern void (*NativeGE_showFPS) (void); doesn't do anything */
/// @}

/// @}

/***************************************************************************/

/// @addtogroup init Initialization and finalization
/// @{
/// Initialization function must be called before everything else.
void libgame_init(void);
/// @}

/***************************************************************************/

/// @addtogroup dmsg Graphical Debug Console
/// @{
int dmsg_init(int x, int y, int width, int height);
void dmsg_shutdown(void);
void dmsg_wait(int enable);
void dmsg_clear(void);
int dmsg_puts(char *__s);
int dmsg_printf(char *format, ...);
/// @}

/***************************************************************************/

/// @addtogroup ecos eCos interface
/// @{
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
    _ecos_mode_t st_mode;       ///< File mode
    _ecos_ino_t st_ino;         ///< File serial number
    _ecos_dev_t st_dev;         ///< ID of device containing file
    _ecos_nlink_t st_nlink;     ///< Number of hard links
    _ecos_uid_t st_uid;         ///< User ID of the file owner
    _ecos_gid_t st_gid;         ///< Group ID of the file's group
    _ecos_off_t st_size;        ///< File size (regular files only)
    _ecos_time_t st_atime;      ///< Last access time
    _ecos_time_t st_mtime;      ///< Last data modification time
    _ecos_time_t st_ctime;      ///< Last file status change time
};

/// File access modes used for open() and fnctl().
#define _ECOS_O_RDONLY     (1<<0)       ///< Open for reading only
#define _ECOS_O_WRONLY     (1<<1)       ///< Open for writing only
#define _ECOS_O_RDWR       (_ECOS_O_RDONLY|_ECOS_O_WRONLY) ///< Open for reading and writing

/// File access mode mask
#define _ECOS_O_ACCMODE    (_ECOS_O_RDONLY|_ECOS_O_RDWR|_ECOS_O_WRONLY)

/// open() mode flags
#define _ECOS_O_CREAT      (1<<3)       ///< Create file it it does not exist
#define _ECOS_O_EXCL       (1<<4)       ///< Exclusive use
#define _ECOS_O_NOCTTY     (1<<5)       ///< Do not assign a controlling terminal
#define _ECOS_O_TRUNC      (1<<6)       ///< Truncate

/// File status flags used for open() and fcntl()
#define _ECOS_O_APPEND     (1<<7)       ///< Set append mode
#define _ECOS_O_DSYNC      (1<<8)       ///< Synchronized I/O data integrity writes
#define _ECOS_O_NONBLOCK   (1<<9)       ///< No delay
#define _ECOS_O_RSYNC      (1<<10)      ///< Synchronized read I/O
#define _ECOS_O_SYNC       (1<<11)      ///< Synchronized I/O file integrity writes

/// @name Hooks
/// @{
extern int (*_ecos_close) (int fd);
extern int (*_ecos_read) (int fd, void *buf, unsigned int count);
extern int (*_ecos_write) (int fd, const void *buf, unsigned int count);
extern int (*_ecos_lseek) (int fd, int offset, int whence);
extern int (*_ecos_fstat) (int fd, struct _ecos_stat * buf);
extern int (*_ecos_open) (const char *pathname, int flags, int mode);
extern _ecos_DIR *(*_ecos_opendir) (const char *name);
extern int *(*_ecos_cyg_error_get_errno_p) (void);
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

extern uint64_t (*cyg_current_time) (void);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_fs NativeGE file system access
/// @ingroup nativege
/// @{

/// eCos constants from fcntl.h, unistd.h
#define FS_O_RDONLY     (1<<0)
#define FS_O_WRONLY     (1<<1)
#define FS_O_RDWR       (O_RDONLY|O_WRONLY)
#define FS_O_CREAT      (1<<3)
#define FS_O_EXCL       (1<<4)
#define FS_O_NOCTTY     (1<<5)
#define FS_O_TRUNC      (1<<6)

#define FS_SEEK_SET		0
#define FS_SEEK_CUR		1
#define FS_SEEK_END		2

#define	FS_STDIN_FILENO	0
#define	FS_STDOUT_FILENO	1
#define	FS_STDERR_FILENO	2

/// @name Hooks
/// @{
extern int (*NativeGE_fsOpen) (const char *filename, int flags, int *fd);

/// returns 0 (okay), 2 (error)
extern int (*NativeGE_fsRead) (int fd, const void *buf, size_t count, int *result);
extern int (*NativeGE_fsWrite) (int fd, const void *buf, size_t count, int *result);
extern int (*NativeGE_fsClose) (int fd);
extern uint64_t (*NativeGE_fsSeek) (int fd, int offset, int whence);

extern int (*NativeGE_writeRecord) (const char *pathname, void *buf, uint8_t flags,
                                    _ecos_off_t offset, _ecos_size_t count);
extern int (*NativeGE_readRecord) (const char *pathname, void *buf, uint8_t flags,
                                   _ecos_off_t offset, _ecos_size_t count);
/// @}

#define tell(fd) (NativeGE_fsSeek(fd, 0, SEEK_CUR) >> 32)

/// @}

/***************************************************************************/

/// @addtogroup hw Low-level hardware access
/// @{
typedef struct {
    void (*setBuffFormat) (int);
    int (*getBuffFormat) (void);
    int (*getWidth) (void);
    int (*getHeight) (void);
    uint16_t *(*getShadowBuffer) (void);
    void (*setFrameBuffer) (uint16_t *fb);
    void (*flip) (void);
    void (*clear) (void);    //< actually returns BitBlt_hw retval, but that is always 0
    void (*setShadowBuffer) (uint16_t *fb);
    uint16_t *(*getFrameBuffer) (void);
} disp_device_t;

extern disp_device_t *gDisplayDev;

/// @name Hooks
/// @{
extern void (*cache_sync) (void);

extern int (*GetArmCoreFreq) (void);
extern int (*changeARMFreq) (int mhz);

extern void (*hal_clock_read) (uint32_t *us);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup emu Emulator interface (emuIf)
/// @{
typedef struct {
    void *_unknown;
    int (*pause) (void);
    int (*resume) (uint32_t);
    int (*exit) (uint32_t);
    int (*save) (int);
    int (*load) (int);
} emu_apis_t;

extern void **g_stEmuFuncs;
extern emu_apis_t *g_stEmuAPIs;


typedef struct emu_sound_params {
    uint8_t *buf;               ///< + 0x00; sound buffer
    uint32_t buf_size;          ///< + 0x04; sound buffer size (bytes)
    uint32_t rate;              ///< + 0x08; sample rate (Hz)
    uint8_t depth;              ///< + 0x0c; not sure if this does anything at all
    uint8_t channels;           ///< + 0x0d; number of channels (1 or 2)
    uint32_t *callback;         ///< + 0x10; used only when gEmuType is 5 (flash?)
} emu_sound_params_t;

typedef struct emu_graph_params {
    uint16_t *pixels;           ///< source buffer
    uint32_t width;             ///< source width
    uint32_t height;            ///< source height
    uint32_t unknown_flag;
    uint32_t palette;           ///< +10
    uint32_t pad2;
    uint32_t src_clip_x;        ///< source clipping rectangle left
    uint32_t src_clip_y;	///< source clipping rectangle top
    uint32_t src_clip_w;        ///< source clipping rectangle width
    uint32_t src_clip_h;	///< source clipping rectangle height
} emu_graph_params_t;

/// scancode indices
#define EMU_KEY_UP       0
#define EMU_KEY_DOWN     1
#define EMU_KEY_LEFT     2
#define EMU_KEY_RIGHT    3
#define EMU_KEY_O        4
#define EMU_KEY_X        5
#define EMU_KEY_SQUARE   6
#define EMU_KEY_TRIANGLE 7
#define EMU_KEY_R        8
#define EMU_KEY_L        9
#define EMU_KEY_SELECT  10
#define EMU_KEY_START   11
#define EMU_KEY_ESC     12	///< guess

typedef struct emu_keymap {
    int controller;		///< [in] controller ID (usually 0 or 1)
    uint32_t scancode[20];	///< [out] scan code to key code mapping
} emu_keymap_t;

#define EMU_FUNC_ENTRY(n)	(g_stEmuFuncs[(n) / 4])	///< @internal

/// @name Hooks
/// @{
extern int (*emuIfGraphInit) (emu_graph_params_t *);
extern int (*emuIfGraphShow) (void);
extern int (*emuIfGraphChgView) (emu_graph_params_t *);
extern int (*emuIfGraphCleanup) (void);
extern int (*emuIfSoundInit) (emu_sound_params_t *params);
extern uint32_t (*emuIfSoundPlay) (emu_sound_params_t *params);
extern uint32_t (*emuIfSoundCleanup) (emu_sound_params_t *params);

extern int (*emuIfunknown0c) (void *);  ///< sets the source buffer?
extern int (*emuIfKeyInit) (emu_keymap_t *map);
extern uint32_t (*emuIfKeyGetInput) (emu_keymap_t *map);
extern int (*emuIfKeyCleanup) (emu_keymap_t *map);
extern uint32_t (*emuIfGetCurTime) (void);  ///< @return could be uint64_t, not sure
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
/// @}

/// @}

/***************************************************************************/

/// @addtogroup spmp SPMP_SendSignal() command interface
/// @{

/// @name Hooks
/// @{
extern uint16_t (*SPMP_SendSignal) (uint16_t cmd, void *data, uint16_t size);
/// @}

/// @}

/***************************************************************************/

extern uint32_t *_gameMaxBufferSize_p;

extern int libgame_system_id;
#define SYS_UNKNOWN 0
#define SYS_JXD_100 1
#define SYS_JXD_1000 2
#define SYS_JXD_A1000 3
#define SYS_JXD_V3 4
#define SYS_LETCOOL 5
#define SYS_MMM 6

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif                          // __LIBGAME_H__
