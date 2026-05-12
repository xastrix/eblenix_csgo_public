#include "input.h"

#include "globals.h"
#include "fnv.h"
#include "hud.h"
#include "ui.h"
#ifdef LUA_ENABLED
#include "luas.h"
#endif
#include "helpers.h"

#include <algorithm>

static unsigned long WINAPI wnd_proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	if (GLOBAL(b_flags[BF_INITIALISED]))
	{
		// By pressing the unload button in the injector, you are sending a message,
		// and here we intercept it and call the unload function.
		if (m == WM_COPYDATA) {
			const auto pcds = (PCOPYDATASTRUCT)l;

			if (pcds && pcds->dwData == 1) {
				switch (fnv::hash(static_cast<const char*>(pcds->lpData))) {
				case fnv::hash(LOADER_UNLOAD_HOOK_MESSAGE): {
					g::unload();
					return true;
				}
				}
			}
		}

		if (!(GLOBAL(lib_state.get_state()) == state_t::SL_SHUTDOWN))
		{
			g_input->process_message(m, w, l);

#ifdef LUA_ENABLED
			for (auto _ : LUA_CALLBACK(CL_ON_WND_PROC)) {
				auto result = _.fn(m, w, l);
				if (!result.valid()) {
					sol::error err = result;
					Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
				}
			}
#endif
		}
	}

	return CallWindowProcA(g_input->get_wnd_proc(), h, m, w, l);
}

void c_input::init(const std::pair<LPCSTR, LPCSTR>& wnd)
{
	m_hwnd = FindWindowA(wnd.first, wnd.second);

	if (m_hwnd == NULL)
		return;

	m_old_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc)));
}

void c_input::add_hk(unsigned int vk, std::function<void()> fn)
{
	m_hotkeys[vk] = fn;
}

void c_input::process_message(UINT m, WPARAM w, LPARAM l)
{
	if (!GLOBAL(b_flags[BF_CONSOLE_OPENED]) && !GLOBAL(b_flags[BF_CHAT_OPENED]))
		process_keybd_message(m, w);

	process_mouse_message(m, w, l);
}

void c_input::process_mouse_message(UINT m, WPARAM w, LPARAM l)
{
	auto menu_opened = g_ui->get_menu_state();
	
	// emulate WM_KEYDOWN, WM_KEYUP for mouse wheel control
	auto emul_key_for_wheel_navigation = [&](unsigned int k) {
		if (!(menu_opened && g_var->get_as<bool>(V_UI_MOUSE_WHEEL_NAVIGATION).value()))
			return;

		process_keybd_message(WM_KEYDOWN, k);
		process_keybd_message(WM_KEYUP, k);
	};

	if (menu_opened) {
		/* moving the ui by position x, y using the mouse */
		auto ui_pos_x = g_var->get_as<int>(V_UI_POS_X).value();
		auto ui_pos_y = g_var->get_as<int>(V_UI_POS_Y).value();

		static draggable_object_t ui_drag_obj{ ui_pos_x, ui_pos_y, 180, 255 };

		if (move_object(ui_drag_obj, m)) {
			g_var->set(V_UI_POS_X, ui_drag_obj.m_x);
			g_var->set(V_UI_POS_Y, ui_drag_obj.m_y);
		}

		g_hud->process_message(m);
	}

	switch (m) {
	case WM_MOUSEMOVE: {
		m_mouse_pos_x = LOWORD(l);
		m_mouse_pos_y = HIWORD(l);
		break;
	}
	case WM_LBUTTONDOWN: {
		emul_key_for_wheel_navigation(VK_LEFT);
		break;
	}
	case WM_RBUTTONDOWN: {
		emul_key_for_wheel_navigation(VK_RIGHT);
		break;
	}
	case WM_MOUSEWHEEL: {
		m_wheel_accumulate += GET_WHEEL_DELTA_WPARAM(w);

		while (m_wheel_accumulate >= WHEEL_DELTA) {
			m_wheel_accumulate -= WHEEL_DELTA;

			emul_key_for_wheel_navigation(VK_UP);
		}

		while (m_wheel_accumulate <= -WHEEL_DELTA) {
			m_wheel_accumulate += WHEEL_DELTA;

			emul_key_for_wheel_navigation(VK_DOWN);
		}
		break;
	}
	}
}

void c_input::process_keybd_message(UINT m, WPARAM w)
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

	if (state == state_up && m_key_map[key] == state_down)
	{
		m_key_map[key] = state_pressed;

		g_ui->handle_toggle_keys(key);

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
		if (g_ui->get_menu_state())
			g_ui->handle_input(key);
	}
}

std::wstring c_input::virtual_key_to_wstring(unsigned int vk)
{
	std::wstring ret = L"?";
	auto         map = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC);

	switch (vk) {
	case 0: return ret;
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
			ret = buf;
			std::transform(ret.begin(), ret.end(), ret.begin(), toupper);
		}
	}

	return ret;
}

WNDPROC c_input::get_wnd_proc()
{
	return m_old_wnd_proc;
}

void c_input::undo()
{
	if (m_old_wnd_proc)
		SetWindowLongA(m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_old_wnd_proc));

	m_old_wnd_proc = 0;
}