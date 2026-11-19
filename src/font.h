#pragma once

#include "color.h"

enum _font_list {
	Tahoma12px,
	Verdana12px,
	SmallFonts10px,
	Astriumwep12px,
	Astriumwep16px,
	Astriumwep25px,
	maxFonts,
};

enum _draw_string_flags {
	TEXT_NONE,
	TEXT_OUTLINE,
	TEXT_CENTER_X,
};

struct font_t {
	_font_list  index;
	int         px;
	std::string name;
	uint32_t    weight;
	DWORD       quality;
};

class c_fonts {
public:
	void init(IDirect3DDevice9* device);
	void restore(IDirect3DDevice9* device);

	void draw_string(const std::string& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color);
	void draw_stringW(const std::wstring& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color);

	float get_text_width(const std::string& string, ID3DXFont* font);
	float get_text_widthW(const std::wstring& string, ID3DXFont* font);

	float get_text_height(const std::string& string, ID3DXFont* font);
	float get_text_heightW(const std::wstring& string, ID3DXFont* font);

	ID3DXFont* operator[](_font_list index) const {
		return m_fonts[index];
	}

	void undo();

private:
	ID3DXFont* m_fonts[maxFonts]{};
};

inline c_fonts g_font;

#define FONT(index) g_font[index]