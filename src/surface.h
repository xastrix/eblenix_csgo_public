#pragma once

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface031"

class c_surface_draw_manager {
public:
	void set_draw_color(int r, int g, int b, int a = 255);
	void draw_filled_rect(int x, int y, int w, int h);
	void draw_outlined_rect(int x, int y, int w, int h);
	void draw_line(int x1, int y1, int x2, int y2);
	void get_screen_size(int& width, int& height);
	void unlock_cursor();
	void lock_cursor();
	void play_sound(const char* path);
	void get_cursor_pos(int& x, int& y);
};