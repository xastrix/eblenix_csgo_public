#pragma once

#include <common.h>
#include <windows.h>
#include <string>

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