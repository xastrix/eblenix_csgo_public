#include "globals.h"

#include "ui.h"

void g::unload()
{
	if (g_ui->get_menu_state())
		g_ui->set_menu_state(false);

	lib_state.set_state(state_t::SL_SHUTDOWN);
}