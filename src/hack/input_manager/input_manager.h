#pragma once

#include <windows.h>
#include <functional>
#include <string>

enum m_state {
	state_none = 1,
	state_down,
	state_up,
	state_pressed,
};

struct input_manager {
	void init();

	bool is_key_down(unsigned int vk);
	bool is_key_pressed(unsigned int vk);

	void add_hk(unsigned int vk, std::function<void(void)> fn);

	HWND get_window();
	WNDPROC get_wnd_proc();

	void set_window(const HWND hwnd);

	bool process_message(UINT m, WPARAM w, LPARAM l);
	bool process_keybd_message(UINT m, WPARAM w, LPARAM l);

	std::wstring virtual_key_to_wstring(unsigned int vk);

	void undo();
private:
	HWND                      m_hwnd{};
	WNDPROC                   m_old_wnd_proc{};
	m_state                   m_key_map[256]{};
	std::function<void(void)> m_hotkeys[256]{};
};

extern input_manager g_input;