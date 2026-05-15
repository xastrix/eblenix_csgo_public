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
	_font_list  m_index{};
	int         m_px{};
	std::string m_name{};
	uint32_t    m_weight{};
	DWORD       m_quality{};
};

class c_fonts {
public:
	void init(IDirect3DDevice9* device, const std::vector<font_t> list);
	void restore(IDirect3DDevice9* device);

	void draw_string(const std::string& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color);
	void draw_stringW(const std::wstring& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color);

	float get_text_width(const std::string& string, ID3DXFont* font);
	float get_text_widthW(const std::wstring& string, ID3DXFont* font);

	float get_text_height(const std::string& string, ID3DXFont* font);
	float get_text_heightW(const std::wstring& string, ID3DXFont* font);

	static std::shared_ptr<c_fonts> make_shared() {
		return std::shared_ptr<c_fonts>(new c_fonts());
	}

	ID3DXFont* operator[](_font_list index) const {
		return m_fonts[index];
	}

	void undo();

private:
	ID3DXFont* m_fonts[maxFonts]{};
};

inline std::shared_ptr<c_fonts> g_font = c_fonts::make_shared();

#define FONT(index) g_font->operator[](index)