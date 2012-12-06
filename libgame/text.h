#include <stdint.h>

/// @addtogroup text Text rendering routines
/// Text rendering using the fonts from the ROM file system.
/// This interface provides you with an easy way to render text to the
/// frame buffer using the built-in fonts. It allows you to use one
/// of two sizes and four different typefaces and provides full
/// Unicode support.
/// @{
#define FONT_SIZE_12 12
#define FONT_SIZE_16 16

/// @name Typefaces
/// @{
#define FONT_FACE_HZX 0
#define FONT_FACE_SONGTI 1
#define FONT_FACE_SONGTI_BOLD 2
#define FONT_FACE_XINSONG 3
#define FONT_FACE_MAX 4
/// @}

/// Initialize text rendering subsystem.
int text_init(void);
/// Finalize text rendering subsystem.
void text_free(void);
/// Draw a character to the frame buffer.
/// @param codepoint Unicode codepoint
/// @param[in] x left of rendered text (pixels)
/// @param[in] y top of rendered text (pixels)
int text_draw_character(uint32_t codepoint, int x, int y);
/// Draw a character to an arbitrary buffer.
/// @param[out] buf output buffer
/// @param[in] width width of output buffer
/// @param[in] codepoint Unicode codepoint
/// @param[in] x left of rendered text (pixels)
/// @param[in] y top of rendered text (pixels)
int text_draw_character_ex(uint16_t *buf, int width, uint32_t codepoint, int x, int y);
/// Render a string to the frame buffer.
/// @param[in] t text string
/// @param[in] x left of rendered text (pixels)
/// @param[in] y top of rendered text (pixels)
int text_render(const char *t, int x, int y);
/// Render a string centered.
/// @param[in] t text string
/// @param[in] y top of rendered text (pixels)
int text_render_centered(const char *t, int y);
/// Render a string to an arbitrary buffer.
/// @param[out] buf output buffer
/// @param[in] width width of output buffer
/// @param[in] t text string
/// @param[in] x left of rendered text (pixels)
/// @param[in] y top of rendered text (pixels)
int text_render_ex(uint16_t *buf, int width, const char *t, int x, int y);
/// Set the font size.
/// @param fs FONT_SIZE_12 or FONT_SIZE_16
void text_set_font_size(int fs);
/// Get the font size.
/// @return FONT_SIZE_12 or FONT_SIZE_16
int text_get_font_size(void);
/// Set the typeface.
/// @param face One of the typefaces.
void text_set_font_face(int face);
/// Set the text foreground color.
/// @param color text color in RGB565 format
/// @sa MAKE_RGB565
void text_set_fg_color(uint16_t color);
/// Set the text background color.
/// @param color text color in RGB565 format
/// @sa MAKE_RGB565
void text_set_bg_color(uint16_t color);
/// @}
