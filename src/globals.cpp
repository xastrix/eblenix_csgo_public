#include "globals.h"

#include "ui.h"

void g::unload()
{
	if (g_ui.get_menu_state())
		g_ui.set_menu_state(false);

	panic_mode = true;
	done = true;
}