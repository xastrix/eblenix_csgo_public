#pragma once

#include <common.h>
#include <string>
#include <vector>
#include <thread>

enum _module_list {
	clientDLL,
	engineDLL,
	shaderapidx9DLL,
	vstdlibDLL,
	vgui2DLL,
	vguiMatSurfaceDLL,
	materialsystemDLL,
	serverBrowserDLL,
	tier0DLL,
	localizeDLL,
	maxModules,
};

enum _boolean_flags {
	BF_INITIALISED,
	BF_PANIC,
	BF_CONSOLE_OPENED,
	BF_CHAT_OPENED,
	BF_NIGHTMODE_HOLD,
	BF_BOMB_PLANTED,
	maxBooleanFlags,
};

enum _integer_flags {
	IF_SECONDS_IN_GAME,
	IF_MINUTES_IN_GAME,
	IF_HOURS_IN_GAME,
	IF_BOMB_TIMER,
	IF_BOMB_SITE_ID,
	maxIntegerFlags,
};

struct state_t {
	enum _sl {
		SL_INIT_BASE,
		SL_INIT_VARS,
		SL_INIT_HOOKS,
		SL_WAITING_FOR_SHUTDOWN,
		SL_SHUTDOWN,
	};

	state_t& operator++() {
		++m_state;
		return *this;
	}

	state_t operator++(int) {
		state_t tmp = *this;
		++*this;
		return tmp;
	}

	void set_state(int state) {
		m_state = state;
	}

	int get_state() const {
		return m_state;
	}

private:
	int m_state;
};

namespace g
{
	inline bool b_flags[maxBooleanFlags]{};
	inline int i_flags[maxIntegerFlags]{};

	inline state_t lib_state;
	inline std::string module_list[maxModules] = {
		"client.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"vguimatsurface.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
		"tier0.dll",
		"localize.dll",
	};

	void handle_playing_time(const std::chrono::steady_clock::time_point start_time, int interval);
	void unload();
}

#define GLOBAL(x) g::x