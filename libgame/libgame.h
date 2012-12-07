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
/// Routines to make it easier to interact with the system hardware and software.
/// @{
#define	MAKE_RGB(r, g, b) (r & 0xff) | ((g & 0xff) << 8) | ((b & 0xff) << 16);
#define	MAKE_RGB565(r, g, b) (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3))

extern uint64_t libgame_utime(void);
/// Changes to the GAME directory.
/// This function changes the working directory to the GAME directory on the
/// current file system.
///
/// When a "native game" is started, the current directory is not well-defined.
/// It often is the directory the binary resides in, but even that is not
/// guaranteed. This function sets the current directory to a well-known
/// location, making it easier for you to find cofiguration and data files.
extern int libgame_chdir_game(void);

/// Enable/disable debug output.
/// Safe way of enabling/disabling debug output. Does nothing if the
/// required hook is not available.
/// @note The operating system prints a metric ass ton of debug output all the
/// time, even in frequently called routines (such as NativeGE_getKeyInput4Ntv()).
/// At least in some devices, debugging is turned on by default. This can
/// gravely impact the performance of your application and the system
/// as a whole. It is therefore recommended to turn debug output off.
extern void libgame_set_debug(int onoff);
/// @}

/***************************************************************************/

/// @defgroup nativege NativeGE interface
/// Official "native game" application programmer's interface

/***************************************************************************/

/// @addtogroup ge_key NativeGE user input
/// Keypad and touchscreen input interface.
/// @ingroup nativege
/// @{

/// @name Key Bit Masks
/// @{
#define	GE_KEY_UP	1
#define	GE_KEY_DOWN	2
#define	GE_KEY_LEFT	4
#define	GE_KEY_RIGHT	8
#define	GE_KEY_O	(1 << 16)
#define	GE_KEY_X	(2 << 16)
#define	GE_KEY_START	(1 << 13)
/// @}

/// @name Cybergame Key Mapping
/// @{
#define	CYBER_KEY_2		GE_KEY_O
#define	CYBER_KEY_3		GE_KEY_X
/// @}

/// Key state for controllers 1 and 2.
typedef struct ge_key_data {
    uint32_t key1;	///< Controller 1 key state
    uint32_t key2;	///< Controller 2 key state
} ge_key_data_t;

/// Touchscreen event.
typedef struct ge_tp_event {
    uint32_t type;
    uint32_t key;	///< not set by NativeGE_getTPEvent() (on the A16)
    uint16_t x;
    uint16_t y;
} ge_tp_event_t;

/// @name Hooks
/// @{

/// Get current key state.
/// This function is the "official" way for "native games" to read key
/// input. It provides the cooked (translated) state of the d-pad and
/// three buttons. All other buttons (and any additional controllers) the
/// device may have are not accessible. It is recommended to use the
/// [emulator key interface](@ref emu_key) instead, which provides both raw
/// and cooked data and supports all buttons and controllers a device has.
/// @param[out] keys key states of first controller
extern void (*NativeGE_getKeyInput4Ntv) (ge_key_data_t *keys);
/// Poll touchscreen events.
extern int (*NativeGE_getTPEvent) (ge_tp_event_t *);
/* extern char (*NativeGE_setTPClickArea) (void); doesn't do anything */
/// @}

/// @}

/***************************************************************************/

/// @addtogroup debug OS debug interface.
/// Serial debug interface used by the operating system.
/// @{

/// @name Hooks
/// @{
/// Print formatted debug output to serial port.
/// If debugging is enabled (see libgame_set_debug()), this routine will
/// print a message to the on-chip serial port.
/// @note This function is used extensively by the operating system.
extern void (*diag_printf) (char *fmt, ...);
/// @}

/// Enable/disable debug output.
/// See libgame_set_debug() for more information.
extern int *g_onoff_p;
/// Pointer to debug character out function pointer.
/// Can be used to redirect debug output.
extern void (**_diag_putc) (char);
/// @}

/***************************************************************************/

/// @addtogroup ge_gfx NativeGE graphics
/// @ingroup nativege
/// Native Game Environment graphics interface.
/// The "native game" graphics interface is of rather questionable quality. 
/// While it is apparently supposed to be a stable API to develop third-
/// party applications against, it seems to have received very little
/// testing. Even simple things (such as MCatchFillRect()) do not work
/// reliably. On top of that, the performance and flexibility leave quite a
/// bit to be desired.
///
/// It was, in fact, the low quality of this API that led me to further
/// develop the original libgame in the first place. I would suggest
/// to use the [emulator graphics interface](@ref emu) or
/// [direct hardware access](@ref hw) instead.
/// @{

/// @name Hooks
/// @{

/// Initialize MCatch graphics subsystem.
extern int (*MCatchInitGraph) (void);
/// Clears (not sets) the frame buffer.
extern int (*MCatchSetFrameBuffer) (int width, int height);
/// @brief Sets the display resolution...\ NOT!
/// @details Checks the parameter for sanity, then ignores it and sets the fixed
/// "native game" resolution of 320x240.
extern int (*MCatchSetDisplayScreen) (mcatch_point2d_t *point);
/// Enable double buffering and optionally clear the shadow buffer.
/// Has nothing obvious to do with the camera.
/// @param mode 32: enable double buffering; 33: clear shadow buffer, too
/// @return SPMP error code
extern int (*MCatchSetCameraMode) (int mode);
/// @param[in] color pointer to the color's three components
extern int (*MCatchSetFGColor) (uint8_t *color);
/// @param[out] color buffer for the color's three components
extern uint32_t (*MCatchGetFGColor) (uint8_t *color);
extern int (*MCatchSetColorROP) (uint8_t rop);
extern int (*MCatchSetAlphaBld) (uint8_t src_alpha, uint8_t dest_alpha);
extern int (*MCatchGetAlphaBld) (uint8_t *src_alpha, uint8_t *dest_alpha);
/// Fill a rectangle on the screen with the current foreground color.
/// On the A1000, this sometimes works and sometimes doesn't.
extern int (*MCatchFillRect) (mcatch_rect_t *rect);
/// Enable graphics pipeline features.
/// Feature codes:
/// - 0: set out color order to 0
/// - 1: set render type to 1
/// - 3: set render type to 0
/// - 4: set destination context clip rectangle
/// - 5: set destination context color read mask
/// - 6: set destination context color write mask
/// - 7: set stipple type to 1
/// - 8: enable Java game acceleration(?) and rotation
/// - 9: set stipple type
/// - 10: enable stipple for lines
/// - 11: enable gradient for lines(?)
/// - 12: all of the above (except set render type to 0)
///
/// @param feature feature code
extern int (*MCatchEnableFeature) (uint32_t feature);
/// Flush something. Or not.
/// I have not seen any firmware yet in which this isn't a NOP.
extern int (*MCatchFlush) (void);
/// Blits the MCatch display buffer to the hardware frame buffer.
extern int (*MCatchPaint) (void);
/// Create a surface.
/// @param[out] imgid
extern int (*MCatchLoadImage) (mcatch_loadimg_t *loadimg, uint8_t *imgid);
/// Destroy a surface.
extern int (*MCatchFreeImage) (uint8_t img_id);
/// Blit a surface to the screen.
extern int (*MCatchBitblt) (uint8_t img_id, mcatch_rect_t *rect, mcatch_point2d_t *at);
extern int (*MCatchSprite) (uint8_t img_id, mcatch_rect_t *rect, mcatch_point2d_t *at);

/* extern int (*MCatchStoreImage) (void); doesn't do anything */
/* extern int (*MCatchDecodeImageFromCard) (void); doesn't do anything */

/// @param[out] rop current color ROP
extern int (*MCatchGetColorROP) (uint8_t *rop);
/// @param[in] read_write 0 to set read mask, 1 to set write mask
extern int (*MCatchSetBitPlaneMask) (int read_write, uint16_t mask);
/// @param[in] read_write 0 for read mask, 1 for write mask
/// @param[out] mask
extern int (*MCatchGetBitPlaneMask) (int read_write, uint16_t *mask);
/// Get screen dimensions.
/// @param[out] screen
extern int (*MCatchGetDisplayScreen) (mcatch_rect_t *screen);
/// Set destination context clipping rectangle.
/// @param[in] clip
/// @return SPMP error code
extern int (*MCatchSetRectClip) (mcatch_rect_t *clip);
/// Get destination context clipping rectangle.
/// @param[out] clip
extern int (*MCatchGetRectClip) (mcatch_rect_t *clip);
/// @param[in] mask
extern int (*MCatchSetStyleMask) (mcatch_rect_t *mask);
/// @param[out] mask
extern int (*MCatchGetStyleMask) (mcatch_rect_t *);
/// @param[in] mask
extern int (*MCatchSetLineMask) (mcatch_rect_t *mask);
/// @param[out] mask
extern int (*MCatchGetLineMask) (mcatch_rect_t *);
/// Disable graphics pipeline features.
/// Feature codes:
/// - 0: set out color order to 1
/// - 1: set render type to 0
/// - 3: set render type to 1
/// - 4: disable destination context clip rectangle
/// - 5: disable destination context read color mask
/// - 6: disable destination context write color mask
/// - 7: set stipple type to 0
/// - 8: disable Java game acceleration and rotation
/// - 9: disable stipple
/// - 10: disable stipple for lines
/// - 11: disable gradient for lines(?)
/// - 12: all of the above (except set render type to 1)
///
/// @param feature feature code
extern int (*MCatchDisableFeature) (int);
/// Set line style.
/// @param unknown only checked for compliance (must be at most 64)
/// @param style 0 for solid line, 1 for dashed (guess)
/// @return SPMP error code
extern int (*MCatchSetStyleLine) (uint8_t unknown, uint8_t style);

/* extern int (*MCatchPreviewColorkey) (void); doesn't do anything */

/// Get frame buffer dimensions.
/// @param[out] width
/// @param[out] height
extern int (*MCatchGetFrameBuffer) (uint16_t *width, uint16_t *height);
extern int (*MCatchSetMutableImage) (uint8_t);
/// @param equation 0 or 1
/// @return SPMP error code
extern int (*MCatchSetPerPixelAlphaEq) (uint8_t equation);
/// Set transformation.
/// Transformations supported:
/// - 0: no transformation
/// - 1: mirroring, rotation 2
/// - 2: mirroring, no rotation
/// - 3: no mirroring, rotation 2
/// - 4: mirroring, rotation 3
/// - 5: no mirroring, rotation 1
/// - 6: no mirroring, rotation 3
/// - 7: mirroring, rotation 1
///
/// @param reference screen point of reference
/// @param transformation type of transformation
extern int (*MCatchSetTransformation) (mcatch_point2d_t *reference, int transformation);
/// @param img_id surface ID
/// @param unknown1 accepts 0-3
/// @return (non-standard) error code
extern int (*MCatchQueryImage) (uint8_t img_id, uint8_t unknown1);
/// Enable or disable double buffering.
/// @note This only affects the "native" graphics interface.
/// @param enable 0 to disable, 1 to enable
extern int (*MCatchEnableDoubleBuffer) (int enable);
/// Fill a rectangle on the screen with a color gradient.
/// @param rect rectangle to be filled
extern int (*MCatchGradientFill) (mcatch_rect_t *rect, uint16_t[6], uint32_t[2]);

/* extern int (*MCatchUpdateScreen) (void); doesn't do anything (except produce debug output) */

/// @param unknown1 must be 24 or lower
/// @param unknown2 must be 24 or lower
extern int (*MCatchShowFont) (mcatch_point2d_t *unknown0, int, uint8_t unknown1,
                              uint8_t unknown2);
/// Change a portion of the palette.
/// @param imd_id surface ID
/// @param start first color to be changed
/// @param size size of color data in bytes
/// @param colors pointer to color data
extern int (*MCatchModifyPalette) (uint8_t img_id, uint8_t start, uint8_t size, void *colors);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_res NativeGE audio
/// @ingroup nativege
/// Native Game Environment audio interface.
/// The "native game" audio interface provides facilities for parsing and
/// playing MIDI and WAV files.
///
/// The WAV support is almost comically limited and supports no more than
/// a single 8 kHz sound to be played at a time. I would suggest to use
/// the [emulator sound interface](@ref emu) instead.
///
/// See an [example program](@ref audio/game.c).
/// @{

typedef struct ge_res_entry {
    char name[16];
    uint8_t *res_data;
} ge_res_entry_t;

/// Audio resource information.
typedef struct {
    char *data;
    int size;
} ge_res_info_t;

/// @name Hooks
/// @{

/// Load resource table.
/// This function installs an array of ge_res_entry_t structures as the
/// resource table that will later be used by NativeGE_getRes() to
/// retrieve audio resources.
/// @param val ignored
/// @param[in] res_table table of resources
extern int (*NativeGE_initRes) (int val, ge_res_entry_t *res_table);
/// Get audio resource information.
/// This function fills in a ge_res_info_t structure that can be passed
/// to NativeGE_playRes().
/// @param[in] name name as specified in the resource table
/// @param[out] res_info resource info
/// @return resource type (must be passed to NativeGE_playRes())
extern int (*NativeGE_getRes) (char *name, ge_res_info_t *res_info);
/// Play back audio resource.
/// Plays an audio resource. Only one resource of a type can be played at
/// a time.
/// @param res_type resource type as returned by NativeGE_getRes()
/// @param repeat repeat count (0 means forever)
/// @param res_info resource info
extern int (*NativeGE_playRes) (uint8_t res_type, int repeat, ge_res_info_t *res_info);
/// Stop audio playback.
/// @param res_type resource type as returned by NativeGE_getRes()
extern int (*NativeGE_stopRes) (int res_type);
/// Pause audio playback.
/// @param res_type resource type as returned by NativeGE_getRes()
extern void (*NativeGE_pauseRes) (uint8_t res_type);
/// Resume audio playback.
/// @param res_type resource type as returned by NativeGE_getRes()
extern void (*NativeGE_resumeRes) (uint8_t res_type);

/// @note This hook is only available on recent firmwares.
extern uint16_t (*NativeGE_SPUCommand) (uint16_t, uint32_t);
/// @}

/// @example audio/game.c

/// @}

/***************************************************************************/

/// @addtogroup ge_control NativeGE thread control
/// Game thread control.
/// These functions are mostly used by the operating system to control the
/// game thread.
/// @ingroup nativege
/// @{

/// @name Hooks
/// @{

/// Ends the game thread.
extern int (*NativeGE_gameExit) (void);
/// Pauses the game thread.
extern int (*NativeGE_gamePause) (void);
/// Resumes the game thread.
extern int (*NativeGE_gameResume) (uint32_t);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_misc NativeGE miscellaneous
/// Native Game Environment hooks that do not fit any other category.
/// @ingroup nativege
/// @{

/// @name Hooks
/// @{

/// Get time since application start.
/// @note This function returns milliseconds, but the actual resolution is
/// one eCos system tick (10 ms). If you need more precise timing, use
/// libgame_utime().
/// @return time since application start (ms)
extern uint32_t (*NativeGE_getTime) (void);     /* returns system ticks
                                                   multiplied by 10 */
/* extern void (*NativeGE_showFPS) (void); doesn't do anything */
/// @}

/// @}

/***************************************************************************/

/// @addtogroup init Initialization and finalization
/// Library initialization and finalization routines.
/// @{

/// Initialize libgame interfaces.
/// This function performs a number of initializations:
/// - Initialize [NativeGE](@ref nativege) hooks passed from the operating system.
/// - Perform firmware analysis and setup "non-official" interfaces
///   ([emulator](@ref emu), [eCos](@ref ecos), [hardware access](@ref hw),
///   [SPMP](@ref spmp))
/// - Detect system type (see @ref libgame_system_id).
/// - Determine available memory.
///
/// This function must be called before any other libgame interfaces can be used.
/// @todo libgame_init() should really be called automatically by the start-up
/// code because nothing works without it anyway.
void libgame_init(void);
/// @}

/***************************************************************************/

/// @addtogroup dmsg Graphical Debug Console
/// On-screen debug output console.
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
/// eCos operating system interface.
/// The SPMP8000 firmware is based on the eCos operating system, which
/// provides standard-conforming mechanisms for file system access and
/// threading. libgame makes these interfaces accessible to your
/// application because their functionality is not available or has been
/// artificially limited in the "official" SPMP8000 programmer's
/// interfaces.
/// @warning The data types and constants used by eCos are different
/// from those used by newlib. Do not mix them! Whenever you call
/// functions with an \c _ecos prefix you should only use data types
/// with an \c _ecos prefix.
/// @{

/// @addtogroup ecos_fs File system interface
/// File system access.
/// This interface provides you access to the file systems at the lowest
/// possible level, including the ROM file system, without any filtering
/// or workarounds. It is fully POSIX-compatible, although some of the
/// more esoteric features are missing.
///
/// libgame glues this interface to newlib's file interface, so you can
/// (and probably should) use the functions from newlib without the
/// @c _ecos prefix. It also means you can use the standard I/O interface
/// (fopen() and friends).
/// @ingroup ecos
/// @{

typedef void _ecos_DIR;		///< eCos directory handle
#define _ecos_NAME_MAX 256	///< Maximum path length

/// eCos Directory entry.
/// @note The SPMP8000 firmware's eCos has been configured without support
/// for the \c d_type field. If you need to distinguish between files and
/// directories, you have to use stat() or _ecos_stat().
struct _ecos_dirent {
    char d_name[_ecos_NAME_MAX + 1];
};

/// @name _ecos_stat() data types
/// @{
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

/// @note Many of these fields (e.g. st_mode, st_nlink, st_uid, st_gid)
/// are meaningless because the underlying features are not supported by
/// the FAT or romfs file systems.
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
/// @}

/// @name _ecos_open() flags
/// @{
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
/// @}

/// @name Hooks
/// @{
extern int (*_ecos_close) (int fd);
extern int (*_ecos_read) (int fd, void *buf, unsigned int count);
extern int (*_ecos_write) (int fd, const void *buf, unsigned int count);
extern int (*_ecos_lseek) (int fd, int offset, int whence);
extern int (*_ecos_fstat) (int fd, struct _ecos_stat * buf);
extern int (*_ecos_open) (const char *pathname, int flags, int mode);
extern _ecos_DIR *(*_ecos_opendir) (const char *name);
/// Get a pointer to eCos's \c errno.
extern int *(*_ecos_cyg_error_get_errno_p) (void);
extern void *_ecos_cyg_fd_alloc;
extern struct _ecos_dirent *(*_ecos_readdir) (_ecos_DIR * dirp);
extern int (*_ecos_readdir_r) (_ecos_DIR * dirp, struct _ecos_dirent * entry,
                               struct _ecos_dirent ** result);
extern int (*_ecos_closedir) (_ecos_DIR * dirp);
extern int (*_ecos_stat) (const char *path, struct _ecos_stat * buf);
/// Get current working directory.
/// eCos's getcwd() function is somewhat quirky: It omits part of the path.
/// For instance, the @c GAME directory on the built-in flash file system
/// is @c /fat20a2/hda2/GAME. _ecos_getcwd(), however, returns
/// @c /hda2/GAME instead.
/// @note The glue code provides a getcwd() function that returns the
/// correct path, and you should use it unless you have a good reason not to.
extern char *(*_ecos_getcwd) (char *buf, size_t size);
/// Change current working directory.
/// @warning eCos's chdir() function appears to be buggy: it does not always
/// return an error when the operation cannot be performed, so you have to
/// use getcwd() to check.
extern int (*_ecos_chdir) (const char *path);
extern int (*_ecos_unlink) (const char *pathname);
extern int (*_ecos_rmdir) (const char *pathname);
extern int (*_ecos_mkdir) (const char *pathname, _ecos_mode_t mode);
extern int (*_ecos_fsync) (int fd);
/// @}

/// @}

/// @name Hooks
/// @{

/// Delay the current thread by a number of ticks.
/// @param delay Delay in system ticks [cyg_tick_count_t, actually]
extern void (*cyg_thread_delay) (uint64_t delay);
/// Get system time in ticks.
/// eCos as configured in the SPMP8000 firmware keeps system time with a 100
/// Hz resolution.
/// @return current time (ticks)
extern uint64_t (*cyg_current_time) (void);

/// @}

/// @}

/***************************************************************************/

/// @addtogroup ge_fs NativeGE file system access
/// @ingroup nativege
/// Native Game Environment file system access.
/// The "native game" file system interface provides basic facilities for
/// file system access. It is restricted to the read/write file systems
/// (built-in flash memory and SD card); it is not possible to access the
/// ROM file system with it, and it does not provide any mechanism to
/// read directories. If you want to do these things (and probably in most
/// other cases as well), you should use the standard POSIX-like file
/// control and standard I/O functions provided by newlib (open(), fopen(),
/// and friends), or the [eCos system interface](@ref ecos).
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

/// Open a file.
extern int (*NativeGE_fsOpen) (const char *filename, int flags, int *fd);
/// Read data from a file.
/// @return 0 (okay), 2 (error)
extern int (*NativeGE_fsRead) (int fd, const void *buf, size_t count, int *result);
/// Write data to a file.
extern int (*NativeGE_fsWrite) (int fd, const void *buf, size_t count, int *result);
/// Close a file.
extern int (*NativeGE_fsClose) (int fd);
/// Set position within a file.
extern uint64_t (*NativeGE_fsSeek) (int fd, int offset, int whence);

/// Write data to a file at a specific offset.
extern int (*NativeGE_writeRecord) (const char *pathname, void *buf, uint8_t flags,
                                    _ecos_off_t offset, _ecos_size_t count);
/// Read data from a file at a specific offset.
extern int (*NativeGE_readRecord) (const char *pathname, void *buf, uint8_t flags,
                                   _ecos_off_t offset, _ecos_size_t count);
/// @}

#define tell(fd) (NativeGE_fsSeek(fd, 0, SEEK_CUR) >> 32)

/// @}

/***************************************************************************/

/// @addtogroup hw Low-level hardware access
/// Direct access to display devices, CPU clock, and high-resolution timer.
/// @{
typedef struct {
    /// Set the frame buffer pixel format.
    /// @param format pixel format number
    void (*setBuffFormat) (int format);
    /// Get the frame buffer pixel format.
    /// @return pixel format number
    int (*getBuffFormat) (void);
    /// Get the display device width.
    /// @return display width (pixels)
    int (*getWidth) (void);
    /// Get the display device height.
    /// @return display height (pixels)
    int (*getHeight) (void);
    /// Get a pointer to the shadow buffer.
    /// @return shadow buffer pointer
    uint16_t *(*getShadowBuffer) (void);
    /// Set the frame buffer.
    /// @param[in] fb frame buffer pointer
    void (*setFrameBuffer) (uint16_t *fb);
    /// Exchange (flip) shadow and frame buffer.
    /// @note flip() waits until the vertical blank period before switching
    /// the buffers if (and only if) the frame and shadow buffers are
    /// different. It is therefore possible to disable double-buffering
    /// (and the associated blocking behavior of flip()) by setting the
    /// shadow buffer to the frame buffer or vice versa.
    void (*flip) (void);
    /// Clear the shadow buffer.
    void (*clear) (void);    //< actually returns BitBlt_hw retval, but that is always 0
    /// Set the shadow buffer.
    /// @param[in] fb shadow buffer pointer
    void (*setShadowBuffer) (uint16_t *fb);
    /// Get a pointer to the frame buffer.
    /// @return frame buffer pointer
    uint16_t *(*getFrameBuffer) (void);
} disp_device_t;

/// Global pointer to the currently active display device.
/// @note To avoid graphics corruption it is important to call cache_sync()
/// after modifying the frame buffer (or before flipping the shadow buffer)
/// to make sure that the changes are actually committed to memory.
extern disp_device_t *gDisplayDev;

/// @name Hooks
/// @{
/// Synchronize data cache.
extern void (*cache_sync) (void);

/// Get current processor core frequency.
/// @return frequency (Hz)
extern int (*GetArmCoreFreq) (void);
/// Set processor frequency.
/// This function only sets the CPU clock divider. This means that it
/// is not possible to set arbitrary values here, and changeARMFreq()
/// will use the nearest possible value.
/// @param mhz frequency (MHz)
extern int (*changeARMFreq) (int mhz);

/// Read the high-speed timer.
/// This function reads an on-chip interval timer that is used to update
/// the eCos system tick clock. It is mainly used by libgame_utime().
extern void (*hal_clock_read) (uint32_t *us);
/// @}

/// @}

/***************************************************************************/

/// @addtogroup emu Emulator interface (emuIf)
/// Emulator application interface.
/// The emulator API (emuIf) is provided for the benefit of the built-in
/// emulators shipped with all SPMP8000 devices. It supports the typical
/// range of functionality required by an emulator:
/// - Scaled and clipped video output.
/// - Streaming audio output.
/// - Timing functions.
/// - Key input.
/// - Limited file system access.
///
/// These interfaces are not provided to "native game" applications by the
/// operating system; libgame uses heuristics to spot their location and
/// makes them accessible to you.
///
/// The most interesting parts are the [video](@ref emu_graph) and
/// [audio](@ref emu_sound) interfaces, which are much more capable than
/// anything that is provided as part of the "offical" third-party API.
/// @{

/// Operating system callbacks structure.
/// See [g_stEmuAPIs](@ref g_stEmuAPIs) for information on how to use this.
typedef struct {
    void *_unknown;
    int (*pause) (void);
    int (*resume) (uint32_t);
    int (*exit) (uint32_t);
    int (*save) (int);
    int (*load) (int);
} emu_apis_t;

/// Operating system callbacks.
/// The functions registered here are called when certain events take place:
/// - The application is paused.
/// - The application is resumed.
/// - The application is terminated.
/// - The state is to be saved.
/// - The state is to be loaded.
///
/// The operating system initializes this structure with generic functions
/// from the [NativeGE](@ref nativege) interface, namely NativeGE_pause(),
/// NativeGE_gameResume(), and NativeGE_gameExit().
///
/// To disable a callback, set it to NULL.
extern emu_apis_t *g_stEmuAPIs;

/// Emulator hooks table.
/// This table contains all hooks to the emulator interface functions.
/// libgame provides you with a global hook for every function, so you
/// will most likely never have to use this.
extern void **g_stEmuFuncs;

/// @}

/// @addtogroup emu_sound Sound interface
/// Streaming sound output interface.
/// This interface allows you to output a continuous generated audio stream.
/// The sample rate and number of channels (one or two) can be chosen freely.
/// The sample format is always 16-bit signed.
/// @ingroup emu
/// @{
typedef struct emu_sound_params {
    uint8_t *buf;               ///< sound buffer
    uint32_t buf_size;          ///< sound buffer size (bytes)
    uint32_t rate;              ///< sample rate (Hz)
    uint8_t depth;              ///< does not seem to do anything at all
    uint8_t channels;           ///< number of channels (1 or 2)
    uint32_t *callback;         ///< used only in newer firmwares when gEmuType is 5 (flash?)
} emu_sound_params_t;

/// @name Hooks
/// @{
extern int (*emuIfSoundInit) (emu_sound_params_t *params);
extern uint32_t (*emuIfSoundPlay) (emu_sound_params_t *params);
extern uint32_t (*emuIfSoundCleanup) (emu_sound_params_t *params);
/// @}

/// @}

/// @addtogroup emu_graph Graphics interface
/// Graphics interface with hardware scaling and scrolling.
/// @ingroup emu
/// @{
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

/// @name Hooks
/// @{

/// Initialize the emulator graphics subsystem.
/// This function has to be called to set up the graphics context. You have
/// to fill in the emu_graph_params_t structure beforehand and allocate
/// a sufficiently sized source buffer to hold a bitmap of the given
/// dimensions.
/// @param[in] gp graphics settings
extern int (*emuIfGraphInit) (emu_graph_params_t *gp);
/// Update the screen.
/// This function performs scaling and conversion before blitting your
/// source buffer to the screen. It also calls disp_device_t::flip()
/// afterwards.
extern int (*emuIfGraphShow) (void);
/// Change graphics settings.
/// If you change the graphics settings after the call to emuIfGraphInit()
/// you have to call this function to register the new settings with the
/// graphics subsystem.
/// @param[in] gp graphics settings
extern int (*emuIfGraphChgView) (emu_graph_params_t *gp);
/// Finalize the emulator graphics subsystem.
/// Call this function to finalize the graphics subsystem before exiting
/// your program.
extern int (*emuIfGraphCleanup) (void);
/// @}

/// @}

/// @addtogroup emu_key Key input interface
/// Raw and translated key input access.
/// @ingroup emu
/// @{

/// @name Key map indices
/// These constants should be used to find the mask for a key in an emu_keymap_t.
/// @{
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
/// @}

typedef struct emu_keymap {
    int controller;		///< [in] controller ID (usually 0 or 1)
    uint32_t scancode[20];	///< [out] key code to scancode mapping
} emu_keymap_t;

/// @name Hooks
/// @{

/// Initialize emulator input subsystem.
/// This function initializes the emulator key input subsystem and provides
/// you with a mapping for the given controller that tells you which bit
/// in the raw (and thus device-specific) key state bitmap returned by
/// emuIfKeyGetInput() correspond to a specific key. You have to set
/// emu_keymap_t::controller to the number of the controller you want to
/// get mappings for. If a controller is not available, an all-zero mapping
/// will be returned.
/// @note While this mapping is generally more comprehensive than the
/// translation provided by NativeGE_getKeyInput4Ntv(), it does not
/// necessarily provide all available hardware keys. Generally, at least the
/// d-pad, X, O, START, and SELECT buttons will be mapped. If your
/// application requires or benefits from having a large number of keys you
/// may want to give the user the opportunity to map the keys herself so all
/// available buttons can be used.
/// @param[in,out] map key code to raw key state bitmap translation table
extern int (*emuIfKeyInit) (emu_keymap_t *map);
/// Get current key state.
/// Returns the raw key state bitmap of the given controller. Use "map" to
/// test the returned value for specific keys.
/// @param[in] key map (only emu_keymap_t::controller is used)
/// @return raw key state bitmap
extern uint32_t (*emuIfKeyGetInput) (emu_keymap_t *map);
/// Finalize the emulator input subsystem.
/// Call this function before exiting your program if you have previously
/// initialized the key input subsystem using emuIfKeyInit().
/// @param[in] map
extern int (*emuIfKeyCleanup) (emu_keymap_t *map);
/// @}

/// @}

/// @addtogroup emu_time Timing interface
/// Time-related functions.
/// The emulator timing interface does not provide any functionality that
/// isn't provided in a more consistent and competent way by other
/// interfaces.
/// @ingroup emu
/// @{

/// @name Hooks
/// @{

/// Get time since start of application.
/// @note While this function returns milliseconds, the actual resolution
/// is 10 ms (one eCos system tick). If you need more precise timing, use
/// libgame_utime().
/// @return time in milliseconds
extern uint32_t (*emuIfGetCurTime) (void);
/// Pause execution of application thread for some time.
/// @note It is unclear to me what unit of time this function expects. The
/// SK8607 and A1000 implementations do the following:
/// 1. If the value is less than 10, call \c cyg_thread_delay(1).
/// 2. If the value is 10 or more, call \c cyg_thread_delay(value / 40).
///
/// @note If you need a delay function that works in a well-defined manner you
/// should probably call cyg_thread_delay() directly.
extern void (*emuIfTimeDelay) (uint32_t);
/// @}

/// @}

/// @addtogroup emu_fs File system interface
/// @ingroup emu
/// File system access.
/// The emulator file system interface imposes fewer restrictions on the
/// user than the [NativeGE file interface](@ref ge_fs), but is still a
/// bit limited: there is no directory access. I recommend to use newlib's
/// POSIX-like file or standard I/O interface, or the
/// [eCos file interface](@ref ecos).
/// @{

/// @name Hooks
/// @{

/// Open a file.
/// Flags:
/// - 0: read-only
/// - 1: write-only
/// - 3: create, write-only
/// - 7: read/write
/// - 15: truncate, read-only (does that even make sense?)
/// @param[in] pathname path to file
/// @param[in] flags
/// @return file descriptor
extern int (*emuIfFsFileOpen) (const char *pathname, uint32_t flags);
/// Get the file size of an open file.
/// @param[in] fd file descriptor
/// @return size
extern uint32_t (*emuIfFsFileGetSize) (int fd);
/// Write data to a file.
/// @param[in] fd file descriptor
/// @param[in] buf data
/// @param[in] count byte count
/// @return number of bytes written (0 on error)
extern int (*emuIfFsFileWrite) (int fd, const void *buf, uint32_t count);
/// Read data from file.
/// @param[in] fd file descriptor
/// @param[out] buf data buffer
/// @param[in] count byte count
/// @return number of bytes read (0 on error)
extern int (*emuIfFsFileRead) (int fd, void *buf, uint32_t count);
/// Read byte from file.
/// @param[in] fd file descriptor
/// @return byte read (0 means there is either an error, or an actual zero
/// byte has been read. Yes, that is stupid.)
extern int (*emuIfFsFileGetChar) (int fd);
/// Set position within the file.
/// @param[in] fd file descriptor
/// @param[in] offset desired position
/// @param[in] whence FS_SEEK_SET, FS_SEEK_CUR, or FS_SEEK_END
/// @return bullshit; seriously, just use lseek() or _ecos_lseek()
extern int (*emuIfFsFileSeek) (int fd, _ecos_off_t offset, uint8_t whence);
/// Get current position within the file.
/// @param[in] fd file descriptor
/// @return current position (-1 on error)
extern _ecos_off_t (*emuIfFsFileCurPos) (int fd);
/// Close file.
/// @param[in] fd file descriptor
/// @return 0 on success, -1 on error
extern int (*emuIfFsFileClose) (int fd);
/// @}

/// @}

/// @addtogroup emu_other Other functions
/// @ingroup emu
/// Miscellaneous functions from the new emulator ABI.
/// There are two versions of the emulator ABI that have been spotted in the
/// wild:
/// - The "old" ABI contains 20 function hooks, the purposes of which are
///   well-known.
/// - The "new" ABI (which is found in most devices) adds the 8 additional
///   function hooks presented here. Not much is known about them yet.
/// @{

/// @name Hooks
/// @{
extern int (*emuIfunknown0c) (void *);  ///< sets the source buffer?
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

#define EMU_FUNC_ENTRY(n)	(g_stEmuFuncs[(n) / 4])

/***************************************************************************/

/// @addtogroup spmp SPMP command interface
/// An interface to send commands to the SPMP system.
/// The SPMP system provides operating system facilities for the built-in
/// "media player" applications.
/// @note Due to the nature of this interface we know only a few of the
/// provided commands by name. These commands are marked as "canonical".
/// All other command names are educated (or, in some cases, wild) guesses.
/// @{

/// @name Hooks
/// @{

/// Send SPMP system command.
/// @note Different commands require different data types to be passed as
/// \c data. The correct type can be found in the descriptions of the
/// individual commands.
/// @param[in] cmd command ID
/// @param[in,out] data pointer to input or output data
/// @param[in] size size of \c data
/// @return SPMP error code
extern uint16_t (*SPMP_SendSignal) (uint16_t cmd, void *data, uint16_t size);
/// @}

/// @example diag/game.c

/// @}

/***************************************************************************/

extern uint32_t *_gameMaxBufferSize_p;

/// @addtogroup utility
/// @{

/// @name System ID
/// @{

/// System ID detected during initalization.
/// libgame uses the presence or absence of certain files to find out
/// what specific machine it is running on. This does not currently work
/// very well, so you should take this information with a grain of salt.
extern int libgame_system_id;

#define SYS_UNKNOWN 0
#define SYS_JXD_100 1
#define SYS_JXD_1000 2
#define SYS_JXD_A1000 3
#define SYS_JXD_V3 4
#define SYS_LETCOOL 5
#define SYS_MMM 6
/// @}

/// @}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif                          // __LIBGAME_H__
