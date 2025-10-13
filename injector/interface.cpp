#include "interface.h"
#include "ui.h"

#include "csgo_icon.hpp"

Interface g_interface;

bool           loop{ true };
std::once_flag init_d3d{};
std::string    dlls[]{ "eblenix_csgo.dll" };
std::string    game_processes[]{ "csgo.exe" };
HWND           game_windows[]{ FindWindowA(CSGO_CLASS_NAME, 0) };

sprite_t       csgo_icon{};

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
			loop = false;
		}
		}

		return DefWindowProcA(h, m, w, l);
	};
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = 0;
	m_wc.hIcon = 0;
	m_wc.hCursor = LoadCursorA(0, IDC_ARROW);
	m_wc.lpszMenuName = 0;
	m_wc.lpszClassName = m_class_name.c_str();
	m_wc.hIconSm = 0;

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

LPDIRECT3DDEVICE9 Interface::get_device()
{
	return m_device;
}

D3DPRESENT_PARAMETERS& Interface::get_present_params()
{
	return m_present_params;
}

void Interface::on_loop()
{
	while (loop) {
		MSG msg{};
		while (PeekMessageA(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				loop = false;
		}

		if (!loop)
			break;

		POINT point{};
		RECT rect{};

		GetCursorPos(&point);
		GetWindowRect(m_hwnd, &rect);

		if (is_focused()) {
			if ((m_mouse_pos_x <= m_width && m_mouse_pos_y <= 25) && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
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

		m_device->Clear(
			0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 68, 90, 129),
			1.0f, 0
		);

		if (!(FAILED(m_device->BeginScene())))
		{
			std::call_once(init_d3d, [&]() {
				if (g_d3d.init(m_device))
					csgo_icon.init(m_device, csgo_ico, sizeof(csgo_ico), 24, 24);
			});

			g_d3d.begin_render_states();
			{
				csgo_icon.begin();
				{
					static auto game_id{ -1 };
					auto        game_selected{ game_id != -1 };
					auto        game_process_info{ util::get_proc(game_selected ? game_processes[game_id] : "") };

					i_group_box Games{ "Games", 20, 20, game_selected ? 200 : m_width - 40, 80 }; {
						i_game_selector CSGO{ &Games, csgo_icon, "CS:GO", game_process_info.active ? true : false, &game_id, 0 };
					}

					if (game_selected)
					{
						i_group_box Actions{ "Actions", 230, 20, 135, 80 };
						{
							auto dll_used{ util::is_dll_used(dlls[game_id]) };

							i_button Load{ &Actions, dll_used ? "Unload" : "Load", 115, 24, [&]() {
								switch (dll_used) {
								case true: {
									util::send_msg_to_proc(game_windows[game_id], LOADER_UNLOAD_HOOK_MESSAGE);
									break;
								}
								case false: {
									if (!util::file_exists(dlls[game_id])) {
										MessageBoxA(0, std::string{ "Module " + dlls[game_id] + " not found in current directory" }.c_str(), "eblenix_injector", MB_OK);
										return;
									}

									if (!game_process_info.active)
										return;

									util::inject(game_process_info.id, dlls[game_id]);
								}
								}
							} };

							i_button Exit{ &Actions, "Exit", 115, 24, []() {
								loop = false;
							} };
						}
					}
				}
				csgo_icon.end();
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
	csgo_icon.on_reset();

	m_device->Reset(&m_present_params);

	csgo_icon.on_reset_end();
	g_d3d.create_objects();
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

bool Interface::is_focused()
{
	return (GetForegroundWindow() == m_hwnd && IsWindowVisible(m_hwnd));
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