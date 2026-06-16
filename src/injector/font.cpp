#include "renderer.h"

static std::vector<font_t> g_font_list{};

void c_fonts::init(IDirect3DDevice9* device, const std::vector<font_t> list)
{
	if (g_font_list.empty())
		g_font_list = list;

	for (const auto& font : g_font_list)
	{
		D3DXCreateFontA(
			device,
			font.m_px, 0,
			font.m_weight, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			font.m_quality,
			FF_DONTCARE,
			font.m_name.c_str(),
			&m_fonts[font.m_index]);
	}
}

void c_fonts::draw_string(const std::string& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color)
{
	RECT r{ x, y, x, y };

	if (flags & TEXT_OUTLINE) {
		RECT o_r{ x + 1, y + 1, x + 1, y + 1 };
		font->DrawTextA(NULL, string.c_str(), -1, &o_r, DT_NOCLIP, c_color(0, 0, 0, color.get_arr()[3]).get());
	}

	font->DrawTextA(NULL, string.c_str(), -1, &r, DT_NOCLIP, color.get());
}

void c_fonts::draw_stringW(const std::wstring& string, float x, float y, ID3DXFont* font, uint8_t flags, c_color color)
{
	RECT r{ x, y, x, y };

	if (flags & TEXT_OUTLINE) {
		RECT o_r{ x + 1, y + 1, x + 1, y + 1 };
		font->DrawTextW(NULL, string.c_str(), -1, &o_r, DT_NOCLIP, c_color(0, 0, 0, color.get_arr()[3]).get());
	}

	font->DrawTextW(NULL, string.c_str(), -1, &r, DT_NOCLIP, color.get());
}

float c_fonts::get_text_width(const std::string& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

float c_fonts::get_text_widthW(const std::wstring& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextW(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

float c_fonts::get_text_height(const std::string& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.bottom - r.top);
}

float c_fonts::get_text_heightW(const std::wstring& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextW(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.bottom - r.top);
}

void c_fonts::undo()
{
	for (const auto& font : g_font_list)
	{
		if (m_fonts[font.m_index]) {
			m_fonts[font.m_index]->Release();
			m_fonts[font.m_index] = nullptr;
		}
	}
}