#pragma once

#include <string>
#include <mutex>
#include <vector>

#define NOMINMAX

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

struct cs_globals {
	bool           m_panic_mode{},
	               m_ui_opened{},
	               m_console_visible{},
	               m_chat_visible{};
	int            m_screen_width{},
		           m_screen_height{};
	std::once_flag m_init_render{},
	               m_update_crosshair_col{};
};

struct globals : public cs_globals {
	bool           m_done{},
	               m_initialized{};
	_game_status   m_status{};

	std::string module_list[maxModules]{
		"client.dll",
		"client_panorama.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
	};

	std::string csgo_versions[maxVersions]{
		"1.37.3.1",
		"1.37.3.8",
	};

	void done() {
		m_ui_opened = false;
		m_panic_mode = true;
		m_done = true;
	}
};

inline globals g;