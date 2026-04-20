#pragma once

#include <vector>
#include <d3d9.h>

#include "globals.h"
#include "util.h"

enum _interface_status {
	INTERFACE_LOADED,
	INTERFACE_FAILED,
};

struct Interface {
	_interface_status init();

	void loop();
	void on_reset();

	int get_width();
	int get_height();

	int get_mouse_pos_x();
	int get_mouse_pos_y();

	bool is_window_active();
	bool is_window_hovered();

	void set_window_pos(int x, int y);

	void undo();

private:
	bool update_frame() {
		MSG msg;

		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) return false;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		return true;
	}

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
	int                   m_mouse_pos_x{},
		                  m_mouse_pos_y{};
};

inline Interface g_interface;