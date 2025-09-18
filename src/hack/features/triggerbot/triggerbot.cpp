#include "triggerbot.h"

#include "../../var_manager/var_manager.h"
#include "../../helpers/helpers.h"

#include "../../../csgo/interfaces/interfaces.h"
#include "../../../csgo/math/math.h"

triggerbot g_triggerbot;

void triggerbot::instance(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>("triggerbot->enabled").value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.m_local || !g_csgo.m_local->is_life_state())
		return;

	const auto weapon = g_csgo.m_local->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (g_helpers.is_knife(weapon) || g_helpers.is_grenade(weapon))
		return;

	if (g_helpers.is_weapon_switching(weapon))
		return;

	const auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	vec3 src, dst, forward, crosshair_forward;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	g_math.angle_vectors(cmd->viewangles, forward);
	forward *= weapon_data->weapon_range;

	filter.skip = g_csgo.m_local;

	src = g_csgo.m_local->get_eye_pos();
	dst = src + forward;

	ray.initialize(src, dst);
	g_csgo.m_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

	if (tr.did_hit_world() || !tr.did_hit_non_world_entity())
		return;

	const auto entity = tr.entity;

	if (!entity)
		return;

	if (!g_vars.get_as<bool>("triggerbot->flash_check").value() && g_csgo.m_local->is_flashed())
		return;

	if (!g_vars.get_as<bool>("triggerbot->smoke_check").value() && g_helpers.is_behind_smoke(g_csgo.m_local->get_eye_pos(), tr.end))
		return;

	if (!g_vars.get_as<bool>("triggerbot->teammate_check").value() && g_csgo.m_local->get_team_num() == entity->get_team_num())
		return;

	if (!g_vars.get_as<bool>("triggerbot->jump_check").value() && g_csgo.m_local->is_in_air())
		return;

	if (g_vars.get_as<bool>("triggerbot->scope_check").value() && g_helpers.is_sniper(weapon) && !g_csgo.m_local->is_scoped())
		return;

	if (entity->get_client_class()->class_id != ccsplayer)
		return;

	if (entity == g_csgo.m_local || entity->get_dormant() || !entity->is_alive() || entity->has_gun_game_immunity())
		return;

	if (!g_helpers.is_taser(weapon))
	{
		if (g_vars.get_as<bool>("triggerbot->hitbox->head").value())
		{
			if (tr.hitgroup == hitgroup_head)
			{
				g_helpers.is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>("triggerbot->hitbox->body").value())
		{
			if (tr.hitgroup == hitgroup_chest || tr.hitgroup == hitgroup_stomach)
			{
				g_helpers.is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>("triggerbot->hitbox->arms").value())
		{
			if (tr.hitgroup == hitgroup_leftarm || tr.hitgroup == hitgroup_rightarm)
			{
				g_helpers.is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>("triggerbot->hitbox->legs").value())
		{
			if (tr.hitgroup == hitgroup_leftleg || tr.hitgroup == hitgroup_rightleg)
			{
				g_helpers.is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}
	}
}