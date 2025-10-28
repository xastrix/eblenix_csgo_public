#pragma once

#include <string>
#include <windows.h>
#include <shared_defines.h>

enum _game_list {
	CSGO,
	maxGames,
};

namespace g
{
	inline
		std::string dlls[maxGames]{};

	inline
		std::string game_processes[maxGames]{};

	inline
		HWND game_windows[maxGames]{};
}