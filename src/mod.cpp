#include "mod.h"

void mod::init(void* I)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto h = CreateThread(0, 0, [](void* I) -> unsigned long __stdcall
	{
		if (util::wait_for_module(serverBrowserDLL) == WM_TIMEOUT)
			goto _Ex;

		if (util::wait_for_module(clientDLL, clientPanoramaDLL, 200) == WM_TIMEOUT)
			goto _Ex;

		g_vars.init();
		g_sig.init();
		g_csgo.init();
		g_fonts.init();

		g::status = []() {
			for (int i = noneVersion; i < maxVersions; i++) {
				if (i == noneVersion) continue;

				if (g_csgo.m_engine->get_product_version_string() == g::csgo_versions[i]) {
					g_vars.set("misc->visual->viewmodel->fov", Helpers::get_viewmodel_fov());
					g_vars.set("keys->on_toggle->ui", VK_INSERT);

					return gameVersionOK;
				}
			}

			return gameVersionOutdated;
		}();

		g_input.init({ CSGO_CLASS_NAME, 0 });
		g_hooks.init();
		g_events.init();

		if (g::initialized = static_cast<bool>(g::status)) {
			g_input.add_hk(VK_F12, []() {
				g::unload();
			});
		}
		else {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			g::unload();
		}

		while (!g::done)
			std::this_thread::sleep_for(std::chrono::milliseconds(700));

		/* reset vars before unloading */
		g_vars.reset();

	_Ex:
		FreeLibraryAndExitThread(static_cast<HMODULE>(I), EXIT_SUCCESS);
	}, I, 0, 0);

	if (h)
		CloseHandle(h);
}

void mod::undo()
{
	if (!g::done)
		return;

	g_events.undo();
	g_hooks.undo();
	g_input.undo();
	g_vars.undo();
	g_fonts.undo();
}

bool __stdcall DllMain(const HMODULE mod, const int32_t r, void*)
{
	switch (r) {
	case DLL_PROCESS_ATTACH: {
		mod::init(mod);
		break;
	}
	case DLL_PROCESS_DETACH: {
		mod::undo();
		break;
	}
	}

	return true;
}