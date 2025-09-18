#pragma once

#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <d3dx9.h>

#include "../var_manager/var_manager.h"

enum _fonts {
	Tahoma12px,
	Verdana12px,
	Astriumwep16px,
	Astriumwep25px,
	maxFonts,
};

enum dstring_flags : uint8_t {
	TEXT_NONE,
	TEXT_OUTLINE,
	TEXT_CENTER_X,
};

struct vertex_t {
	float    m_x{},
		     m_y{},
		     m_z{};
	float    m_rhw{};
	D3DCOLOR m_color{};
	float    m_tu{},
		     m_tv{};
};

class color_t {
public:
	color_t(int r = 255, int g = 255, int b = 255, int a = 255)
		: m_color{ r, g, b, a } {}

	explicit color_t(const std::string& var_name, int alpha = 255)
		: m_color{
			get_var_value(var_name + "->r"),
			get_var_value(var_name + "->g"),
			get_var_value(var_name + "->b"),
			alpha,
	} {}

	color_t(int alpha, const std::string& var_name)
		: m_color{
			alpha,
			get_var_value(var_name + "->r"),
			get_var_value(var_name + "->g"),
			get_var_value(var_name + "->b"),
	} {};

	D3DCOLOR get() const {
		return D3DCOLOR_RGBA(m_color[0], m_color[1], m_color[2], m_color[3]);
	}

	D3DCOLOR get_revert() const {
		return D3DCOLOR_ARGB(m_color[0], m_color[3], m_color[2], m_color[1]);
	}

	const std::array<int, 4>& get_arr() const { return m_color; }

private:
	std::array<int, 4> m_color{};

	int get_var_value(const std::string& key) const {
		auto opt = g_vars.get_as<int>(key);

		if (!opt.has_value())
			return 0;

		return opt.value();
	}
};

struct font_t {
	_fonts      index{};
	int         px{};
	std::string name{};
	uint32_t    weight{};
};

struct render_manager {
	bool init(IDirect3DDevice9* device);
	bool create_objects();

	void begin_render_states();
	void end_render_states();

	void draw_line(int x0, int y0, int x1, int y1, float thickness, color_t color);
	void draw_filled_rect(int x, int y, int w, int h, color_t color);
	void draw_corner_box(int x, int y, int w, int h, int cx, int cy, color_t color);
	void draw_string(const std::string& string, int x, int y, ID3DXFont* font, uint8_t flags, color_t color);
	void draw_stringW(const std::wstring& string, int x, int y, ID3DXFont* font, uint8_t flags, color_t color);
	void draw_status(const std::string& string, color_t color);

	int get_text_width(const std::string& string, ID3DXFont* font);
	int get_text_widthW(const std::wstring& string, ID3DXFont* font);

	ID3DXFont* get_font(const _fonts index);

	void undo();
private:
	IDirect3DDevice9*     m_device{};
	ID3DXLine*            m_line{};
	IDirect3DStateBlock9* m_block{};
	ID3DXFont*            m_fonts[maxFonts]{};
};

extern render_manager g_render;