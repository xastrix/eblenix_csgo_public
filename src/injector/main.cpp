#include "interface.h"

int __stdcall WinMain(HINSTANCE I, HINSTANCE, LPSTR, int32_t)
{
	srand((unsigned)time(0));

	if (g_interface.init(I) == INTERFACE_FAILED)
		return EXIT_FAILURE;

	g_interface.loop();
	g_interface.undo();

	return EXIT_SUCCESS;
}