#include "surface.h"

void c_surface_draw_manager::set_draw_color(int r, int g, int b, int a)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int, int, int, int);
	return (*(original_fn**)this)[15](this, r, g, b, a);
}

void c_surface_draw_manager::draw_filled_rect(int x, int y, int w, int h)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int, int, int, int);
	return (*(original_fn**)this)[16](this, x, y, x + w, y + h);
}

void c_surface_draw_manager::draw_outlined_rect(int x, int y, int w, int h)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int, int, int, int);
	return (*(original_fn**)this)[18](this, x, y, x + w, y + h);
}

void c_surface_draw_manager::draw_line(int x1, int y1, int x2, int y2)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int, int, int, int);
	return (*(original_fn**)this)[19](this, x1, y1, x2, y2);
}

void c_surface_draw_manager::get_screen_size(int& width, int& height)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int&, int&);
	return (*(original_fn**)this)[44](this, width, height);
}

void c_surface_draw_manager::unlock_cursor()
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*);
	return (*(original_fn**)this)[66](this);
}

void c_surface_draw_manager::lock_cursor()
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*);
	return (*(original_fn**)this)[67](this);
}

void c_surface_draw_manager::play_sound(const char* path)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, const char*);
	return (*(original_fn**)this)[82](this, path);
}

void c_surface_draw_manager::get_cursor_pos(int& x, int& y)
{
	using original_fn = void(__thiscall*)(c_surface_draw_manager*, int&, int&);
	return (*(original_fn**)this)[100](this, x, y);
}