#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// @addtogroup usbdbg USB Debugging
/// Debug output via USB connection.
/// These functions allow you to send debug output over the USB mass
/// storage connection. The data can be read on your workstation via
/// the SD card block device using the \c usb_console.c program.
/// Using the USB port is more reliable than [audio debugging](@ref adbg),
/// and much faster than either adbg or the serial port.
///
/// Be sure to check the [example program](@ref usb/usb.c).
/// @note USB debugging does not unmount the internal storage while the
/// device is connected to your workstation as mass storage. You have to
/// take care that your workstation OS does not automatically mount USB
/// mass storage devices, as that can lead to system crashes and data
/// corruption. It is advisable to reset the device after a debugging
/// session.
/// @{

/// Enable blocking I/O.
/// Setting this variable to a non-zero value causes calls to usbdbg_puts()
/// to block if the previous data has not been requested by the debug
/// console yet. You may have to enable this if you send large chunks of
/// debug output at a time.
extern int usbdbg_blocking;

/// Send a string to the USB debug console.
void usbdbg_puts(const char *);

void usbdbg_write(const char *, int);

/// Initialize USB debug system.
/// This function must be run before any other \c usbdbg functions
/// can be used.
int usbdbg_init(void);

/// Finalize USB debug system.
/// This function must be run before exiting your application. It returns
/// the system to the default USB plugging behavior.
/// @note Currently, this is experimental functionality. Side-effects of
/// the USB debugging hack may remain even after calling this function.
/// It is currently recommended to reset the device after each debugging
/// session.
void usbdbg_fini(void);

/// Wait until USB device has been plugged in.
void usbdbg_wait_for_plug(void);

void usbdbg_redirect_stdio(int);

/// @example usb/usb.c

/// @}

#ifdef __cplusplus
}
#endif /* __cplusplus */
