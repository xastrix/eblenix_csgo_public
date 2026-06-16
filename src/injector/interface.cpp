#include "interface.h"

#include "ui.h"
#include "resource.h"

#include <common.h>
#include <dwmapi.h>

_interface_status Interface::init(HINSTANCE instance)
{
	m_window_name = util::random_string(12);
	m_class_name = util::random_string(12);

	memset(&m_wc, 0, sizeof(m_wc));
	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_VREDRAW | CS_HREDRAW;
	m_wc.lpfnWndProc = [](HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT CALLBACK
	{
		switch (m) {
		case WM_SYSCOMMAND: {
			if ((w & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		}

		return DefWindowProcA(h, m, w, l);
	};
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hInstance = instance;
	m_wc.hIcon = LoadIconA(m_wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	m_wc.hCursor = LoadCursorA(0, IDC_ARROW);
	m_wc.lpszMenuName = 0;
	m_wc.lpszClassName = m_class_name.c_str();
	m_wc.hIconSm = m_wc.hIcon;

	RegisterClassExA(&m_wc);

	m_hwnd = CreateWindowExA(WS_EX_LAYERED, m_class_name.c_str(), m_window_name.c_str(), WS_POPUP | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, nullptr, nullptr, nullptr, 0);

	if (!m_hwnd) {
		UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
		return INTERFACE_FAILED;
	}

	m_nid.cbSize = sizeof(NOTIFYICONDATAA);
	m_nid.hWnd = m_hwnd;
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON;
	m_nid.hIcon = m_wc.hIcon;
	Shell_NotifyIconA(NIM_ADD, &m_nid);

	SetLayeredWindowAttributes(m_hwnd, RGB(1, 1, 1), 0, LWA_COLORKEY);

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

	g_font.init(m_device, {
		{ Tahoma12px, 12, "Tahoma", FW_MEDIUM, ANTIALIASED_QUALITY },
		{ Verdana12px, 12, "Verdana", FW_NORMAL, PROOF_QUALITY },
		{ Verdana13px, 13, "Verdana", FW_NORMAL, PROOF_QUALITY },
	});

	g_renderer.init(m_device);

	return INTERFACE_LOADED;
}

void Interface::loop()
{
	c_game* games[1];

	g_ui.set_size(c_vec2(m_width, m_height));

	if (games[0] = g_ui.set_game("CS:GO")) {
		g_ui.set_group(games[0], "Description", c_vec2(330, 230), [](c_group* ctx) {
			ctx->add_child(c_ui_mem_pool::alloc<c_label>("A hack for counter-strike global offensive, created"));
			ctx->add_child(c_ui_mem_pool::alloc<c_label>("solely for educational purposes in the process of"));
			ctx->add_child(c_ui_mem_pool::alloc<c_label>("learning gamehacking."));
		});

		games[0]->set_cursor_pos(c_vec2(155, 210));

		g_ui.set_group(games[0], "Actions", c_vec2(210, 80), [this](c_group* ctx) {
			ctx->add_child(c_ui_mem_pool::alloc<c_button>("Load", [this]() {
				std::string dll = "eblenix_csgo.dll";

				if (!util::file_exists(dll))
					return push_windows_notify(_PRODUCT_NAME, dll + " not found", NIIF_WARNING);

				DWORD pid = util::get_proc_id("csgo.exe");

				if (!pid)
					return push_windows_notify(_PRODUCT_NAME, "CSGO not running", NIIF_WARNING);

				if (!util::inject(pid, dll))
					return push_windows_notify(_PRODUCT_NAME, "Failed to inject", NIIF_ERROR);

				return PostQuitMessage(0);
			}));
		}, true);
	}

	while (update_frame())
	{
		g_ui.poll_input(m_hwnd);

		m_device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(1, 1, 1), 1.0f, 0);

		if (SUCCEEDED(m_device->BeginScene()))
		{
			g_renderer.begin();
			{
				g_ui.think();
				g_ui.draw();
			}
			g_renderer.end();

			m_device->EndScene();
		}

		on_reset();
	}
}

void Interface::on_reset()
{
	auto result = m_device->Present(0, 0, 0, 0);
	if (!(result == D3DERR_DEVICELOST && m_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET))
		return;

	g_font.undo();
	g_renderer.undo();

	m_device->Reset(&m_present_params);

	g_renderer.init(m_device);
	g_font.init(m_device, {});
}

void Interface::undo()
{
	c_ui_mem_pool::clear();

	m_nid.cbSize = sizeof(NOTIFYICONDATAA);
	m_nid.hWnd = m_hwnd;
	m_nid.uID = 1;
	Shell_NotifyIconA(NIM_DELETE, &m_nid);

	DestroyWindow(m_hwnd);
	UnregisterClassA(m_class_name.c_str(), m_wc.hInstance);
}

void Interface::push_windows_notify(const std::string& title, const std::string& msg, DWORD icon_type)
{
	m_nid.uFlags = NIF_INFO;

	strcpy_s(m_nid.szInfoTitle, title.c_str());
	strcpy_s(m_nid.szInfo, msg.c_str());

	m_nid.dwInfoFlags = icon_type;

	Shell_NotifyIconA(NIM_MODIFY, &m_nid);
}