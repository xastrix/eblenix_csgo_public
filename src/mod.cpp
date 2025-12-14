#include "mod.h"

#include "fonts.hpp"
static std::vector<font_resource_t> font_list = {
	{ AstriumwepRes, astriumwep_ttf, ASTRIUMWEP_TTF_SZ },
};

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

			g_fonts.init(font_list);
			g_vars.init();

			GLOBAL(state)++;
		}
		case CS_INIT_INTERFACES: {

			g_sig.init();
			g_csgo.init();

			GLOBAL(state)++;
		}
		case CS_CHECKING_GAME_VERSION: {

			auto setup_vars = []() {
				g_vars.set(V_MISC_VISUAL_VIEWMODEL_FOV, Helpers::get_viewmodel_fov());
				g_vars.set(V_KEYS_ON_TOGGLE_UI, VK_INSERT);
			};

			GLOBAL(status) = [setup_vars]() {
#ifndef DISABLE_CSGO_VERSION_CHECK
				const auto current_game_vers = std::string{ g_csgo.m_engine->get_product_version_string() };

				for (int i = 0; i < maxVersions; i++) {
					const auto target_ver = GLOBAL(csgo_version_list[i]);

					size_t pos = target_ver.find('*');
					if (pos == std::string::npos) {
						if (current_game_vers.compare(0, target_ver.size(), target_ver) == 0) {
							setup_vars();
							return gameVersionOK;
						}

						continue;
					}

					auto pat_ver = target_ver.substr(0, pos);
					if (pat_ver.empty())
						continue;

					if (pat_ver.back() == '.')
						pat_ver.pop_back();

					if (current_game_vers.compare(0, pat_ver.size(), pat_ver) == 0) {
#endif
						setup_vars();

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

				g_ui.set_menu_state(true);
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
	g_fonts.undo(font_list);
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