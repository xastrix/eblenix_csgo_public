#include "movement.h"

#include "interfaces.h"

void c_move::run(user_cmd_t* cmd)
{
	if (!(g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game()))
		return;

	if (!g_cs->get_local() || !g_cs->get_local()->is_life_state())
		return;

	bunnyhop_t::get_instance().think(cmd);
	autostrafe_t::get_instance().think(cmd);
	blockbot_t::get_instance().think(cmd);

	cmd->m_buttons |= in_bullrush;
}