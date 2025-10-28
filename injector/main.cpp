#include "interface.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	srand((unsigned)time(0));

	if (!(g_interface.init() == INTERFACE_FAILED))
	{
		g::game_processes[CSGO] = "csgo.exe";
		g::game_windows  [CSGO] = FindWindowA(CSGO_CLASS_NAME, 0);

		g::dlls          [CSGO] = "eblenix_csgo.dll";

		g_interface.on_loop();
	}

	g_interface.undo();

	return EXIT_SUCCESS;
}