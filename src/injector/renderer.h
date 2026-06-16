#pragma once

#include <array>
#include <vector>
#include <d3dx9.h>
#include <cmath>

enum _font_list {
	Tahoma12px,
	Verdana12px,
	Verdana13px,
	maxFonts,
};

enum _draw_string_flags {
	TEXT_NONE,
	TEXT_OUTLINE,
};

struct font_t {
	_font_list  m_index{};
	int         m_px{};
	std::string m_name{};
	uint32_t    m_weight{};
	DWORD       m_quality{};
};

class c_color {
public:
	c_color(int r = 255, int g = 255, int b = 255, int a = 255)
		: m_color{ r, g, b, a } {}

	D3DCOLOR get() const {
		return D3DCOLOR_RGBA(m_color[0], m_color[1], m_color[2], m_color[3]);
	}

	const std::array<int, 4>& get_arr() const {
		return m_color;
	}

private:
	std::array<int, 4> m_color;
};

class c_fonts {
public:
	void init(IDirect3DDevice9* device, const std::vector<font_t> list);

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

class c_vec2 {
public:
	float x;
	float y;

	c_vec2() : x(0.0f), y(0.0f) {}
	c_vec2(float x, float y) : x(x), y(y) {}

	c_vec2 operator+(const c_vec2& other) const {
		return c_vec2(x + other.x, y + other.y);
	}

	c_vec2 operator-(const c_vec2& other) const {
		return c_vec2(x - other.x, y - other.y);
	}

	c_vec2& operator+=(const c_vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	c_vec2& operator-=(const c_vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	c_vec2 operator*(float scalar) const {
		return c_vec2(x * scalar, y * scalar);
	}

	c_vec2 operator/(float scalar) const {
		return c_vec2(x / scalar, y / scalar);
	}

	float length() const {
		return std::sqrt(x * x + y * y);
	}

	c_vec2 normalize() const {
		float len = length();

		if (len > 0.0f)
			return c_vec2(x / len, y / len);

		return c_vec2(0.0f, 0.0f);
	}

	float dot(const c_vec2& other) const {
		return x * other.x + y * other.y;
	}
};

class c_renderer {
public:
	void init(IDirect3DDevice9* device);

	void rect(c_vec2 pos, c_vec2 size, c_color c);
	void rect(float x, float y, float w, float h, c_color c) {
		return rect({ x, y }, { w, h }, c);
	}

	void rect_fill(c_vec2 pos, c_vec2 size, c_color c);
	void rect_fill(float x, float y, float w, float h, c_color c) {
		return rect_fill({ x, y }, { w, h }, c);
	}

	void rect_cornered(c_vec2 pos, c_vec2 size, float radii, c_color c);
	void rect_cornered(float x, float y, float w, float h, float radii, c_color c) {
		return rect_cornered({ x, y }, { w, h }, radii, c);
	}

	void rect_fill_cornered(c_vec2 pos, c_vec2 size, float radii, c_color c);
	void rect_fill_cornered(float x, float y, float w, float h, float radii, c_color c) {
		return rect_fill_cornered({ x, y }, { w, h }, radii, c);
	}

	void line(c_vec2 a, c_vec2 b, c_color c);
	void line(float x, float y, float w, float h, c_color c) {
		return line({ x, y }, { w, h }, c);
	}

	IDirect3DDevice9* get_device() {
		return m_device;
	}

	void begin();
	void end();

	void undo();

private:
	IDirect3DDevice9*            m_device;
	IDirect3DStateBlock9*        m_state_block;
	IDirect3DVertexDeclaration9* m_vert_dec;
	IDirect3DVertexShader9*      m_vert_shader;
};

inline c_fonts g_font;
inline c_renderer g_renderer;