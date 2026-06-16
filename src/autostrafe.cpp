#include "movement.h"

#include "vars.h"
#include "interfaces.h"

#include <algorithm>

void autostrafe_t::think(user_cmd_t* cmd)
{
	if (!g_var->get_as<bool>(V_MISC_MOVEMENT_AUTO_STRAFE).value())
		return;

	if (g_cs->get_local()->get_move_type() == movetype_ladder ||
		g_cs->get_local()->get_move_type() == movetype_noclip ||
		g_cs->get_local()->get_move_type() == movetype_observer)
		return;

	if (cmd->buttons & in_forward || cmd->buttons & in_back ||
		cmd->buttons & in_moveleft || cmd->buttons & in_moveright)
		return;

	const auto velocity = length_2d(g_cs->get_local()->get_velocity());

	if (velocity > 1.0f && !(g_cs->get_local()->get_flags() & fl_onground))
	{
		if (!cmd->mousedx) {
			cmd->forwardmove = std::min<float>(450.0f, 5850.0f / velocity);
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.0f : 450.0f;
		}
		else {
			cmd->sidemove = cmd->mousedx < 0.0f ? -450.0f : 450.0f;
		}
	}
}