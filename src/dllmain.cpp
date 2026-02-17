#include "input.h"
#include "events.h"
#include "hooks.h"
#include "state_mgr.h"

#include "fonts.hpp"
static HANDLE g_astriumwepFont{};

static void __stdcall init(HMODULE I)
{
	// waiting for the server browser module (the last module loaded by the game)
	if (Helpers::wait_for_module(serverBrowserDLL, 600) == WM_TIMEOUT)
		g_state->set_current_state(SL_SHUTDOWN);

#ifdef CSGO_2020_BUILD
	// waiting for the client module
	// if not found, swap to client_panorama.dll and try again
	if (Helpers::wait_for_module(clientDLL, clientPanoramaDLL, 200) == WM_TIMEOUT)
		g_state->set_current_state(SL_SHUTDOWN);
#else
	// waiting for the client module
	if (Helpers::wait_for_module(clientDLL, 200) == WM_TIMEOUT)
		g_state->set_current_state(SL_SHUTDOWN);
#endif

	g_state->call_state(SL_INIT_BASE, [](state_t& state) {
		g_astriumwepFont = AddFontMemResourceEx(astriumwep_ttf, ASTRIUMWEP_TTF_SZ, NULL, 0);

		g_vars.init();
		g_sig.init();
		g_csgo.init();

		g_font.init(g_csgo.m_device, {
			{ Tahoma12px,     12, "Tahoma",     FW_MEDIUM, ANTIALIASED_QUALITY },
			{ Verdana12px,    12, "Verdana",    FW_SEMIBOLD, ANTIALIASED_QUALITY },
			{ Astriumwep12px, 12, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
			{ Astriumwep16px, 16, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
			{ Astriumwep25px, 25, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
	    });

		if (g_renderer.init(g_csgo.m_device))
			g_ui.init(g_csgo.m_device);

#ifndef CSGO_2020_BUILD
		// patch 'STEAM validation rejected' msgbox in the (non-steam) csgo
		Helpers::write<unsigned char>(reinterpret_cast<uintptr_t>(g_sig[S_STEAM_VALIDATION_REJECTED]), 0x75);
#endif
		state++;
	});

	g_state->call_state(SL_CHECK_GAME_VERSION, [](state_t& state) {
		auto setup_basic_vars = []() {
			g_vars.set(V_MISC_VISUAL_VIEWMODEL_FOV, Helpers::get_viewmodel_fov());

			g_vars.set(V_UI_POS_X, 80);
			g_vars.set(V_UI_POS_Y, 80);

			g_vars.set(V_UI_COL_R, 95);
			g_vars.set(V_UI_COL_G, 115);
			g_vars.set(V_UI_COL_B, 150);
			g_vars.set(V_UI_COL_A, 130);

			g_vars.set(V_KEYS_ON_TOGGLE_UI, VK_INSERT);
		};

		GLOBAL(status) = [setup_basic_vars]() {
#ifndef DISABLE_CSGO_VERSION_CHECK
			const auto current_game_vers = std::string{ g_csgo.m_engine->get_product_version_string() };

			for (int i = 0; i < maxVersions; i++) {
				const auto target_ver = GLOBAL(csgo_version_list[i]);

				size_t pos = target_ver.find('*');
				if (pos == std::string::npos) {
					if (current_game_vers.compare(0, target_ver.size(), target_ver) == 0) {
						setup_basic_vars();
						return gameVersionOK;
					}

					continue;
				}

				auto pat_ver = target_ver.substr(0, pos);
				if (pat_ver.empty())
					continue;

				if (pat_ver.back() == '.') pat_ver.pop_back();
				if (current_game_vers.compare(0, pat_ver.size(), pat_ver) == 0) {
					setup_basic_vars();
					return gameVersionOK;
				}
			}

			return gameVersionOutdated;
#else
			setup_basic_vars();
			return gameVersionOK;
#endif
		}();

		state++;
		});

	g_state->call_state(SL_INIT_HOOKS, [](state_t& state) {
		g_input.init({ CSGO_CLASS_NAME, 0 });
		g_hooks.init();
		g_event.init();

		state++;
	});

	g_state->call_state(SL_WAITING_FOR_SHUTDOWN, [](state_t& state) {
		// set initialised flag
		if (GLOBAL(b_flags[BF_INITIALISED]) = static_cast<bool>(GLOBAL(status))) {
			// set unload key
			g_input.add_hk(VK_F12, []() {
				g::unload();
			});

			// open the ui
			g_ui.set_menu_state(true);
		}
		else {
			// if not initialised, sleep 5s and then call the unload fn.

			// during this time, the cheat displays a notification
			// in the top-left corner indicating that the game version failed the check.

			// also, you can disable the game version check
			// by uncommenting DISABLE_CSGO_VERSION_CHECK in common.h.
			std::this_thread::sleep_for(std::chrono::seconds(5));
			g::unload();
		}

		while (!(state == SL_SHUTDOWN))
			std::this_thread::sleep_for(std::chrono::seconds(2));
	});

	g_state->call_state(SL_SHUTDOWN, [I](state_t& state) {
		// reset world brightness if nightmode was enabled
		if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_vars.get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value()) {
			Helpers::modulate_world_brightness({
				{ "World", 1.0f },
				{ "SkyBox", 1.0f },
				{ "Model", 1.0f },
				{ "Decal", 1.0f },
				{ "Other", 1.0f },
				});
		}

		// turn off all functions before unloading
		GLOBAL(b_flags[BF_PANIC]) = true;

		// reset all vars before unloading
		g_vars.reset();

#ifndef CSGO_2020_BUILD
		// write 0x74 to the address, restoring it to the original value
		Helpers::write<unsigned char>(reinterpret_cast<uintptr_t>(g_sig[S_STEAM_VALIDATION_REJECTED]), 0x74);
#endif
		// free stuff
		g_event.undo();
		g_hooks.undo();
		g_input.undo();
		g_vars.undo();

		// free loaded fonts
		if (g_astriumwepFont) {
			RemoveFontMemResourceEx(g_astriumwepFont);
			g_astriumwepFont = nullptr;
		}

		// exit thread
		FreeLibraryAndExitThread(I, EXIT_SUCCESS);
	});
}

bool __stdcall DllMain(const HMODULE mod, const int32_t r, void*)
{
	if (r != DLL_PROCESS_ATTACH)
		return false;

	if (auto h = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), mod, 0, nullptr))
		CloseHandle(h);

	return true;
}