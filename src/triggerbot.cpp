#include "aimbot.h"

#include "vars.h"
#include "interfaces.h"
#include "helpers.h"
#include "math.h"

void triggerbot_t::think(user_cmd_t* cmd)
{
	vec3         src, dst, forward, crosshair_forward;
	trace_t      tr;
	ray_t        ray;
	trace_filter filter;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_ENABLED).value())
		return;

	if (!(g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game()))
		return;

	if (!g_cs->get_local() || !g_cs->get_local()->is_life_state())
		return;

	const auto weapon = g_cs->get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (Helpers::is_knife(weapon) || Helpers::is_grenade(weapon))
		return;

	const auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	Math::angle_vectors(cmd->viewangles, forward);
	forward *= weapon_data->weapon_range;

	filter.e = g_cs->get_local();

	src = g_cs->get_local()->get_eye_pos();
	dst = src + forward;

	ray.init(src, dst);
	g_cs->m_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

	if (tr.did_hit_world() || !tr.did_hit_non_world_entity())
		return;

	const auto entity = tr.entity;

	if (!entity)
		return;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_FLASH_CHECK).value() && g_cs->get_local()->is_flashed())
		return;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_cs->get_local()->get_eye_pos(), tr.end))
		return;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_TEAMMATE_CHECK).value() && g_cs->get_local()->get_team_num() == entity->get_team_num())
		return;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_JUMP_CHECK).value() && !(g_cs->get_local()->get_flags() & fl_onground))
		return;

	if (g_var->get_as<bool>(V_TRIGGERBOT_SCOPE_CHECK).value() && Helpers::is_sniper(weapon) && !g_cs->get_local()->is_scoped())
		return;

	if (entity->get_client_class()->class_id != ccsplayer)
		return;

	if (entity == g_cs->get_local() || entity->get_dormant() || !entity->is_life_state() || entity->has_gun_game_immunity())
		return;

	if (!Helpers::is_taser(weapon))
	{
		if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_HEAD).value())
		{
			if (tr.hitgroup == hitgroup_head)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_BODY).value())
		{
			if (tr.hitgroup == hitgroup_chest || tr.hitgroup == hitgroup_stomach)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_ARMS).value())
		{
			if (tr.hitgroup == hitgroup_leftarm || tr.hitgroup == hitgroup_rightarm)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}

		if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_LEGS).value())
		{
			if (tr.hitgroup == hitgroup_leftleg || tr.hitgroup == hitgroup_rightleg)
			{
				Helpers::is_revolver(weapon) ?
					cmd->buttons |= in_attack2 : cmd->buttons |= in_attack;
			}
		}
	}
}