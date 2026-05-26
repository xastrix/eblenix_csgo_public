#pragma once

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface031"

using surface_font_t = unsigned long;

enum _surface_font_flags {
	sff_none,
	sff_italic = 0x001,
	ssf_underline = 0x002,
	sff_strikeout = 0x004,
	sff_symbol = 0x008,
	sff_antialias = 0x010,
	sff_gaussianblur = 0x020,
	sff_rotary = 0x040,
	sff_dropshadow = 0x080,
	sff_additive = 0x100,
	sff_outline = 0x200,
	sff_custom = 0x400,
	sff_bitmap = 0x800,
};

class c_surface_draw_manager {
public:
	void set_draw_color(int r, int g, int b, int a = 255);
	void set_text_color(int r, int g, int b, int a = 255);
	void draw_filled_rect(int x, int y, int w, int h);
	void draw_outlined_rect(int x, int y, int w, int h);
	void draw_line(int x1, int y1, int x2, int y2);
	void draw_text_font(surface_font_t font);
	void draw_text_pos(int x, int y);
	void draw_render_text(const wchar_t* text, int len);
	void get_screen_size(int& width, int& height);
	void unlock_cursor();
	void lock_cursor();
	surface_font_t font_create();
	void set_font_glyph(surface_font_t font, const char* name, int tall, int weight, int blur, int scanlines, int flags);
	void play_sound(const char* path);
	void get_cursor_pos(int& x, int& y);
};