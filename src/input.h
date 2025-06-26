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

struct mouse_t {
	friend struct input;

	int get_mouse_pos_x() {
		return m_mouse_pos_x;
	}

	int get_mouse_pos_y() {
		return m_mouse_pos_y;
	}

	int get_mouse_wheel_accumlate() {
		return m_wheel_accumulate;
	}

	bool move_object(UINT m, int w, int h, int& x, int& y) {
		static bool dragging{};
		
		static int drag_offset_x{};
		static int drag_offset_y{};

		switch (m) {
		case WM_LBUTTONDOWN: {
			if (m_mouse_pos_x >= x && m_mouse_pos_x <= x + w &&
				m_mouse_pos_y >= y && m_mouse_pos_y <= y + h) {
				dragging = true;
				drag_offset_x = m_mouse_pos_x - x;
				drag_offset_y = m_mouse_pos_y - y;
			}
			break;
		}
		case WM_MOUSEMOVE: {
			if (dragging) {
				x = m_mouse_pos_x - drag_offset_x;
				y = m_mouse_pos_y - drag_offset_y;
			}
			break;
		}
		case WM_LBUTTONUP: {
			dragging = false;
			break;
		}
		}

		return dragging;
	}

private:
	int m_mouse_pos_x{};
	int m_mouse_pos_y{};
	int m_wheel_accumulate{};
};

struct input : public mouse_t {
	void init(const std::pair<LPCSTR, LPCSTR>& wnd);
	void add_hk(unsigned int vk, std::function<void()> fn);

	void process_message(UINT m, WPARAM w, LPARAM l);
	void process_mouse_message(UINT m, WPARAM w, LPARAM l);
	void process_keybd_message(UINT m, WPARAM w);

	std::wstring virtual_key_to_wstring(unsigned int vk);
	m_state get_key_state(unsigned int vk);
	WNDPROC get_wnd_proc();

	void undo();
private:
	HWND                  m_hwnd{};
	WNDPROC               m_old_wnd_proc{};
	m_state               m_key_map[256]{};
	std::function<void()> m_hotkeys[256]{};
};

inline input g_input;