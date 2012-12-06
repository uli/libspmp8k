#include <stdint.h>

/// @addtogroup text Text rendering routines
/// @{
#define FONT_SIZE_12 12
#define FONT_SIZE_16 16

#define FONT_FACE_HZX 0
#define FONT_FACE_SONGTI 1
#define FONT_FACE_SONGTI_BOLD 2
#define FONT_FACE_XINSONG 3
#define FONT_FACE_MAX 4

int text_init(void);
void text_free(void);
int text_draw_character(uint32_t codepoint, int x, int y);
int text_draw_character_ex(uint16_t *buf, int width, uint32_t codepoint, int x, int y);
int text_render(const char *t, int x, int y);
int text_render_centered(const char *t, int y);
int text_render_ex(uint16_t *buf, int width, const char *t, int x, int y);
void text_set_font_size(int fs);
int text_get_font_size(void);
void text_set_font_face(int face);
void text_set_fg_color(uint16_t color);
void text_set_bg_color(uint16_t color);
/// @}
