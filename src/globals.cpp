#include "globals.h"

#include "ui.h"

bool g::is_state(int index)
{
	return state == index;
}

void g::set_state(int index)
{
	state = index;
}

void g::unload()
{
	if (g_ui.get_menu_state())
		g_ui.set_menu_state(false);

	state = CS_SHUTDOWN;
}