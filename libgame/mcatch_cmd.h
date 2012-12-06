/* mcatch_cmd.h - SPMP_SendSignal() commands and data structures; part of libgame
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

/// @file

#ifndef __SPMP_H__
#define __SPMP_H__

#include <stdint.h>

/// @addtogroup system System commands
/// Various system settings and information.
/// @ingroup spmp
/// @{

typedef struct {
    uint32_t audio_volume;         /**< [in,out] +00 */
    uint16_t panel_brightness;     /**< [in,out] +04 */
    uint16_t equalizer_mode;       /**< [in] +06 */
    uint16_t dolby_enabled;        /**< [in] +08; 0x10 enables, all other values disable. */
    uint16_t pwr_off_sec;          /**< [in] +0a */
    uint16_t set_audio_volume;     /**< [in] +0c */
    uint16_t set_panel_brightness; /**< [in] +0e */
    uint16_t set_equalizer;        /**< [in] +10 */
    uint16_t set_dolby;            /**< [in] +12 */
    uint16_t set_pwr_off_sec;      /**< [in] +14 */
    uint16_t earphone_detected;    /**< [out] +16 */
    uint32_t lcd_width;            /**< [out] +18 */
    uint32_t lcd_height;           /**< [out] +1c */
    uint32_t osd_width;            /**< [out] +20 */
    uint32_t osd_height;           /**< [out] +24 */
} mcatch_sysinfo_t;

/// Change various system settings.
/// Which settings are to be changed is controlled by setting the
/// corresponding set_* member to 0 or 1.
/// @param[in] data [mcatch_sysinfo_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SET_SYSINFO		0x001
/// Retrieve various system settings and invariants.
/// @param[out] data [mcatch_sysinfo_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_GET_SYSINFO		0x002

/// Shut down system.
/// @return error code
#define MCATCH_CMD_SYSTEM_POWER_OFF		0x003 /* canonical */
/// Get battery voltage (guess).
/// @param[out] data voltage [uint8_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_CAL_VOLTAGE		0x004

typedef struct {
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
    uint32_t day;
    uint32_t month;
    uint32_t year;
} mcatch_system_time_t;

/// Get system time from real-time clock.
/// @param[out] data [mcatch_system_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_TIME_GET		0x005
/// Set real-time clock.
/// @param[in] data [mcatch_system_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_TIME_SET		0x006
/// Get alarm time/date (guess).
/// @param[out] data [mcatch_system_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_ALARMTIME_GET		0x007
/// Set alarm time/date (guess).
/// @param[in] data [mcatch_system_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_ALARMTIME_SET		0x008
/// Disable alarm.
/// @return error code
#define MCATCH_CMD_SYSTEM_ALARMTIME_DISABLE	0x009 /* canonical */

typedef struct {
    uint32_t __unused_00;
    uint32_t suspend_time;
} mcatch_suspend_time_t;

/// Enable system suspend (guess).
/// @param[in] data Time until suspend. [mcatch_suspend_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SUSPEND_ENABLE	0x00A /* probably canonical */
/// Disable system suspend (guess).
/// @return error code
#define MCATCH_CMD_SYSTEM_SUSPEND_DISABLE	0x00B /* canonical */

/// Enable TV output.
/// @return error code
#define MCATCH_CMD_SYSTEM_TVOUT_ENABLE		0x00C
/// Disable TV output.
/// @return error code
#define MCATCH_CMD_SYSTEM_TVOUT_DISABLE		0x00D

typedef struct {
    int8_t __unknown_00; /**< [in] */
    int8_t __unknown_01; /**< [in] */
    int8_t __unknown_02; /**< [out] */
    int8_t __unknown_03; /**< [in] */
    int8_t __unknown_04; /**< [out] */
} mcatch_system_update_t;

/// Update system from internal storage.
/// @warning This function writes bootloader and romfs. Don't use unless
/// you know exactly what you're doing. And you don't!
/// @param[in,out] data [mcatch_system_update_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_UPDATE		0x00E	/* careful there! */

/// Sets system time without updating the real-time clock.
/// The purpose of this is unclear.
/// @param[in] data [mcatch_system_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_TIME_SET_NO_RTC	0x00F

/// Check for presence of an SD card.
/// @return SD card present (guess).
#define MCATCH_CMD_SYSTEM_IS_SD_PLUGGED_IN	0x010

/// Turn off speaker.
/// @return error code
#define MCATCH_CMD_SYSTEM_SPEAKER_OFF		0x011
/// Turn on speaker.
/// @return error code
#define MCATCH_CMD_SYSTEM_SPEAKER_ON		0x012

/// Lock keyboard.
/// May only apply to MicroWindows.
/// @return error code
#define MCATCH_CMD_SYSTEM_KEY_LOCK		0x013
/// Unlock keyboard.
/// May only apply to MicroWindows.
/// @return error code
#define MCATCH_CMD_SYSTEM_KEY_UNLOCK		0x014

typedef struct {
    uint32_t __unknown_00;
    uint32_t __unknown_08;
    uint32_t suspend_time;	/**< [in] +0c */
} mcatch_mmisuspend_t;

/// Enable MMI suspend.
/// @param data [mcatch_mmisuspend_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_MMISUSPEND_ENABLE	0x015 /* canonical */
/// Disable MMI suspend.
/// @return error code
#define MCATCH_CMD_SYSTEM_MMISUSPEND_DISABLE	0x016 /* canonical */

/* #define MCATCH_CMD_SYSTEM_NOP		0x017 */

/// Start "detect thread".
/// This thread appears to be responsible for hardware detection (earphones,
/// for instance) and handling of power management events.
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_DETECT_START	0x018
/// Stop "detect thread".
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_DETECT_STOP	0x019

/// Set audio volume.
/// @param[in] data audio volume (0 - 31) [uint32_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_AUDIO_VOLUME_SET	0x01A

/// Set LCD panel brightness.
/// Note that setting the display brightness is not implemented in all devices.
/// @param[in] data [mcatch_sysinfo_t *, uses mcatch_sysinfo_t::panel_brightness]
/// @return error code
#define MCATCH_CMD_SYSTEM_SET_PANEL_BRIGHTNESS	0x01B
/// Set equalizer mode.
/// @param[in] data [mcatch_sysinfo_t *, uses mcatch_sysinfo_t::equalizer_mode]
/// @return error code
#define MCATCH_CMD_SYSTEM_EQ_SET		0x01C
/// Enable/disable Dolby.
/// @param[in] data [mcatch_sysinfo_t *, uses mcatch_sysinfo_t::dolby_enabled]
/// @return error code
#define MCATCH_CMD_SYSTEM_DOLBY_SW		0x01D
/// Set power-off seconds (guess).
/// @param[in] data [mcatch_sysinfo_t *, uses mcatch_sysinfo_t::pwr_off_sec]
/// @return error code
#define MCATCH_CMD_SYSTEM_SET_PWR_OFF_SEC	0x01E

/// Initialize FM radio hardware (guess).
/// @return error code
#define MCATCH_CMD_SYSTEM_FM_INIT		0x01F

typedef struct {
    uint32_t cmd;		/**< [in] +00 */
    uint32_t start_freq;	/**< [in] +04 */
    uint32_t end_of_band;	/**< [out] +08; set if end of frequency band
                                           has been reached while searching */
    uint32_t end_freq;		/**< [out] +0c (guess) */
    uint32_t freq_range;	/**< [in] +10 */
    uint32_t sensitivity;	/**< [in} +14 */
} mcatch_fm_t;

#define FM_CMD_SEARCH_UP	0xA0	/**< search upwards from mcatch_fm_t::start_freq */
#define FM_CMD_SEARCH_DOWN	0xB0	/**< search downwards from mcatch_fm_t::start_freq */
#define FM_CMD_SET_FREQUENCY	0xC0	/**< set frequency to mcatch_fm_t::start_freq */

/// Send command to FM radio.
/// @param[in] data [mcatch_fm_t *, uses mcatch_fm_t::start_freq (in),
///                  mcatch_fm_t::end_of_band (out), mcatch_fm_t::end_freq (out)]
/// @return error code
#define MCATCH_CMD_SYSTEM_FM			0x020
/// Shut down FM radio hardware.
/// @return error code
#define MCATCH_CMD_SYSTEM_FM_HW_STOP		0x021
/// Change FM frequency range.
/// @param[in] data 0 for 76-108 MHz, 1 for 88.5-108 MHz [mcatch_fm_t *, uses mcatch_fm_t::freq_range]
/// @return error code
#define MCATCH_CMD_SYSTEM_FM_CHG_BAND_LIMIT	0x022
/// Set FM search sensitivity.
/// @param[in] data Sensitivity (0-3) [mcatch_fm_t *, uses mcatch_fm_t::sensitivity]
/// @return error code
#define MCATCH_CMD_SYSTEM_FM_SEARCH_TOP_LVL	0x023

/// Flush NAND cache (guess).
/// @return error code
#define MCATCH_CMD_SYSTEM_NAND_CACHE_FLUSH	0x024

typedef struct {
    uint32_t __unknown_00;
    uint32_t __unknown_04;
    uint32_t __unknown_08;
    uint32_t __unknown_0c;
    uint32_t pwr_off_mode;	/**< [in] +10 */
    uint32_t pwr_off_time;	/**< [in] +14 */
} mcatch_pwr_off_time_t;

/// Set system power off mode and time (guess).
/// @param data [mcatch_pwr_off_time_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_PWR_OFF_TIME_SET	0x025
/// Set power off mode to 0.
/// @return error code
#define MCATCH_CMD_SYSTEM_SET_PWR_OFF_MODE_ZERO	0x026

/* #define MCATCH_CMD_SYSTEM_INVALID		0x027 */

typedef struct {
    uint32_t offset;	/**< [in] byte offset, will be rounded up to next 512 bytes; maximum 10240 */
    void *buf;		/**< [in,out] data buffer */
} mcatch_nand_t;

/// Read hidden NAND data (guess).
/// @param[in] data [mcatch_nand_t *, uses mcatch_nand_t::offset]
/// @param[out] data [mcatch_nand_t *, uses mcatch_nand_t::buf]
/// @return error code
#define MCATCH_CMD_SYSTEM_NAND_HIDDEN_READ	0x028
/// Write hidden NAND data (guess).
/// @param[in] data [mcatch_nand_t *, uses mcatch_nand_t::offset]
/// @param[in] data [mcatch_nand_t *, uses mcatch_nand_t::buf]
/// @return error code
#define MCATCH_CMD_SYSTEM_NAND_HIDDEN_WRITE	0x029

/* #define MCATCH_CMD_SYSTEM_NOP		0x02A */
/* #define MCATCH_CMD_SYSTEM_NOP_FAIL		0x02B */

/// Start MMI (guess).
#define MCATCH_CMD_SYSTEM_MMI_START		0x02C /* canonical */
/// Set "MMI message ready" flag (guess).
/// @return error code
#define MCATCH_CMD_SYSTEM_MMI_MSG_READY		0x02D /* canonical */

/// Set "long key" latency (guess).
/// @param[in] uint32_t latency
/// @return error code
#define MCATCH_CMD_SYSTEM_SET_LONGKEY_LATENCY	0x02E

/// Set ARM clock speed high.
/// On the SK8607, this sets the clock speed to 216 MHz.
#define MCATCH_CMD_SYSTEM_ARM_CLOCK_SPEED_UP	0x02F

/// Register MicroWindows window handle (guess).
/// @param data HWND handle
/// @return error code
#define MCATCH_CMD_SYSTEM_HWND_REG		0x030

typedef struct {
    uint32_t get_romfs;			/**< [in] set to 1 to receive romfs information */
    uint32_t romfs_serial_num;		/**< [out] +04; romfs serial number */
    uint32_t redboot_serial_num;	/**< [out] +08; RedBoot version number */
    uint32_t __unknown_0c;
    uint32_t __unknown_10;
    char romfs_serial_str[0x10];	/**< [out] +14; romfs serial number as text */
    char redboot_version_str[0x10];	/**< [out] +24; RedBoot version number as text */
    char __unknown_34[0x10];
    char source_version_str[0x10];	/**< [out] +44; source code version as text */
    char __unknown_54[0x200];
    char romfs_string[0x100];		/**< [out] +254; romfs information string (size is a guess) */
} mcatch_sys_version_t;

/// Get system version numbers.
/// @param[in,out] data [mcatch_sys_version_t *]
#define MCATCH_CMD_SYSTEM_SYS_VERSION_GET	0x031
/// Get reason for system wakeup (guess).
/// @param[out] data reason [uint32_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_GET_WAKEUP_REASON	0x032
/// Reboot system (guess).
/// @return error code
#define MCATCH_CMD_SYSTEM_SYS_REBOOT		0x033

/* #define MCATCH_CMD_SYSTEM_INVALID		0x034 */

/// Resume LCD (guess).
#define MCATCH_CMD_SYSTEM_RESUME_LCD		0x035
/// Suspend LCD (guess).
#define MCATCH_CMD_SYSTEM_SUSPEND_LCD		0x036

/// Set system frequency to 21.
#define MCATCH_CMD_SYSTEM_CHANGE_SYS_FREQ_21	0x037
/// Set system frequency to 10.
#define MCATCH_CMD_SYSTEM_CHANGE_SYS_FREQ_10	0x038
/// Lock system frequency (guess).
#define MCATCH_CMD_SYSTEM_LOCK_FREQ		0x039
/// Unlock system frequency (guess).
#define MCATCH_CMD_SYSTEM_UNLOCK_FREQ		0x03A

/* #define MCATCH_CMD_SYSTEM_INVALID		0x03B .. 0x03F */

typedef struct {
    int __unknown_00;	/**< [in] +00; 0-2 accepted */
    mcatch_rect_t rect;	/**< [in] +04 */
} mcatch_system_040_t;

/// Unknown function.
/// Defined in A1000; not defined in SK8607. Seems to be display-related.
/// @param data [mcatch_system_040_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_UNKNOWN_040		0x040
/// Unknown function.
/// Defined in A1000; not defined in SK8607. Seems to be display-related.
/// @return error code
#define MCATCH_CMD_SYSTEM_UNKNOWN_041		0x041
/// Unknown disable command.
/// Defined in A1000; not defined in SK8607. Seems to be display-related
/// and disable something.
/// @return error code
#define MCATCH_CMD_SYSTEM_UNKNOWN_042		0x042
/// Unknown enable command.
/// Defined in A1000; not defined in SK8607. Seems to be display-related
/// and enable something.
/// @return error code
#define MCATCH_CMD_SYSTEM_UNKNOWN_043		0x043

/* #define MCATCH_CMD_SYSTEM_INVALID            0x044 .. 0x04F */

/// Bogus audio enable command.
/// Defined in A1000; not defined in SK8607. Calls audio_EffectSel_Enable() with
/// an invalid argument.
/// @return error code (always 0)
#define MCATCH_CMD_SYSTEM_UNKNOWN_050		0x050
/// Bogus audio disable command.
/// Defined in A1000; not defined in SK8607. Calls audio_EffectSel_Disable() with
/// an invalid argument.
/// @return error code (always 0)
#define MCATCH_CMD_SYSTEM_UNKNOWN_051		0x051

/* #define MCATCH_CMD_SYSTEM_NOP		0x052 */

/* #define MCATCH_CMD_SYSTEM_INVALID            0x053 .. 0x05F */

/// Get bit 0 of IoTraps (guess).
/// @param[out] data bit [uint32_t *]
/// @return error code
#define MCATCH_CMD_SYSTEM_GET_IOTRAPS_BIT0	0x060

/// @}

/// @addtogroup dsc DSC commands
/// Commands that control the digital still camera.
/// The camera code appears to use the game memory area at 0xa00000, so it's
/// probably not usable unless you relocate your code to a safe place.
/// @ingroup spmp
/// @{

typedef struct {
    uint16_t __unknown_00;		/**< [in] +00; perhaps x size */
    uint16_t __unknown_02;		/**< [in] +02; perhaps y size */
    uint8_t input_module;	/**< [in] +04 */
    uint8_t dsc_timestamp;	/**< [in] +05 */
    uint8_t __unknown_06;
    uint8_t __unknown_07;
    uint8_t white_balance;	/**< [in] +08 */
    uint8_t night_mode;		/**< [in] +09 */
    uint8_t pwr_freq_mode;	/**< [in] +0a */
    uint8_t led_ctl_set;	/**< [in] +0b */
    uint8_t __unknown_0c;		/**< [in] +0c; zoom-related */
    uint8_t sensor_num;		/**< [in] +0d */
    uint8_t tv_input_mode;	/**< [in] +0e */
} mcatch_dsc_t;

typedef struct {
    uint8_t __unknown_00[0x21c];
} mcatch_dsc_cap_attr_t;

/// Start camera hardware (guess).
/// @param data [mcatch_dsc_t *]
/// @return error code
#define MCATCH_CMD_DSC_START			0x101
/// Stop camera hardware (guess).
/// @return error code
#define MCATCH_CMD_DSC_STOP			0x102
/// Set camera zoom (guess).
/// @param data zoom [uint8_t *]
#define MCATCH_CMD_DSC_ZOOM			0x103
/// Set preview brightness (guess).
/// @param data brightness [uint8_t *]
#define MCATCH_CMD_DSC_PREVIEW_BRIGHTNESS_SET	0x104
/// Set camera options (guess).
/// @param data [mcatch_dsc_t *]
#define MCATCH_CMD_DSC_OPTION_SET		0x105
/// Capture photo/video (guess).
/// @param data [mcatch_dsc_cap_attr_t *]
/// @return error code
#define MCATCH_CMD_DSC_CAPTURE			0x106

/// @}

/// @addtogroup image Image/playback commands
/// Image file handling.
/// @ingroup spmp
/// @{

/// Get image information (guess).
/// @param data unknown [void *]
/// @return error code
#define MCATCH_CMD_IMAGE_INFO_GET		0x301 /* probably canonical */
/// Start playback (guess).
/// In the SK8607 firmware, it looks as if this is an incorrectly implemented
/// NOP; it seems to inadvertently fall through to the next command.
#define MCATCH_CMD_PLAYBACK_START		0x302 /* canonical */

/* #define MCATCH_CMD_IMAGE_INVALID		0x303 */

typedef struct {
    uint32_t src_width;		/**< +00 */
    uint32_t src_height;	/**< +04 */
    uint32_t src_top;		/**< +08 */
    uint32_t src_left;		/**< +0c */
    uint32_t des_width;		/**< +10 */
    uint32_t des_height;	/**< +14 */
    uint32_t des_top;		/**< +18 */
    uint32_t des_left;		/**< +1c */
    char filename[];		/**< +20 */
} mcatch_zoom_pan_t;

/// Unknown function.
/// @param data [mcatch_zoom_pan_t *]
/// @return error code
#define MCATCH_CMD_PLAYBACK_ZOOM_PAN_SET	0x304 /* canonical */
/// Unknown function.
/// @param data [mcatch_zoom_pan_t *]
/// @return error code
#define MCATCH_CMD_IMAGE_PLAY_ZOOM_PAN		0x305

/* #define MCATCH_CMD_INVALID			0x306 */

/// Free image (guess).
/// In the A1000 and SK8607 firmwares, this command is identical to
/// MCATCH_CMD_IMAGE_FREE_IMAGE2.
#define MCATCH_CMD_IMAGE_FREE_IMAGE		0x307
/// Free image (guess).
/// In the A1000 and SK8607 firmwares, this command is identical to
/// MCATCH_CMD_IMAGE_FREE_IMAGE. 
#define MCATCH_CMD_IMAGE_FREE_IMAGE2		0x308

/// Load JPEG decode codec (guess).
/// Note that this command appends "/image" to the codec name, so make sure
/// you allocate a large enough buffer.
/// @param data codec name (guess) [char *]
/// @return error code
#define MCATCH_CMD_IMAGE_JPEG_DECODE_CODEC_LOAD	0x309
/// Unload JPEG decode codec (guess).
#define MCATCH_CMD_IMAGE_JPEG_DECODE_CODEC_RELEASE	0x30A
/// Flip display buffers.
#define MCATCH_CMD_IMAGE_DISPLAY_FLIP		0x30B

/// @}

/// @addtogroup media Media commands
/// Media playback and recording.
/// @ingroup spmp
/// @{
typedef struct {
    uint64_t __unused_00;
    uint16_t __unknown_08;		/**< +08; video-related */
    uint16_t __unknown_0a;		/**< +0a; video-related */
    uint8_t audio_video;	/**< +0c; 0 for audio, 1 for video recording */
    uint8_t __unused_0d;
    uint16_t __unused_0e;
    uint64_t __unused_10;
    uint32_t __unknown_18;		/**< +18; audio-related */
    uint32_t __unknown_1c;		/**< +1c; video-related */
    uint32_t __unknown_20;		/**< +20; video-related */
    char filename[0x200];	/**< [in] +0x24 */
    /* total size 0x224 bytes */
} mcatch_media_t;

/// Start media recording (guess).
/// @param data [mcatch_media_t *]
/// @return error code
#define MCATCH_CMD_MEDIA_RECORD			0x401
/// Stop media recording (guess).
/// @return error code
#define MCATCH_CMD_MEDIA_RECORD_STOP		0x402

/// This data "structure" is flat-out insane.
typedef struct {
    uint32_t __unknown_00;
    uint32_t __unknown_04;
    uint16_t __unknown_08;
    uint8_t __unknown_0a;
    uint8_t __unknown_0b;
    uint32_t __unknown_0c;
    uint8_t __unknown_10[0x380];
    uint8_t __unknown_390[0x80];
    uint32_t __unused_410;
    uint32_t __unknown_414;
    uint32_t __unused_418;
    uint16_t __unknown_41c;
    uint16_t __unknown_41e;
    uint8_t __unknown_420;
    uint8_t __unknown_421;
    uint16_t __unused_422;
    uint8_t __unused_424[0x304];
    uint8_t __unknown_728;
    uint8_t __unused_729;
    uint8_t __unused_72a;
    char __unknown_72b[0x201];
    uint32_t media_start_sec;	/**< [in] +92c */
    uint32_t __unused_930;
    uint8_t __unknown_934;
    uint8_t __unused_935;
    uint16_t __unused_936;
    uint8_t __unused_938[0x51c];
    uint8_t __unused_e54;
    uint8_t __unknown_e55;
    /* total size min. 0xe56 (3670) bytes... */
} mcatch_media_info_t;

/// Get media info (guess).
/// @param data [mcatch_media_info_t *]
/// @return error code
#define MCATCH_CMD_MEDIA_INFO_GET		0x403
/// Start media playback (guess).
/// @param data [mcatch_media_info_t *]
/// @return error code
#define MCATCH_CMD_MEDIA_START			0x404
/// Stop media playback (guess).
/// @param[out] data bookmark position (NULL to ignore) [uint32_t *]
/// @return error code
#define MCATCH_CMD_MEDIA_STOP			0x405
/// Pause media playback (guess).
/// @return error code
#define MCATCH_CMD_MEDIA_PAUSE			0x406
/// Resume media playback (guess).
/// @return error code
#define MCATCH_CMD_MEDIA_RESUME			0x407
/// Set media playback position (guess).
/// @param data position (ms) [int] (NB: passed by value, not a pointer!)
/// @return error code
#define MCATCH_CMD_MEDIA_SEEK			0x408
/// Enable background audio playback (guess).
#define MCATCH_CMD_MEDIA_AUDIO_BG_START		0x409 /* canonical */
/// Disable background audio playback (guess).
#define MCATCH_CMD_MEDIA_AUDIO_BG_STOP		0x40A /* canonical */
/// Unknown function.
/// A1000 and SK8607 firmwares always returns 0xa.
/// @param data [void *]
/// @return error code
#define MCATCH_CMD_MEDIA_GET_FRAME		0x840B

/// @}

/// @addtogroup fs File system commands
/// File system access.
/// @ingroup spmp
/// @{

typedef struct {
    char *dev_name;
    char *dir_name;
    char *fs_name;
} mcatch_fs_mount_t;

/// Mount device.
/// @param data [mcatch_fs_mount_t *]
/// @return error code
#define MCATCH_CMD_FS_DEV_MOUNT			0x601 /* probably canonical */
/// Unmount device.
/// @param data [mcatch_fs_mount_t *, uses mcatch_fs_mount_t::dir_name]
/// @return error code
#define MCATCH_CMD_FS_DEV_UNMOUNT		0x602 /* canonical */

/// Format device with a FAT32 file system (guess).
/// @param data device name (guess) [char *]
/// @return error code
#define MCATCH_CMD_FS_DEV_FORMAT		0x603

typedef struct {
    uint32_t capacity;		/**< [out] +00 */
    uint32_t free_bytes;	/**< [out] +04 */
    uint32_t __unknown_08;		/**< [out] +08 */
    uint32_t __unused_0c;
    uint32_t dev_num;	/**< [in] +10 */
    char mountpoint[];	/**< [in] +14 */
} mcatch_device_info_t;

/// Get device information.
/// @param data [mcatch_device_info_t *]
/// @return error code
#define MCATCH_CMD_FS_DEV_INFO_GET		0x604

typedef struct {
    uint32_t request;	/**< [in] +00 */
    uint32_t __unused_04;
    uint32_t __unknown_08;
    uint32_t __unknown_0c;
    char dir[0x200];	/**< [in] +10 */
    char filter[0x200];	/**< [in] +210 */
    uint8_t __unknown_410;	/**< [in] +410 */
    uint8_t __unknown_411;	/**< [in] +411 */
    void *unknown_414;	/**< [out] +414; must be at least 0x60 bytes */
} mcatch_multi_file_t;

/// Get number of files to be found (guess).
/// @param data [mcatch_multi_file_t *]
/// @return error code
#define MCATCH_CMD_FS_MULTI_FILE_GET_NUM	0x8605
/// Find multiple files (guess).
/// @param data [mcatch_multi_file_t *]
/// @return error code
#define MCATCH_CMD_FS_MULTI_FILE_FIND		0x606

/// Set current directory.
/// @param[in] data directory name [char *]
/// @return error code
#define MCATCH_CMD_FS_SET_DIR			0x607
/// Get current directory.
/// @param[out] data directory name [char *]
/// @return error code
#define MCATCH_CMD_FS_GET_DIR			0x8608
/// Create directory.
/// @param[in] data directory name [char *]
/// @return error code
#define MCATCH_CMD_FS_MKDIR			0x609
/// Remove directory.
/// @param[in] data directory name [char *]
/// @return error code
#define MCATCH_CMD_FS_RMDIR			0x60A

typedef struct {
    uint32_t ctime_hour;	/**< [out] +00 */
    uint32_t ctime_min;		/**< [out] +04 */
    uint32_t ctime_sec;		/**< [out] +08 */
    uint32_t ctime_day;		/**< [out] +0c */
    uint32_t ctime_month;	/**< [out] +10 */
    uint32_t ctime_year;	/**< [out] +14 */
    uint32_t mtime_hour;	/**< [out] +18 */
    uint32_t mtime_min;		/**< [out] +1c */
    uint32_t mtime_sec;		/**< [out] +20 */
    uint32_t mtime_day;		/**< [out] +24 */
    uint32_t mtime_month;	/**< [out] +28 */
    uint32_t mtime_year;	/**< [out] +2c */
    uint32_t atime_hour;	/**< [out] +30 */
    uint32_t atime_min;		/**< [out] +34 */
    uint32_t atime_sec;		/**< [out] +38 */
    uint32_t atime_day;		/**< [out] +3c */
    uint32_t atime_month;	/**< [out] +40 */
    uint32_t atime_year;	/**< [out] +44 */
    uint32_t type;	/**< [out] +48; 1 is file, 2 is directory */
    uint32_t size;	/**< [out] +4c */
    uint32_t __unused_50[3];
    uint32_t __unknown_5c;	/**< [out] +5c */
    char filename[];	/**< [in] +60 */
} mcatch_file_info_t;

/// Get file information.
/// @param data [mcatch_file_info_t *]
/// @return error code
#define MCATCH_CMD_FS_FILE_INFO_GET		0x60B

/* #define MCATCH_CMD_FS_INVALID		0x60C */

/// Remove file or directory.
/// @param data file/directory name [char *]
/// @return error code
#define MCATCH_CMD_FS_DELETE			0x60D

/* #define MCATCH_CMD_FS_INVALID		0x60E .. 0x610 */

/// Open file read-only.
/// @param data file name [char *]
/// @return file handle, or error code 0x70000002
#define MCATCH_CMD_FS_OPEN			0x8611
/// Close file.
/// @param[in] data file handle [int *]
/// @return error code
#define MCATCH_CMD_FS_CLOSE			0x612

typedef struct {
    uint32_t count;		/**< [in] +00 */
    int seek_start;		/**< [in] +04; set to not zero to seek to start of file first */
    uint32_t __unknown_08;	/**< [in] +08 */
    int fd;			/**< [in] +0c */
    char filename[0x200];	/**< [in] +10 */
    uint8_t mode;		/**< [in] +210 */
    void *buf;			/**< [in,out] +214 */
} mcatch_fs_file_t;

/// Open file in modes other than read-only.
/// @param data [mcatch_fs_file_t *]
/// @return file handle, or error code 0x70000002
#define MCATCH_CMD_FS_OPEN_EXT			0x8612
/// Read data from file.
/// @param data [mcatch_fs_file_t *]
/// @return error code
#define MCATCH_CMD_FS_READ			0x613

/* #define MCATCH_CMD_FS_INVALID		0x614 */

/// Write data to file.
/// @param data [mcatch_fs_file_t *]
/// @return error code
#define MCATCH_CMD_FS_WRITE			0x615

/* #define MCATCH_CMD_FS_INVALID		0x616 */

typedef struct {
    uint32_t offset;	/**< [in] +00 */
    int fd;		/**< [in] +04 */
    uint8_t whence;		/**< [in] +08 */
    uint8_t src_file[0x200];	/**< [in] +09 */
    uint8_t dst_file[0x200];	/**< [in] +209 */
} mcatch_fs_file_cmd_t;

/// Seek in file.
/// @param data [mcatch_fs_file_cmd_t *]
/// @return eCos(!) error code
#define MCATCH_CMD_FS_SEEK			0x617
/// Copy file.
/// @param data [mcatch_fs_file_cmd_t *]
/// @return error code
#define MCATCH_CMD_FS_COPY_FILE			0x618
/// Get current position in file (guess).
/// In the A1000 and SK8607 firmwares this function is unimplemented.
/// @param data [mcatch_fs_file_cmd_t *]
/// @return unknown
#define MCATCH_CMD_FS_TELL			0x619

/// Unknown function.
#define MCATCH_CMD_FS_FORCE_SET_REFRESH		0x61A

/// Find file in directory by number (guess).
/// @param data [mcatch_fs_file_t *]
/// @return (non-standard) error code
#define MCATCH_CMD_FS_FIND_FILE_NO_IN_DIR	0x61B

/// @}

/// @addtogroup usb USB commands
/// USB subsystem control.
/// @ingroup spmp
/// @{

/// Set USB mode (guess).
/// @param data not used
/// @param size must not be 1
/// @return error code
#define MCATCH_CMD_USB_MODE_SET			0x901 /* canonical */

typedef struct {
    uint8_t __unused_00[6];
    char name[];		/**< [in] +06 */
} mcatch_usb_class_t;

/// Set USB class name (guess).
/// The name is saved, but does not seem to be used anywhere.
/// @param data [mcatch_usb_class_t *]
#define MCATCH_CMD_USB_SET_CLASS_NAME		0x902

typedef struct {
    uint8_t __unused_00[0x1c];
    uint32_t suspend_time;	/**< [in] +1c */
} mcatch_usb_power_t;

/// Enable USB power saving (guess).
/// @param [mcatch_usb_power_t *]
/// @return error code
#define MCATCH_CMD_USB_SAVE_POWER_ON		0x903 /* canonical */
/// Disable USB power saving (guess).
/// @return error code
#define MCATCH_CMD_USB_SAVE_POWER_OFF		0x904 /* probably canonical */

/// Unknown command.
/// Defined on A1000; not defined on SK8607. Calls stopAllStorageTransfer_udc().
/// @return error code (always 0)
#define MCATCH_CMD_USB_STOP_STORAGE_TRANSFER	0x905
/// Unknown command.
/// Defined on A1000; not defined on SK8607. Calls startAllStorageTransfer_udc().
/// @return error code (always 0)
#define MCATCH_CMD_USB_START_STORAGE_TRANSFER	0x906

/// @}

/// @addtogroup gfx Graphics commands
/// Display subsystem controls.
/// @ingroup spmp
/// @{

typedef struct {
    uint32_t fb_format;		/**< +00 */
    uint32_t __unknown_04;
    uint32_t enable_lcd;	/**< +08; ignored if TV enabled */
    uint32_t enable_osd;	/**< +0c */
    uint32_t __unknown_10;	/**< [out] +10 */
} mcatch_gfx_mode_t;

/// Set graphics mode (guess).
/// @param data [mcatch_gfx_mode_t *]
/// @return error code
#define MCATCH_CMD_GFX_MODE_SET			0xB01 /* canonical */
/// Get graphics mode (guess).
/// @param data [mcatch_gfx_mode_t *]
/// @return error code
#define MCATCH_CMD_GFX_MODE_GET			0xB02 /* probably canonical */

/* #define MCATCH_CMD_GFX_INVALID		0xB03 */

/// Clear both display device buffers if TV is not enabled.
#define MCATCH_CMD_GFX_CLEAR_FLIP_IF_TV_OFF	0xB04
/// Clear display frame buffer if TV is not enabled.
#define MCATCH_CMD_GFX_CLEAR_IF_TV_OFF		0xB05
/// Set frame buffer format.
/// @param data frame buffer format [uint32_t *]
/// @return error code
#define MCATCH_CMD_GFX_SET_FB_FORMAT		0xB06

/// Unknown function.
/// Seems to be power management-related.
#define MCATCH_CMD_GFX_SET_POWER_MNT		0xB07
/// Unknown function.
/// Reverts actions of MCATCH_CMD_GFX_SET_POWER_MNT.
#define MCATCH_CMD_GFX_RESET_POWER_MNT		0xB08
/// Unknown function.
/// In the A1000 and SK8607 firmware, this points to the same code as
/// MCATCH_CMD_GFX_SET_POWER_MNT.
#define MCATCH_CMD_GFX_SET_POWER_MNT_2		0xB09
/// Unknown function.
/// In the A1000 and SK8607 firmware, this points to the same code as
/// MCATCH_CMD_GFX_RESET_POWER_MNT.
#define MCATCH_CMD_GFX_RESET_POWER_MNT_2	0xB0A

/// Enable/disable vertical TV synchronization (guess).
/// In the SK8607 firmware, this sets the global variable
/// g_tv_wait_frame_end, which is not read anywhere.  On the A1000, it is
/// entirely unimplemented.
/// @param data enable TV v-sync [int]
/// @return error code
#define MCATCH_CMD_GFX_SET_TV_WAIT_FRAME_END	0xB0B

/// Set alpha channel for on-screen display (guess).
/// @param data alpha channel value (0-100) [int]
/// @return error code
#define MCATCH_CMD_GFX_SET_OSD_BUFFER_MIX_ALPHA	0xB0C

/* #define MCATCH_CMD_GFX_NOP			0xB0D */

/// Enable/disable double buffer copying (guess).
/// This only affects the Microwindows UI, not the emuIf or NativeGE interfaces.
/// @param data 0 to disable, 1 to enable [int]
#define MCATCH_CMD_GFX_SET_DISABLE_DOUBLE_BUFFER_COPY	0xB0E

/// Enable/disable the display backlight.
/// @param data 0 to disable, 1 to enable [int]
#define MCATCH_CMD_GFX_SET_BACKLIGHT_ENABLED	0xB0F

/// Set on-screen display clipping area (guess).
/// @param data [mcatch_rect_t *]
#define MCATCH_CMD_GFX_LCD_SET_OSD_CLIP_AREA	0xB10

/// @}

/// @addtogroup gm Game manager commands
/// Game thread management.
/// @ingroup spmp
/// @{

/// Create and start the game thread (guess).
/// Initializes the game system, calls the emulator startup function and
/// creates, then runs the game thread.
#define MCATCH_CMD_GM_START_GAME_THREAD		0xD01

/* #define MCATCH_CMD_GM_NOP			0xD02 */

typedef struct {
    uint32_t resume_param;	/**< [in] +00; passed to emu_apis_t::resume */
    uint32_t ctrl_key;		/**< [in] +04 */
} mcatch_gm_t;

/// Set control key value, then resume game thread.
/// @param data [mcatch_gm_t *]
/// @return (non-standard) error code
#define MCATCH_CMD_GM_SET_CRTL_KEY_VALUE_AND_RESUME	0xD03
/// Stop game.
/// @param data emu_apis_t::exit parameter [uint32_t *]
/// @return (non-standard) error code
#define MCATCH_CMD_GM_EXIT_GAME			0xD04
/// Save game state.
/// @return (non-standard) error code
#define MCATCH_CMD_GM_SAVE_STATE		0xD05

/// Set keyboard to game mode (guess).
/// @return error code
#define MCATCH_CMD_GM_ENABLE_GAME_KEYBOARD	0xD06
/// Set keyboard to Microwindows mode (guess).
/// @return error code
#define MCATCH_CMD_GM_ENABLE_MW_KEYBOARD	0xD07
/// Set "longkey latency".
/// @param data latency [uint32_t *]
/// @return error code
#define MCATCH_CMD_GM_SET_LONGKEY_LATENCY	0xD08

/// Load game state.
/// @return (non-standard) error code
#define MCATCH_CMD_GM_LOAD_STATE		0xD09

/// @}

#endif

