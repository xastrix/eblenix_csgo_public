#include "triggerbot.h"

#include "vars.h"
#include "helpers.h"
#include "interfaces.h"
#include "math.h"

void triggerbot::run(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>(V_TRIGGERBOT_ENABLED).value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.get_local() || !g_csgo.get_local()->is_life_state())
		return;

	const auto weapon = g_csgo.get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (Helpers::is_knife(weapon) || Helpers::is_grenade(weapon))
		return;

	if (Helpers::is_weapon_switching(weapon))
		return;

	const auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	vec3 src, dst, forward, crosshair_forward;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	Math::angle_vectors(cmd->viewangles, forward);
	forward *= weapon_data->weapon_range;

	filter.skip = g_csgo.get_local();

	src = g_csgo.get_local()->get_eye_pos();
	dst = src + forward;

	ray.init(src, dst);
	g_csgo.m_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

	if (tr.did_hit_world() || !tr.did_hit_non_world_entity())
		return;

	const auto entity = tr.entity;

	if (!entity)
		return;

	if (!g_vars.get_as<bool>(V_TRIGGERBOT_FLASH_CHECK).value() && g_csgo.get_local()->is_flashed())
		return;

	if (!g_vars.get_as<bool>(V_TRIGGERBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_csgo.get_local()->get_eye_pos(), tr.end))
		return;

	if (!g_vars.get_as<bool>(V_TRIGGERBOT_TEAMMATE_CHECK).value() && g_csgo.get_local()->get_team_num() == entity->get_team_num())
		return;

	if (!g_vars.get_as<bool>(V_TRIGGERBOT_JUMP_CHECK).value() && !(g_csgo.get_local()->get_flags() & fl_onground))
		return;

	if (g_vars.get_as<bool>(V_TRIGGERBOT_SCOPE_CHECK).value() && Helpers::is_sniper(weapon) && !g_csgo.get_local()->is_scoped())
		return;

	if (entity->get_client_class()->class_id != ccsplayer)
		return;

	if (entity == g_csgo.get_local() || entity->get_dormant() || !entity->is_life_state() || entity->has_gun_game_immunity())
		return;

	if (!Helpers::is_taser(weapon))
	{
		if (g_vars.get_as<bool>(V_TRIGGERBOT_HITBOX_HEAD).value())
		{
			if (tr.hitgroup == hitgroup_head)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>(V_TRIGGERBOT_HITBOX_BODY).value())
		{
			if (tr.hitgroup == hitgroup_chest || tr.hitgroup == hitgroup_stomach)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>(V_TRIGGERBOT_HITBOX_ARMS).value())
		{
			if (tr.hitgroup == hitgroup_leftarm || tr.hitgroup == hitgroup_rightarm)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_vars.get_as<bool>(V_TRIGGERBOT_HITBOX_LEGS).value())
		{
			if (tr.hitgroup == hitgroup_leftleg || tr.hitgroup == hitgroup_rightleg)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}
	}
}