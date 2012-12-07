/// @addtogroup dmsg Graphical Debug Console
/// On-screen debug output console.
/// @{

/// Initialize debug console.
/// @note dmsg_init() uses the [NativeGE graphics API](@ref ge_gfx) and
/// assumes that you have already initialized it.
int dmsg_init(int x, int y, int width, int height);
/// Finalize debug console.
void dmsg_shutdown(void);
/// Enable/disable waiting for key press when the screen is full.
void dmsg_wait(int enable);
/// Clear the screen.
void dmsg_clear(void);
/// Write a string to the console.
int dmsg_puts(char *__s);
/// Write a formatted string to the console.
int dmsg_printf(char *format, ...);
/// @}
