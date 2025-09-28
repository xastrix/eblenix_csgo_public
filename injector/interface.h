#pragma once

#include <common.h>
#include <string>
#include <vector>
#include <mutex>
#include <d3d9.h>
#include <d3dx9.h>

#include "util.h"

enum _interface_status {
	INTERFACE_LOADED,
	INTERFACE_FAILED,
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

struct Interface {
	_interface_status init();

	LPDIRECT3DDEVICE9 get_device();
	D3DPRESENT_PARAMETERS& get_present_params();

	void on_loop();
	void on_reset();

	int get_width();
	int get_height();

	int get_mouse_pos_x();
	int get_mouse_pos_y();

	bool is_focused();

	void set_window_pos(int x, int y);

	void undo();
private:
	WNDCLASSEX            m_wc{};
	HWND                  m_hwnd{};
	LPDIRECT3D9           m_d3d9{};
	LPDIRECT3DDEVICE9     m_device{};
	D3DPRESENT_PARAMETERS m_present_params{};
	std::string           m_window_name{};
	std::string           m_class_name{};
	int                   m_width{ 385 };
	int                   m_height{ 120 };
	int                   m_mouse_pos_x{};
	int                   m_mouse_pos_y{};
};

struct d3d {
	bool init(IDirect3DDevice9* device);
	bool create_objects();

	void begin_render_states();
	void end_render_states();

	void draw_line(int x0, int y0, int x1, int y1, D3DCOLOR color);
	void draw_rect(int x, int y, int w, int h, D3DCOLOR color);
	void draw_filled_rect(int x, int y, int w, int h, D3DCOLOR color);
	void draw_string(const std::string& string, int x, int y, D3DCOLOR color);

	int get_text_width(const std::string& string);
	int get_text_height(const std::string& string);

	void undo();
private:
	IDirect3DDevice9*     m_device{};
	ID3DXLine*            m_line{};
	IDirect3DStateBlock9* m_block{};
	ID3DXFont*            m_font{};
};

struct sprite_manager {
	void init(IDirect3DDevice9* device, const byte* img, const size_t img_size, const int width, const int height);

	void begin();
	void end();

	void on_reset();
	void on_reset_end();

	void draw(const int x, const int y);

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

extern Interface g_interface;
extern d3d g_d3d;