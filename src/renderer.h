#pragma once

#include "font.h"

enum gradient_flags {
	GRADIENT_HORIZONTAL,
	GRADIENT_VERTICAL,
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

struct sprite_manager {
	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height);

	void begin();
	void end();

	void on_reset();
	void on_reset_end();

	void draw(int x, int y, color_t color);

	int get_width();
	int get_height();
private:
	int                m_width{};
	int                m_height{};
	IDirect3DDevice9*  m_device{};
	ID3DXSprite*       m_sprite{};
	IDirect3DTexture9* m_texture{};
	const byte*        m_image{};
	size_t             m_image_size{};
};
using sprite_t = sprite_manager;

struct renderer {
	bool init(IDirect3DDevice9* device);
	bool create_objects(IDirect3DDevice9* device);

	void begin_render_states();
	void end_render_states();

	void draw_line(float x0, float y0, float x1, float y1, float thickness, color_t color);
	void draw_filled_rect(float x, float y, float w, float h, color_t color);
	void draw_filled_rect_fade(float x, float y, float w, float h, uint8_t flags, color_t first, color_t second);
	void draw_corner_box(float x, float y, float w, float h, float cx, float cy, color_t color);

	void undo();
private:
	IDirect3DDevice9*     m_device{};
	ID3DXLine*            m_line{};
	IDirect3DStateBlock9* m_block{};
};

inline renderer g_render;