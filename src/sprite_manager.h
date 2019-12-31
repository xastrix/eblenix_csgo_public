#pragma once

#include "render_manager.h"

struct sprite_manager {
	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height);

	void begin();
	void end();

	void on_reset();
	void on_reset_end();

	void draw(int x, int y);

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