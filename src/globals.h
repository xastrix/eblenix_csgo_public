#pragma once

#include <string>
#include <mutex>
#include <vector>

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
	noneVersion,
	firstVersion,
	lastVersion,
	maxVersions,
};

namespace g
{
	inline bool           done{},
		                  initialized{},
		                  panic_mode{},
	                      console_visible{},
	                      chat_visible{};

	inline int            screen_width{},
		                  screen_height{};

	inline std::once_flag init_render{},
	                      update_crosshair_col{};

	inline _game_status   status{};

	inline std::string    module_list[maxModules]{
		"client.dll",
		"client_panorama.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
	};

	inline std::string    csgo_versions[maxVersions]{
		"1.37.3.1",
		"1.37.3.8",
	};

	void unload();
}