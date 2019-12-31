#include "input_manager.h"

#include "globals.h"
#include "ui.h"
#include "fnv.h"

#include <algorithm>

static unsigned long WINAPI wnd_proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (GLOBAL(initialised))
	{
		if (m == WM_COPYDATA) {
			const auto pcds = (PCOPYDATASTRUCT)l;

			if (pcds && pcds->dwData == 1) {
				switch (fnv::hash(static_cast<const char*>(pcds->lpData))) {
				case fnv::hash(LOADER_UNLOAD_HOOK_MESSAGE): {
					g::Unload();
					return true;
				}
				}
			}
		}

		if (GLOBAL(state) != CS_SHUTDOWN)
		{
			if (!GLOBAL(console_opened) && !GLOBAL(chat_opened))
			{
				g_input.process_message(m, w);
			}
		}
	}

	return CallWindowProcA(g_input.get_wnd_proc(), h, m, w, l);
}

void input_manager::init(const std::pair<LPCSTR, LPCSTR>& wnd)
{
	m_hwnd = FindWindowA(wnd.first, wnd.second);

	if (m_hwnd == NULL)
		return;

	m_old_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc)));
}

void input_manager::add_hk(unsigned int vk, std::function<void()> fn)
{
	m_hotkeys[vk] = fn;
}

WNDPROC input_manager::get_wnd_proc()
{
	return m_old_wnd_proc;
}

void input_manager::process_message(UINT m, WPARAM w)
{
	process_keybd_message(m, w);

	if (g_ui.get_menu_state() && g_vars.get_as<bool>(V_UI_MOUSE_WHEEL_NAVIGATION).value())
		process_mouse_message(m, w);
}

void input_manager::process_mouse_message(UINT m, WPARAM w)
{
	switch (m) {
	case WM_LBUTTONDOWN: {
		process_keybd_message(WM_KEYDOWN, VK_LEFT);
		process_keybd_message(WM_KEYUP, VK_LEFT);
		break;
	}
	case WM_RBUTTONDOWN: {
		process_keybd_message(WM_KEYDOWN, VK_RIGHT);
		process_keybd_message(WM_KEYUP, VK_RIGHT);
		break;
	}
	case WM_MOUSEWHEEL: {
		int wheel_accumulate{ 0 };

		wheel_accumulate += GET_WHEEL_DELTA_WPARAM(w);

		while (wheel_accumulate >= WHEEL_DELTA) {
			wheel_accumulate -= WHEEL_DELTA;

			process_keybd_message(WM_KEYDOWN, VK_UP);
			process_keybd_message(WM_KEYUP, VK_UP);
		}

		while (wheel_accumulate <= -WHEEL_DELTA) {
			wheel_accumulate += WHEEL_DELTA;

			process_keybd_message(WM_KEYDOWN, VK_DOWN);
			process_keybd_message(WM_KEYUP, VK_DOWN);
		}
		break;
	}
	}
}

void input_manager::process_keybd_message(UINT m, WPARAM w)
{
	int key = w;
	auto state = state_none;

	switch (m) {
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: {
		state = state_down;
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		state = state_up;
		break;
	}
	default:
		return;
	}

	if ((state == state_up && m_key_map[key] == state_down))
	{
		m_key_map[key] = state_pressed;

		g_ui.handle_toggle_keys(key);

		auto& hotkey_callback = m_hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();
	}
	else
	{
		m_key_map[key] = state;
	}

	if (state == state_down)
	{
		if (g_ui.get_menu_state())
			g_ui.handle_input(key);
	}
}

std::wstring input_manager::virtual_key_to_wstring(unsigned int vk)
{
	std::wstring k{ L"?" };
	auto map{ MapVirtualKeyW(vk, MAPVK_VK_TO_VSC) };

	switch (vk) {
	case 0: {
		return k;
	}
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_LWIN:
	case VK_RWIN:
	case VK_APPS:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_INSERT:
	case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		map |= KF_EXTENDED;
	default:
		wchar_t buf[128]{};
		if (GetKeyNameTextW(map << 16, buf, 128)) {
			k = buf;
			std::transform(k.begin(), k.end(), k.begin(), toupper);
		}
	}

	return k;
}

m_state input_manager::get_key_state(unsigned int vk)
{
	return m_key_map[vk];
}

void input_manager::undo()
{
	if (m_old_wnd_proc)
		SetWindowLongA(m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_old_wnd_proc));

	m_old_wnd_proc = 0;
}