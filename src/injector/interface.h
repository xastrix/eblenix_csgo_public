#pragma once

#include <vector>
#include <d3d9.h>

#include "util.h"

enum _interface_status {
	INTERFACE_LOADED,
	INTERFACE_FAILED,
};

struct Interface {
	_interface_status init(HINSTANCE instance);

	void loop();
	void on_reset();

	int get_width() { return m_width; }
	int get_height() { return m_height; }

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

	void push_windows_notify(const std::string& title, const std::string& msg, DWORD icon_type = NIIF_INFO);

private:
	WNDCLASSEX            m_wc{};
	NOTIFYICONDATAA       m_nid{};
	HWND                  m_hwnd{};
	LPDIRECT3D9           m_d3d9{};
	LPDIRECT3DDEVICE9     m_device{};
	D3DPRESENT_PARAMETERS m_present_params{};
	std::string           m_window_name;
	std::string           m_class_name;
	int                   m_width{ 480 };
	int                   m_height{ 340 };
};

inline Interface g_interface;