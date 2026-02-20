#include "globals.h"

#include "ui.h"
#include "states.h"

void g::unload()
{
	if (g_ui.get_menu_state())
		g_ui.set_menu_state(false);

	g_state->set_current_state(SL_SHUTDOWN);
}