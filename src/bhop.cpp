#include "movement.h"

#include "vars.h"
#include "interfaces.h"

void bunnyhop_t::think(user_cmd_t* cmd)
{
	static uint8_t u{};

	if (!g_var->get_as<bool>(V_MISC_MOVEMENT_BUNNYHOP).value())
		return;

	if (g_cs->get_local()->get_move_type() == movetype_ladder ||
		g_cs->get_local()->get_move_type() == movetype_noclip ||
		g_cs->get_local()->get_move_type() == movetype_observer)
		return;

	if (!(u & 0x1) && (u & 0x2)) {
		u &= ~0x2;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (g_cs->get_local()->get_flags() & fl_onground) {
			u |= 0x1;
			u |= 0x2;
		}
		else {
			cmd->buttons &= ~in_jump;
			u &= ~0x1;
		}
	}
	else
		u &= ~0x3;
}