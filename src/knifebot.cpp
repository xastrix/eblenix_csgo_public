#include "knifebot.h"

#include "var_manager.h"
#include "helpers.h"
#include "interfaces.h"

knifebot g_knifebot;

void knifebot::instance(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>("knifebot->enabled").value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.m_local || !g_csgo.m_local->is_life_state())
		return;

	const auto weapon = g_csgo.m_local->get_active_weapon();

	if (!weapon || !Helpers::is_knife(weapon))
		return;

	if (!g_vars.get_as<bool>("knifebot->flash_check").value() && g_csgo.m_local->is_flashed())
		return;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
	{
		auto entity = (c_base_player*)g_csgo.m_entity_list->get_client_entity(i);

		if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_csgo.m_local)
			continue;

		if (!g_vars.get_as<bool>("knifebot->smoke_check").value() && Helpers::is_behind_smoke(g_csgo.m_local->get_eye_pos(), entity->get_hitbox_position(hitbox_head)))
			return;

		if (!g_vars.get_as<bool>("knifebot->teammate_check").value() && g_csgo.m_local->get_team_num() == entity->get_team_num())
			return;

		const auto distance = g_csgo.m_local->get_eye_pos().distance_to(entity->get_eye_pos());

		if (entity->get_health() <= 51)
		{
			if (distance <= 45.0f)
			{
				cmd->buttons |= in_attack2;
			}
		}
		else
		{
			if (distance <= g_vars.get_as<float>("knifebot->distance").value())
			{
				cmd->buttons |= in_attack;
			}
		}
	}
}