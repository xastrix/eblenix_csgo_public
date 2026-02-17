#pragma once

#include "common.h"
#include <shared_defines.h>

#include <string>
#include <vector>
#include <mutex>

enum _game_status {
	gameVersionOutdated,
	gameVersionOK,
};

enum _module_list {
	clientDLL,
#ifdef CSGO_2020_BUILD
	clientPanoramaDLL,
#endif
	engineDLL,
	shaderapidx9DLL,
	vstdlibDLL,
	vgui2DLL,
	vguiMatSurfaceDLL,
	materialsystemDLL,
	serverBrowserDLL,
	maxModules,
};

enum _csgo_versions {
#ifdef CSGO_2020_BUILD
	CSGO_2020_PATCH,
#else
	CSGO_2023_PATCH,
#endif
	maxVersions,
};

enum _boolean_flags {
	BF_INITIALISED,
	BF_PANIC,
	BF_CONSOLE_OPENED,
	BF_CHAT_OPENED,
	BF_NIGHTMODE_HOLD,
	maxBooleanFlags,
};

enum _once_flags {
	OF_INIT_RENDER_STUFF,
	maxOnceFlags,
};

namespace g
{
	inline bool b_flags[maxBooleanFlags];
	inline std::once_flag of_flags[maxOnceFlags];

	inline int screen_width{},
		       screen_height{};

	inline _game_status status{};

	inline std::string module_list[maxModules] = {
		"client.dll",
#ifdef CSGO_2020_BUILD
		"client_panorama.dll",
#endif
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"vguimatsurface.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
	};

	inline std::string csgo_version_list[maxVersions] = {
#ifdef CSGO_2020_BUILD
		"1.37.3.*", // ~2020 build
#else
		"1.38.8.1", // Oct 12 2023 build
#endif
	};

	void unload();
}

#define GLOBAL(x) g::x