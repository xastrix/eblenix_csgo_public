#pragma once

#include "font.h"

#include "vec2.h"
#include "vec3.h"

#define CIRCLE_POINTS           64
#define POINTS_SPHERE_LATITUDE  16
#define POINTS_SPHERE_LONGITUDE 24

class c_sprite_mgr {
public:
	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height) {
		HRESULT hr = E_FAIL;

		m_width = width;
		m_height = height;
		m_device = device;
		m_image = img;
		m_image_size = img_size;

		D3DXCreateSprite(m_device, &m_sprite);
		hr = D3DXCreateTextureFromFileInMemoryEx(m_device, m_image, m_image_size, m_width, m_height,
			D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

		if (FAILED(hr)) {
			if (m_texture) {
				m_texture->Release();
				m_texture = nullptr;
			}
		}

		m_hresult = hr;
	}

	void init(IDirect3DDevice9* device, const std::string& path, int width, int height) {
		HRESULT hr = E_FAIL;

		m_width = width;
		m_height = height;
		m_device = device;
		m_path = path;

		D3DXCreateSprite(m_device, &m_sprite);
		hr = D3DXCreateTextureFromFileExA(m_device, m_path.c_str(), m_width, m_height,
			D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

		if (FAILED(hr)) {
			if (m_texture) {
				m_texture->Release();
				m_texture = nullptr;
			}
		}

		m_hresult = hr;
	}

	void begin(DWORD flags) {
		if (!m_device || !m_sprite)
			return;

		m_sprite->Begin(flags);
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

		if (m_texture) {
			m_texture->Release();
			m_texture = nullptr;
		}
	}

	void on_reset_end() {
		HRESULT hr = E_FAIL;

		if (!m_device || !m_sprite)
			return;

		m_sprite->OnResetDevice();
		
		D3DXCreateSprite(m_device, &m_sprite);

		if (m_path.empty() && (m_image && m_image_size > 0))
			hr = D3DXCreateTextureFromFileInMemoryEx(m_device, m_image, m_image_size, m_width, m_height,
				D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);
		else
			hr = D3DXCreateTextureFromFileExA(m_device, m_path.c_str(), m_width, m_height,
				D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

		if (FAILED(hr)) {
			if (m_texture) {
				m_texture->Release();
				m_texture = nullptr;
			}
		}

		m_hresult = hr;
	}

	void draw(int x, int y, c_color color) {
		if (!m_device || !m_texture || !m_sprite)
			return;

		D3DXMATRIX matrix{};
		D3DXMatrixTranslation(&matrix, static_cast<float>(x), static_cast<float>(y), 0.0f);

		m_sprite->SetTransform(&matrix);
		m_sprite->Draw(m_texture, 0, 0, 0, color.get());
	}

	int get_width() {
		return m_width;
	}

	int get_height() {
		return m_height;
	}

	HRESULT get_result() {
		return m_hresult;
	}

private:
	int                m_width{};
	int                m_height{};
	IDirect3DDevice9*  m_device{};
	ID3DXSprite*       m_sprite{};
	IDirect3DTexture9* m_texture{};
	std::string        m_path{};
	const byte*        m_image{};
	size_t             m_image_size{};
	HRESULT            m_hresult{};
};

class c_renderer {
public:
	bool init(IDirect3DDevice9* device);
	bool restore(IDirect3DDevice9* device);

	void set_render_states();

	void set_viewport(D3DVIEWPORT9 vp);
	D3DVIEWPORT9 get_viewport();

	void rect(vec2 pos, vec2 size, c_color c);
	void rect(float x, float y, float w, float h, c_color c) {
		return rect({ x, y }, { w, h }, c);
	}

	void rect_fill(vec2 pos, vec2 size, c_color c);
	void rect_fill(float x, float y, float w, float h, c_color c) {
		return rect_fill({ x, y }, { w, h }, c);
	}

	void rect_cornered(vec2 pos, vec2 size, float radii, c_color c);
	void rect_cornered(float x, float y, float w, float h, float radii, c_color c) {
		return rect_cornered({ x, y }, { w, h }, radii, c);
	}

	void rect_fill_cornered(vec2 pos, vec2 size, float radii, c_color c);
	void rect_fill_cornered(float x, float y, float w, float h, float radii, c_color c) {
		return rect_fill_cornered({ x, y }, { w, h }, radii, c);
	}

	void gradient_v(vec2 pos, vec2 size, c_color c_a, c_color c_b);
	void gradient_v(float x, float y, float w, float h, c_color c_a, c_color c_b) {
		return gradient_v({ x, y }, { w, h }, c_a, c_b);
	}

	void gradient_h(vec2 pos, vec2 size, c_color c_a, c_color c_b);
	void gradient_h(float x, float y, float w, float h, c_color c_a, c_color c_b) {
		return gradient_h({ x, y }, { w, h }, c_a, c_b);
	}

	void gradient_multi_fill(vec2 pos, vec2 size, c_color c_a, c_color c_b, c_color c_c, c_color c_d);
	void gradient_multi_fill(float x, float y, float w, float h, c_color c_a, c_color c_b, c_color c_c, c_color c_d) {
		return gradient_multi_fill({ x, y }, { w, h }, c_a, c_b, c_c, c_d);
	}

	void gradient_multi(vec2 pos, vec2 size, c_color c_a, c_color c_b, c_color c_c, c_color c_d);
	void gradient_multi(float x, float y, float w, float h, c_color c_a, c_color c_b, c_color c_c, c_color c_d) {
		return gradient_multi({ x, y }, { w, h }, c_a, c_b, c_c, c_d);
	}

	void line(vec2 a, vec2 b, c_color c);
	void line(float x, float y, float w, float h, c_color c) {
		return line({ x, y }, { w, h }, c);
	}

	void circle(vec2 center, float radius, c_color c);
	void circle(float x, float y, float radius, c_color c) {
		return circle({ x, y }, radius, c);
	}

	void circle_fill(vec2 center, float radius, c_color c);
	void circle_fill(float x, float y, float radius, c_color c) {
		return circle_fill({ x, y }, radius, c);
	}

	void corner_box(vec2 pos, vec2 size, float cx, float cy, c_color c);
	void corner_box(float x, float y, float w, float h, float cx, float cy, c_color c) {
		return corner_box({ x, y }, { w, h }, cx, cy, c);
	}

	void build_lookup_table();

	static std::shared_ptr<c_renderer> make_shared() {
		return std::shared_ptr<c_renderer>(new c_renderer());
	}

	IDirect3DDevice9* get_device() {
		return m_device;
	}

	vec2 get_screen_size() {
		return m_screen_size;
	}

	void begin();
	void end();

	void undo();

private:
	IDirect3DDevice9*            m_device;
	IDirect3DStateBlock9*        m_state_block;
	IDirect3DVertexDeclaration9* m_vert_dec;
	IDirect3DVertexShader9*      m_vert_shader;
	std::vector<vec2>            m_lookup_table;
	std::vector<vec3>            m_lookup_sphere;
	vec2                         m_screen_size;
};

using sprite_t = c_sprite_mgr;
inline std::shared_ptr<c_renderer> g_renderer = c_renderer::make_shared();