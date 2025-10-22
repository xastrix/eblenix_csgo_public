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
	void init(const std::pair<LPCSTR, LPCSTR>& wnd);
	void add_hk(unsigned int vk, std::function<void(void)> fn);

	WNDPROC get_wnd_proc();

	bool process_message(UINT m, WPARAM w, LPARAM l);
	bool process_keybd_message(UINT m, WPARAM w, LPARAM l);

	std::wstring virtual_key_to_wstring(unsigned int vk);
	m_state get_key_state(unsigned int vk);

	void undo();
private:
	HWND                      m_hwnd{};
	WNDPROC                   m_old_wnd_proc{};
	m_state                   m_key_map[256]{};
	std::function<void(void)> m_hotkeys[256]{};
};

inline input_manager g_input;