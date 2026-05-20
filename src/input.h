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

using hkfn_t = std::function<void(int)>;

struct draggable_object_t {
	draggable_object_t(int x, int y, int width, int height) :
		m_x(x), m_y(y), m_width(width), m_height(height) {}

	draggable_object_t(float x, float y, float width, float height) :
		m_x(static_cast<int>(x)), m_y(static_cast<int>(y)),
		m_width(static_cast<int>(width)), m_height(static_cast<int>(height)) {}

	int get_x() { return m_x; }
	void set_x(int x) { m_x = x; }

	int get_y() { return m_y; }
	void set_y(int y) { m_y = y; }

	int get_width() { return m_width; }
	int get_height() { return m_height; }

	int get_drag_x() { return m_drag_offset_x; }
	void set_drag_x(int x) { m_drag_offset_x = x; }

	int get_drag_y() { return m_drag_offset_y; }
	void set_drag_y(int y) { m_drag_offset_y = y; }

	bool get_dragging() { return m_dragging; }
	void set_dragging(bool v) { m_dragging = v; }

private:
	int  m_x, m_y,
		 m_width, m_height,
		 m_drag_offset_x, m_drag_offset_y;
	bool m_dragging;
};

class c_input {
public:
	void init(const std::pair<LPCSTR, LPCSTR>& wnd);
	void add_hk(unsigned int vk, hkfn_t fn) { m_hotkeys[vk] = fn; }

	void process_message(UINT m, WPARAM w, LPARAM l);
	void process_mouse_message(UINT m, WPARAM w, LPARAM l);
	void process_keybd_message(UINT m, WPARAM w);

	bool move_object(draggable_object_t& o, UINT m);

	std::wstring virtual_key_to_wstring(unsigned int vk);
	WNDPROC get_wnd_proc() { return m_old_wnd_proc; }

	_input_states& operator[](unsigned int vk) {
		return m_key_map[vk];
	}

	int get_mouse_pos_x() { return m_mouse_pos_x; }
	int get_mouse_pos_y() { return m_mouse_pos_y; }
	int get_mouse_wheel_accumlate() { return m_wheel_accumulate; }

	bool mouse_is_hovered(const vec2 min, const vec2 max) {
		return (m_mouse_pos_x >= min.x && m_mouse_pos_y >= min.y &&
                m_mouse_pos_x <= max.x && m_mouse_pos_y <= max.y);
	}

	void undo();

	static std::shared_ptr<c_input> make_shared() {
		return std::shared_ptr<c_input>(new c_input());
	}

private:
	HWND          m_hwnd;
	WNDPROC       m_old_wnd_proc;
	_input_states m_key_map[256]{};
	hkfn_t        m_hotkeys[256]{};
	int           m_mouse_pos_x;
	int           m_mouse_pos_y;
	int           m_wheel_accumulate;
};

inline std::shared_ptr<c_input> g_input = c_input::make_shared();

#define KEY(vk) g_input->operator[](vk)