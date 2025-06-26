#include "interface.h"

int __stdcall WinMain(HINSTANCE I, HINSTANCE, LPSTR, int32_t)
{
	srand((unsigned)time(0));

	g::instance = I;
	if (g_interface.init() == INTERFACE_FAILED)
		return EXIT_FAILURE;
	
	g::game_processes[CSGO] = "csgo.exe";
	g::game_windows  [CSGO] = FindWindowA(CSGO_CLASS_NAME, 0);

	g::dlls          [CSGO] = "eblenix_csgo.dll";

	g_interface.render();
	g_interface.undo();

	return EXIT_SUCCESS;
}