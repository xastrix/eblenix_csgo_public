#include "input.h"
#include "events.h"
#include "hooks.h"
#include "states.h"

#include "fonts.hpp"
static DWORD  g_numFonts;
static HANDLE g_fonts[1];

#include <exception_handler.hpp>

static void __stdcall init(HMODULE I)
{
	// waiting for the server browser module (the last module loaded by the game)
	if (Helpers::wait_for_module(serverBrowserDLL, 600) == WM_TIMEOUT)
		g_state->set_current_state(SL_SHUTDOWN);

	// waiting for the client module
	if (Helpers::wait_for_module(clientDLL, 200) == WM_TIMEOUT)
		g_state->set_current_state(SL_SHUTDOWN);

	// initialize the timer by recording the current steady clock time
	auto s_time = std::chrono::steady_clock::now();

	g_state->call_state(SL_INIT_BASE, [](state_t& state) {
		g_fonts[0] = AddFontMemResourceEx(astriumwep_ttf, ASTRIUMWEP_TTF_SZ, NULL, &g_numFonts);
		g_fonts[1] = AddFontMemResourceEx(smallestpixel7_ttf, SMALLESTPIXEL7_TTF_SZ, NULL, &g_numFonts);

		g_var->init();
		g_sig->init();
		g_cs->init();

		g_font->init(g_cs->m_device, {
			{ Tahoma12px,     12, "Tahoma",           FW_MEDIUM,   ANTIALIASED_QUALITY },
			{ Verdana12px,    12, "Verdana",          FW_SEMIBOLD, ANTIALIASED_QUALITY },
			{ SmallFonts10px, 10, "Smallest Pixel-7", FW_THIN,     ANTIALIASED_QUALITY },
			{ Astriumwep12px, 12, "AstriumWep",       FW_NORMAL,   CLEARTYPE_QUALITY },
			{ Astriumwep16px, 16, "AstriumWep",       FW_NORMAL,   CLEARTYPE_QUALITY },
			{ Astriumwep25px, 25, "AstriumWep",       FW_NORMAL,   CLEARTYPE_QUALITY },
	    });

		if (g_renderer->init(g_cs->m_device))
			g_ui->init(g_cs->m_device);

#ifdef LUA_ENABLED
		g_lua->init();

		for (auto _ : LUA_CALLBACK(CL_ON_PREINIT)) {
			auto result = _.fn();
			if (!result.valid()) {
				sol::error err = result;
				Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
			}
		}
#endif
		state++;
	});

	g_state->call_state(SL_INIT_VARS, [](state_t& state) {
		g_var->set(V_VISUALS_INTERFACE_SPECTATORS_POS_Y, g_renderer->get_screen_size().y * 0.5f);
		g_var->set(V_MISC_VISUAL_VIEWMODEL_FOV, Helpers::get_viewmodel_fov());

		g_var->set(V_UI_POS_X, 80);
		g_var->set(V_UI_POS_Y, 80);

		g_var->set(V_KEYS_ON_TOGGLE_UI, VK_INSERT);

		state++;
	});

	g_state->call_state(SL_INIT_HOOKS, [](state_t& state) {
		g_input->init({ CSGO_CLASS_NAME, 0 });
		g_hooks->init();
		g_event->init();

		state++;
	});

	g_state->call_state(SL_WAITING_FOR_SHUTDOWN, [s_time](state_t& state) {
		int last_min  = 0;
		int last_hour = 0;

		// set initialised flag
		GLOBAL(b_flags[BF_INITIALISED]) = true;

#ifdef LUA_ENABLED
		// register a lua callback for the cheat load event
		for (auto _ : LUA_CALLBACK(CL_ON_INIT)) {
			auto result = _.fn();
			if (!result.valid()) {
				sol::error err = result;
				Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
			}
		}
#endif

		// set unload key
		g_input->add_hk(VK_F12, []() {
			g::unload();
		});

		// open the ui
		g_ui->set_menu_state(true);

		while (!(state == SL_SHUTDOWN)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			int total_minutes = Helpers::get_elapsed_time(s_time) / 60000;
			int total_hours = total_minutes / 60;

			if (total_hours != last_hour) {
				last_hour = total_hours;

				GLOBAL(i_flags[IF_HOURS_IN_GAME]) = 0;

				GLOBAL(i_flags[IF_MINUTES_IN_GAME]) = 0;
				GLOBAL(i_flags[IF_SECONDS_IN_GAME]) = 0;

				last_min = 0;
			}

			if (total_minutes != last_min) {
				last_min = total_minutes;
				GLOBAL(i_flags[IF_SECONDS_IN_GAME]) = 0;
			}
			else {
				GLOBAL(i_flags[IF_SECONDS_IN_GAME]) = (Helpers::get_elapsed_time(s_time) % 60000) / 1000;
			}

			GLOBAL(i_flags[IF_MINUTES_IN_GAME]) = total_minutes % 60;
			GLOBAL(i_flags[IF_HOURS_IN_GAME]) = total_hours;
		}
	});

	g_state->call_state(SL_SHUTDOWN, [I](state_t& state) {
		// reset world brightness if nightmode was enabled
		if (g_var->get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_var->get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value()) {
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
		g_var->reset();

#ifdef LUA_ENABLED
		// register a lua callback for the cheat unload event
		for (auto _ : LUA_CALLBACK(CL_ON_UNLOAD)) {
			auto result = _.fn();
			if (!result.valid()) {
				sol::error err = result;
				Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
			}
		}
#endif

		// free stuff
		g_event->undo();
		g_hooks->undo();
		g_input->undo();
#ifdef LUA_ENABLED
		g_lua->undo();
#endif
		g_var->undo();

		// free loaded fonts
		for (int i = 0; i <= 1; i++) {
			if (g_fonts[i]) {
				RemoveFontMemResourceEx(g_fonts[i]);
				g_fonts[i] = nullptr;
			}
		}

		g_numFonts = 0;

		// exit thread
		FreeLibraryAndExitThread(I, EXIT_SUCCESS);
	});
}

bool __stdcall DllMain(const HMODULE mod, const int32_t r, void*)
{
	if (!(r == DLL_PROCESS_ATTACH))
		return false;

	// disables thread library calls
	DisableThreadLibraryCalls(mod);

	// sets a custom handler for unhandled exceptions
	SetUnhandledExceptionFilter(exception_handler::custom_exception_filter);

	if (const auto h = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), mod, 0, nullptr))
		CloseHandle(h);

	return true;
}