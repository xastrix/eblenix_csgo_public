#pragma once

/*
**
** old render manager, used in hack for rendering esp, visuals, gui, and other things
**
*/

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

enum _fonts {
	Tahoma12px,
	VerdanaBold12px,
	maxFonts,
};

struct font_t {
	_fonts      m_index;
	int         m_px;
	std::string m_name;
	uint32_t    m_weight;
	DWORD       m_quality;
};

struct vertex_t {
	float       m_x,
				m_y,
				m_z;
	float       m_rhw;
	D3DCOLOR    m_color;
	float       m_tu, m_tv;
};

struct d3d {
	bool init(IDirect3DDevice9* device);
	bool create_objects(IDirect3DDevice9* device);

	bool begin_drawing();
	void end_drawing();

	void draw_line(int x0, int y0, int x1, int y1, D3DCOLOR color);
	void draw_rect(int x, int y, int w, int h, D3DCOLOR color);
	void draw_filled_rect(int x, int y, int w, int h, D3DCOLOR color);
	void draw_string(const std::string& string, int x, int y, ID3DXFont* font, D3DCOLOR color);

	int get_text_width(const std::string& string, ID3DXFont* font);
	int get_text_height(const std::string& string, ID3DXFont* font);

	ID3DXFont* get_font(const _fonts index);

	void undo();

private:
	IDirect3DDevice9*     m_device{};
	ID3DXLine*            m_line{};
	IDirect3DStateBlock9* m_block{};
	ID3DXFont*            m_fonts[maxFonts]{};
};

struct sprite_manager {
	~sprite_manager() {
		if (m_texture) {
			m_texture->Release();
			m_texture = nullptr;
		}

		if (m_sprite) {
			m_sprite->Release();
			m_sprite = nullptr;
		}
	}

	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height) {
		m_width = width;
		m_height = height;
		m_device = device;
		m_image = img;
		m_image_size = img_size;
	}

	void begin() {
		if (!m_device || !m_sprite)
			D3DXCreateSprite(m_device, &m_sprite);

		if (!m_texture)
			D3DXCreateTextureFromFileInMemoryEx(m_device, m_image, m_image_size, m_width, m_height,
				D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

		m_sprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE);
	}

	void end() {
		if (!m_device || !m_sprite)
			return;

		m_sprite->End();
	}

	void on_reset() {
		if (!m_device || !m_sprite || !m_texture)
			return;

		m_sprite->OnLostDevice();

		m_texture->Release();
		m_texture = nullptr;
	}

	void on_reset_end() {
		if (!m_device || !m_sprite)
			return;

		m_sprite->OnResetDevice();
	}

	void draw(int x, int y) {
		if (!m_device || !m_texture || !m_sprite)
			return;

		D3DXMATRIX matrix{};
		D3DXMatrixTranslation(&matrix, static_cast<float>(x), static_cast<float>(y), 0.0f);

		m_sprite->SetTransform(&matrix);
		m_sprite->Draw(m_texture, 0, 0, 0, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	int get_width() { return m_width; }
	int get_height() { return m_height; }

private:
	int                   m_width;
	int                   m_height;
	IDirect3DDevice9*     m_device{};
	ID3DXSprite*          m_sprite{};
	IDirect3DTexture9*    m_texture{};
	const byte*           m_image;
	size_t                m_image_size;
};

using sprite_t = sprite_manager;
inline d3d g_d3d;