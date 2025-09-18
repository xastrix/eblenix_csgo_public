#pragma once

#include "ui_input.h"
#include "ui_sprites.h"

#include "../render_manager/render_manager.h"

enum _ui_colors {
	UI_MAIN_COL,
	UI_PRIMARY_COL,
	UI_SHADOW_COL,
	UI_TEXT_COL,
	maxUIColors,
};

struct ui_render : public ui_input, protected ui_sprites {
	void draw(int x, int y);
	color_t& get_color(const _ui_colors index);
private:
	void draw_float(float value, int x, int y, color_t color);
	void draw_bool(bool value, int x, int y, color_t color);
	void draw_int(int value, int x, int y, color_t color);
	void draw_hotkey(int k, int x, int x_text, int y, int y_text, int h, bool hold, color_t color);
	void draw_item(std::wstring item, int x, int y, color_t color);
private:
	color_t m_colors[maxUIColors]{};
};