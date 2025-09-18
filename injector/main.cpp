#include "interface.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	srand((unsigned)time(0));

	if (!(g_interface.init() == INTERFACE_FAILED))
		g_interface.on_loop();

	g_interface.undo();

	return EXIT_SUCCESS;
}