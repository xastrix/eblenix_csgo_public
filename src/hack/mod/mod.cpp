#include "mod.h"

void mod::init(void* I)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto h = CreateThread(0, 0, [](void* I) -> unsigned long __stdcall
	{
		do {
			/* looking for csgo */
			g_input.set_window(FindWindowA(CSGO_CLASS_NAME, 0));
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		} while (!g_input.get_window());

		while (!util::wait_for_module(g.module_list[serverBrowserDLL])) {
			/* waiting for the last module that the game loads */
			std::this_thread::sleep_for(std::chrono::milliseconds(600));
		}

		while (!util::wait_for_module(g.module_list[clientDLL])) {
			/* if client.dll is not present, then we try to find client_panorama.dll */
			g.module_list[clientDLL] = g.module_list[clientPanoramaDLL];
		}

		/* initialise variables (config system and etc) */
		g_vars.init();

		/* initialise signatures */
		g_sig.init();

		/* initialise interfaces */
		g_csgo.init();

		/* loading fonts into the system from memory */
		fonts::init();

		/* check version of the game */
		g.m_status = []() {
			for (int i = noneVersion; i < maxVersions; i++) {
				if (i == noneVersion) continue;

				if (g_csgo.m_engine->get_product_version_string() == g.csgo_versions[i]) {
					g_vars.set("misc->visual->viewmodel->fov", g_helpers.get_viewmodel_fov());
					g_vars.set("keys->on_toggle->ui", VK_INSERT);

					return gameVersionOK;
				}
			}

			return gameVersionOutdated;
		}();

		/* initialise input manager */
		g_input.init();

		/* initialise hooks (using minhook hooking library) */
		g_hooks.init();

		/* initialise event manager */
		g_events.init();

		if (g.m_initialized = static_cast<bool>(g.m_status)) {
			/* registration unload button */
			g_input.add_hk(VK_F12, []() {
				g.done();
			});

			/* opening ui */
			g.m_ui_opened = true;
		}
		else {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			g.done();
		}

		while (!g.m_done)
			std::this_thread::sleep_for(std::chrono::milliseconds(700));

		/* reset vars before unload */
		g_vars.reset();

		FreeLibraryAndExitThread(static_cast<HMODULE>(I), EXIT_SUCCESS);
	}, I, 0, 0);

	if (h)
		CloseHandle(h);
}

void mod::undo()
{
	if (!g.m_done)
		return;

	g_events.undo();
	g_hooks.undo();
	g_input.undo();
	g_vars.undo();
	fonts::undo();
}

bool __stdcall DllMain(const HMODULE mod, const int32_t r, void*)
{
	dll_t _dll{ r };

	_dll.in(DLL_PROCESS_ATTACH, [mod]() {
		mod::init(mod);
	});

	_dll.in(DLL_PROCESS_DETACH, []() {
		mod::undo();
	});

	return true;
}