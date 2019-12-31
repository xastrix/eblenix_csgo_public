#include "globals.h"

#include "ui.h"

void g::Unload()
{
	if (g_ui.get_menu_state())
		g_ui.set_menu_state(false);

	panic = true;
	state = CS_SHUTDOWN;
}