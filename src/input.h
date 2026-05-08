#pragma once

#include <windows.h>
#include <functional>
#include <string>

#include "vec2.h"

enum _input_states {
	state_none = 1,
	state_down,
	state_up,
	state_pressed,
};

struct draggable_object_t {
	int m_x, m_y, m_width, m_height;
	bool m_dragging;
	int m_drag_offset_x, m_drag_offset_y;
};

struct mouse_t {
	friend class c_input;

	bool is_hovered(const vec2 min, const vec2 max) {
		return (m_mouse_pos_x >= min.x && m_mouse_pos_y >= min.y &&
			m_mouse_pos_x <= max.x && m_mouse_pos_y <= max.y);
	}

	bool move_object(draggable_object_t& o, UINT m) {
		switch (m) {
		case WM_LBUTTONDOWN: {
			if (m_mouse_pos_x >= o.m_x && m_mouse_pos_x <= o.m_x + o.m_width &&
				m_mouse_pos_y >= o.m_y && m_mouse_pos_y <= o.m_y + o.m_height) {
				o.m_dragging = true;
				o.m_drag_offset_x = m_mouse_pos_x - o.m_x;
				o.m_drag_offset_y = m_mouse_pos_y - o.m_y;
			}
			break;
		}
		case WM_MOUSEMOVE: {
			if (o.m_dragging) {
				o.m_x = m_mouse_pos_x - o.m_drag_offset_x;
				o.m_y = m_mouse_pos_y - o.m_drag_offset_y;
			}
			break;
		}
		case WM_LBUTTONUP: {
			o.m_dragging = false;
			break;
		}
		}

		return o.m_dragging;
	}

	int get_mouse_pos_x() {
		return m_mouse_pos_x;
	}

	int get_mouse_pos_y() {
		return m_mouse_pos_y;
	}

	int get_mouse_wheel_accumlate() {
		return m_wheel_accumulate;
	}

private:
	int m_mouse_pos_x;
	int m_mouse_pos_y;
	int m_wheel_accumulate;
};

class c_input : public mouse_t {
public:
	void init(const std::pair<LPCSTR, LPCSTR>& wnd);
	void add_hk(unsigned int vk, std::function<void()> fn);

	void process_message(UINT m, WPARAM w, LPARAM l);
	void process_mouse_message(UINT m, WPARAM w, LPARAM l);
	void process_keybd_message(UINT m, WPARAM w);

	std::wstring virtual_key_to_wstring(unsigned int vk);
	WNDPROC get_wnd_proc();

	static std::shared_ptr<c_input> make_shared() {
		return std::shared_ptr<c_input>(new c_input());
	}

	_input_states& operator[](unsigned int vk) {
		return m_key_map[vk];
	}

	void undo();

private:
	HWND                  m_hwnd;
	WNDPROC               m_old_wnd_proc;
	_input_states         m_key_map[256]{};
	std::function<void()> m_hotkeys[256]{};
};

inline std::shared_ptr<c_input> g_input = c_input::make_shared();

#define KEY(vk) g_input->operator[](vk)