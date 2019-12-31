#include "mod.h"

void mod::init(void* I)
{
	auto h = CreateThread(0, 0, [](void* I) -> unsigned long __stdcall
	{
		switch (GLOBAL(state)) {
		case CS_START_UP: {

			if (util::wait_for_module(serverBrowserDLL) == WM_TIMEOUT)
				GLOBAL(state) = CS_SHUTDOWN;

			if (util::wait_for_module(clientDLL, clientPanoramaDLL, 200) == WM_TIMEOUT)
				GLOBAL(state) = CS_SHUTDOWN;

			g_fonts.init();
			g_vars.init();

			GLOBAL(state)++;
		}
		case CS_INIT_INTERFACES: {

			g_sig.init();
			g_csgo.init();

			GLOBAL(state)++;
		}
		case CS_CHECKING_GAME_VERSION: {

			GLOBAL(status) = []() {
#ifndef DISABLE_CSGO_VERSION_CHECK
				for (int i = noneVersion; i < maxVersions; i++) {
					if (i == noneVersion)
						continue;

					if (g_csgo.m_engine->get_product_version_string() == GLOBAL(csgo_version_list[i])) {
#endif
						g_vars.set(V_MISC_VISUAL_VIEWMODEL_FOV, Helpers::get_viewmodel_fov());
						g_vars.set(V_KEYS_ON_TOGGLE_UI, VK_INSERT);

						return gameVersionOK;
#ifndef DISABLE_CSGO_VERSION_CHECK
					}
				}

				return gameVersionOutdated;
#endif
			}();

			GLOBAL(state)++;
		}
		case CS_INIT_CHEAT_STUFF: {

			g_input.init({ CSGO_CLASS_NAME, 0 });
			g_hooks.init();
			g_events.init();

			GLOBAL(state)++;
		}
		case CS_WAITING_FOR_SHUTDOWN: {

			if (GLOBAL(initialised) = static_cast<bool>(GLOBAL(status))) {
				g_input.add_hk(VK_F12, []() {
					g::Unload();
				});
			}
			else {
				std::this_thread::sleep_for(std::chrono::seconds(5));
				g::Unload();
			}

			while (GLOBAL(state) != CS_SHUTDOWN)
				std::this_thread::sleep_for(std::chrono::milliseconds(700));

		}
		case CS_SHUTDOWN: {

			g_vars.reset();
			FreeLibraryAndExitThread(static_cast<HMODULE>(I), EXIT_SUCCESS);

		}
		}
	}, I, 0, 0);

	if (h)
		CloseHandle(h);
}

void mod::undo()
{
	if (GLOBAL(state) != CS_SHUTDOWN)
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