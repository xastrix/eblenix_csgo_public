#pragma once

#include "../common.h"

#include <string>
#include <vector>

enum _module_list {
	clientDLL,
	engineDLL,
	shaderapidx9DLL,
	vstdlibDLL,
	vgui2DLL,
	vguiMatSurfaceDLL,
	materialsystemDLL,
	serverBrowserDLL,
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

namespace g
{
	inline bool b_flags[maxBooleanFlags];
	inline int i_flags[maxIntegerFlags];

	inline std::string module_list[maxModules] = {
		"client.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"vguimatsurface.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
	};

	void unload();
}

#define GLOBAL(x) g::x