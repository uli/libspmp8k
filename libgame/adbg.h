#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// @addtogroup adbg Audio Debugging
/// Debug output via audio connection.
/// These functions allow you to send debug output over the audio ports
/// of your device using a serial protocol. The sounds can be decoded
/// on your workstation using the \c adbg_console.py script. This is
/// supposed to save you the hassle of modifying your device to break
/// out the built-in serial port, which can be mechanically tricky.
///
/// Be sure to check the [example program](@ref adbg/game.c).
/// @note For obvious reasons, you have to disable any other use of
/// audio functionality in your application while using adbg.
/// @{

/// Initialize audio debug system.
/// This function must be run before any other \c adbg functions can
/// be used. (But not before libgame_init().)
int adbg_init(void);
/// Finalize audio debug system.
/// This function must be called when exiting your application.
void adbg_fini(void);
/// Send a string over the audio ports.
void adbg_puts(const char *);
/// Formatted printing to the audio ports.
int adbg_printf(const char *fmt, ...);

/// @example adbg/game.c

/// @}

#ifdef __cplusplus
}
#endif /* __cplusplus */
