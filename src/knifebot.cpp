#include "knifebot.h"

#include "vars.h"
#include "helpers.h"
#include "interfaces.h"

void knifebot::run(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>(V_KNIFEBOT_ENABLED).value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.get_local() || !g_csgo.get_local()->is_life_state())
		return;

	const auto weapon = g_csgo.get_local()->get_active_weapon();

	if (!weapon || !Helpers::is_knife(weapon))
		return;

	if (!g_vars.get_as<bool>(V_KNIFEBOT_FLASH_CHECK).value() && g_csgo.get_local()->is_flashed())
		return;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_csgo.get_local())
			continue;

		if (!g_vars.get_as<bool>(V_KNIFEBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_csgo.get_local()->get_eye_pos(), entity->get_hitbox_position(hitbox_head)))
			return;

		if (!g_vars.get_as<bool>(V_KNIFEBOT_TEAMMATE_CHECK).value() && g_csgo.get_local()->get_team_num() == entity->get_team_num())
			return;

		const auto distance = g_csgo.get_local()->get_eye_pos().distance_to(entity->get_eye_pos());

		if (entity->get_health() <= 51)
		{
			if (distance <= 45.0f)
			{
				cmd->buttons |= in_attack2;
			}
		}
		else
		{
			if (distance <= g_vars.get_as<float>(V_KNIFEBOT_DISTANCE).value())
			{
				cmd->buttons |= in_attack;
			}
		}
	}
}