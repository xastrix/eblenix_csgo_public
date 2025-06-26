#include "interface.h"

#include "resource.h"
#include "ui.h"

// once_flag, call_once
#include <mutex>

static std::once_flag g_init_d3d{};

_interface_status Interface::init()
{
	m_window_name = util::random_string(12);
	m_class_name = util::random_string(12);

	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_VREDRAW | CS_HREDRAW;
	m_wc.lpfnWndProc = [](HWND h, UINT m, WPARAM w, LPARAM l) -> LONG_PTR {
		switch (m) {
		case WM_SYSCOMMAND: {
			if ((w & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		}
		case WM_DESTROY: {
			g::loop = false;
		}
		}

		return DefWindowProcA(h, m, w, l);
	};
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = g::instance;
	m_wc.hIcon = LoadIconA(m_wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	m_wc.hCursor = LoadCursorA(0, IDC_ARROW);
	m_wc.lpszMenuName = 0;
	m_wc.lpszClassName = m_class_name.c_str();
	m_wc.hIconSm = m_wc.hIcon;

	RegisterClassExA(&m_wc);

	m_hwnd = CreateWindowExA(WS_EX_TRANSPARENT, m_class_name.c_str(), m_window_name.c_str(), WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, nullptr, nullptr, nullptr, 0);

	if (!m_hwnd) {
		UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
		return INTERFACE_FAILED;
	}

	MoveWindow(m_hwnd, (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - m_height) / 2, m_width, m_height, TRUE);

	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);

	if (!(m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION))) {
		UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
		return INTERFACE_FAILED;
	}

	memset(&m_present_params, 0, sizeof(m_present_params));

	m_present_params.Windowed = TRUE;
	m_present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_present_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_present_params.hDeviceWindow = m_hwnd;
	m_present_params.EnableAutoDepthStencil = TRUE;
	m_present_params.AutoDepthStencilFormat = D3DFMT_D16;
	m_present_params.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_present_params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_present_params.BackBufferCount = 1;
	m_present_params.BackBufferWidth = m_width;
	m_present_params.BackBufferHeight = m_height;

	if (FAILED(m_d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_present_params, &m_device))) {
		m_d3d9->Release();
		UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
		return INTERFACE_FAILED;
	}

	return INTERFACE_LOADED;
}

void Interface::render()
{
	while (g::loop) {
		MSG msg{};
		while (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				g::loop = false;
		}

		if (!g::loop)
			break;

		POINT point{};
		RECT rect{};

		GetCursorPos(&point);
		GetWindowRect(m_hwnd, &rect);

		if (is_window_active() && is_window_hovered()) {
			if ((m_mouse_pos_x <= m_width && m_mouse_pos_y <= 25)
				&& GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				set_window_pos(m_mouse_pos_x, m_mouse_pos_y);
			}
			else {
				m_mouse_pos_x = (point.x - rect.left);
				m_mouse_pos_y = (point.y - rect.top);
			}
		}
		else {
			m_mouse_pos_x = 0;
			m_mouse_pos_y = 0;
		}

		g_ui.clear_frame(m_device);
		std::call_once(g_init_d3d, [&]() {
			if (g_d3d.init(m_device))
				g_ui.get->init(m_device, { m_width, m_height });
		});

		if (!(FAILED(m_device->BeginScene())))
		{
			g_d3d.begin_render_states();
			{
				g_ui.get->begin_frame();

				static int game_id;
				static auto game_process_id = util::get_proc_id(g::game_processes[game_id]);
				static bool loaded = util::is_dll_used(game_process_id, g::dlls[game_id]);

				g_ui.get->group_box("Games", { 20, 20 }, { 200, 80 }); {
					g_ui.get->pop_cursor_pos({ 21, 27 });
#ifndef CSGO_2020_BUILD
					g_ui.get->game_selector("CSGO", *g_ui.csgo_icon_sprite, &game_id, 0);
#else
					g_ui.get->game_selector("CSGO 2020", *g_ui.csgo_icon_sprite, &game_id, 0);
#endif
				}

				g_ui.get->group_box("Actions", { 230, 20 }, { 135, 80 }); {
					g_ui.get->button(loaded ? "Unload" : "Load", []() {
						if (game_id == -1) {
							g_ui.get->push_log("Select a game..");
							return;
						}

						if (!util::file_exists(g::dlls[game_id])) {
							g_ui.get->push_log("Check current directory. " + g::dlls[game_id] + " not found");
							return;
						}

						if (!game_process_id) {
							g_ui.get->push_log(g::game_processes[game_id] + " is not running");
							return;
						}

						if (util::is_dll_used(game_process_id, g::dlls[game_id])) {
							loaded = !(util::send_msg_to_proc(g::game_windows[game_id], LOADER_UNLOAD_HOOK_MESSAGE));
							return;
						}

						loaded = util::inject(game_process_id, g::dlls[game_id]);
					});

					g_ui.get->button("Exit", []() {
						g::loop = false;
					});
				}

				// haha
				g_ui.get->util_box({ m_mouse_pos_x, m_mouse_pos_y }, {
					{ "Exit", []() { g::loop = false; } },
				});

				g_ui.get->end_frame();
			}
			g_d3d.end_render_states();

			m_device->EndScene();
		}

		auto result = m_device->Present(0, 0, 0, 0);
		if ((result == D3DERR_DEVICELOST && m_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET))
			on_reset();
	}
}

void Interface::on_reset()
{
	g_d3d.undo();
	g_ui.get->on_reset();

	m_device->Reset(&m_present_params);

	g_ui.get->on_reset_end();
	g_d3d.create_objects(m_device);
}

int Interface::get_width()
{
	return m_width;
}

int Interface::get_height()
{
	return m_height;
}

int Interface::get_mouse_pos_x()
{
	return m_mouse_pos_x;
}

int Interface::get_mouse_pos_y()
{
	return m_mouse_pos_y;
}

bool Interface::is_window_active()
{
	return (m_hwnd == GetForegroundWindow());
}

bool Interface::is_window_hovered()
{
	return (m_mouse_pos_x >= 0 && m_mouse_pos_x <= 0 + m_width
		&& m_mouse_pos_y >= 0 && m_mouse_pos_y <= 0 + m_height);
}

void Interface::set_window_pos(int x, int y)
{
	int flags{ SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE };

	if (x != 0 && y != 0)
	{
		POINT p{};
		GetCursorPos(&p);

		x = p.x - x;
		y = p.y - y;

		flags &= ~SWP_NOMOVE;
	}

	if (m_width != 0 && m_height != 0)
		flags &= ~SWP_NOSIZE;

	SetWindowPos(m_hwnd, nullptr, x, y, m_width, m_height, flags);
}

void Interface::undo()
{
	g_d3d.undo();

	if (m_device) {
		m_device->Release();
		m_device = nullptr;
	}

	if (m_d3d9) {
		m_d3d9->Release();
		m_d3d9 = nullptr;
	}

	DestroyWindow(m_hwnd);
	UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
}