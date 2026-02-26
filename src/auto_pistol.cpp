#include "aimbot.h"

#include "vars.h"
#include "interfaces.h"
#include "helpers.h"

void auto_pistol_t::think(user_cmd_t* cmd)
{
	if (!g_var->get_as<bool>(V_AIMBOT_AUTOPISTOL).value())
		return;

	if (!(g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game()))
		return;

	const auto weapon = g_cs->get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (!Helpers::is_pistol(weapon))
		return;

	static bool shoot{};
	if (shoot && (cmd->buttons & in_attack))
		cmd->buttons &= ~in_attack;

	shoot = cmd->buttons & in_attack ? true : false;
}