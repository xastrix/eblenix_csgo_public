#pragma once

#include "common.h"

#include <string>
#include <mutex>
#include <vector>
#include <shared_defines.h>

enum _cheat_state {
	CS_START_UP,
	CS_INIT_INTERFACES,
	CS_INIT_CHEAT_STUFF,
	CS_CHECKING_GAME_VERSION,
	CS_WAITING_FOR_SHUTDOWN,
	CS_SHUTDOWN,
};

enum _game_status {
	gameVersionOutdated,
	gameVersionOK,
};

enum _module_list {
	clientDLL,
	clientPanoramaDLL,
	engineDLL,
	shaderapidx9DLL,
	vstdlibDLL,
	vgui2DLL,
	materialsystemDLL,
	serverBrowserDLL,
	maxModules,
};

enum _csgo_versions {
	CSGO_2020_PATCH,
	maxVersions,
};

namespace g
{
	inline bool initialised{},
		        panic{},
	            console_opened{},
	            chat_opened{};

	inline int screen_width{},
		       screen_height{};

	inline std::once_flag init_render_stuff{},
	                      update_crosshair_col{};

	inline bool visuals_nightmode_state{};

	inline int state{};
	inline _game_status status{};

	inline std::string module_list[maxModules] = {
		"client.dll",
		"client_panorama.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
	};

	inline std::string csgo_version_list[maxVersions] = {
		"1.37.3.*",
	};

	void Unload();
}