#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/// @addtogroup adbg Debug output via audio connection
/// @{
int adbg_init(void);
void adbg_fini(void);
void adbg_puts(const char *);
int adbg_printf(const char *fmt, ...);
/// @}

#ifdef __cplusplus
}
#endif /* __cplusplus */
