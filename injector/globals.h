#pragma once

#include "../common.h"

#include <string>
#include <windows.h>

enum _game_list {
	CSGO,
	maxGames,
};

namespace g
{
	inline
		HINSTANCE instance{};

	inline
		std::string dlls[maxGames]{};

	inline
		std::string game_processes[maxGames]{};

	inline
		HWND game_windows[maxGames]{};
}