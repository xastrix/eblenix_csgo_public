#include "triggerbot.h"

#include "vars.h"
#include "interfaces.h"
#include "helpers.h"
#include "math.h"

void c_triggerbot::run(user_cmd_t* cmd)
{
	vec3         src, dst;
	trace_t      tr;
	ray_t        ray;
	trace_filter filter;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_ENABLED).value())
		return;

	if (!(g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game()))
		return;

	if (!g_cs->get_local() || !g_cs->get_local()->is_life_state())
		return;

	calc_trace_to_players(cmd, &filter, src, dst, ray, &tr);

	if (!tr.entity || tr.entity->get_client_class()->class_id != ccsplayer)
		return;

	if (tr.entity == g_cs->get_local() || tr.entity->get_dormant() ||
		!tr.entity->is_life_state() || tr.entity->has_gun_game_immunity())
		return;

	const auto weapon = g_cs->get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (can_shoot(weapon, tr))
		shoot(cmd, weapon, tr);
}

void c_triggerbot::clip_trace_to_players(const vec3& start, const vec3& end, uint32_t mask, trace_t* old_trace, c_base_entity* ent) const
{
	float range, range_along;

	auto entity_center      = vec3(((ent->mins() + ent->maxs()) * 0.5f) + ent->get_absolute_origin());
	auto to_entity          = vec3(entity_center - start);
	auto dir                = vec3(end - start);

	dir.normalize();
	range_along = dot(dir, to_entity);

	if (range_along < 0.0f)
		range = -length(to_entity);

	else if (range_along > 0.0f)
		range = -length(entity_center - end);

	else
		range = length(entity_center - (dir * range_along + start));

	if (range <= 60.0f)
	{
		trace_t tr;
		g_cs->m_trace->clip_ray_to_entity(ray_t(start, end), mask, ent, &tr);

		if (old_trace->fraction > tr.fraction)
			*old_trace = tr;
	}
}

void c_triggerbot::calc_trace_to_players(user_cmd_t* cmd, trace_filter* filter, vec3& src, vec3& dst, ray_t& ray, trace_t* trace)
{
	constexpr uint32_t mask = CONTENTS_SOLID | CONTENTS_GRATE | CONTENTS_HITBOX;

	filter->fp = g_cs->get_local();

	src = g_cs->get_local()->get_eye_pos();
	Math::angle_vectors(cmd->viewangles, dst);

	ray.init(src, (dst * 8192.0f) + src);

	g_cs->m_trace->trace_ray(ray, mask, filter, trace);
	clip_trace_to_players(src, dst, mask, trace, g_cs->get_local());
}

bool c_triggerbot::can_shoot(c_base_weapon* weapon, trace_t trace)
{
	if (Helpers::is_knife(weapon) || Helpers::is_grenade(weapon) || Helpers::is_non_aim(weapon))
		return false;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_FLASH_CHECK).value() && g_cs->get_local()->is_flashed())
		return false;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_cs->get_local()->get_eye_pos(), trace.end))
		return false;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_TEAMMATE_CHECK).value() && (g_cs->get_local()->get_team_num() == trace.entity->get_team_num()))
		return false;

	if (!g_var->get_as<bool>(V_TRIGGERBOT_JUMP_CHECK).value() && !(g_cs->get_local()->get_flags() & fl_onground))
		return false;

	if (g_var->get_as<bool>(V_TRIGGERBOT_SCOPE_CHECK).value() && (Helpers::is_sniper(weapon) && !g_cs->get_local()->is_scoped()))
		return false;

	return true;
}

void c_triggerbot::shoot(user_cmd_t* cmd, c_base_weapon* weapon, trace_t trace)
{
	if (Helpers::is_taser(weapon))
		return;

	if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_HEAD).value())
	{
		if (trace.hitgroup == hitgroup_head)
		{
			cmd->buttons |= Helpers::is_revolver(weapon) ? in_attack2 : in_attack;
		}
	}

	if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_BODY).value())
	{
		if (trace.hitgroup == hitgroup_chest || trace.hitgroup == hitgroup_stomach)
		{
			cmd->buttons |= Helpers::is_revolver(weapon) ? in_attack2 : in_attack;
		}
	}

	if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_ARMS).value())
	{
		if (trace.hitgroup == hitgroup_leftarm || trace.hitgroup == hitgroup_rightarm)
		{
			cmd->buttons |= Helpers::is_revolver(weapon) ? in_attack2 : in_attack;
		}
	}

	if (g_var->get_as<bool>(V_TRIGGERBOT_HITBOX_LEGS).value())
	{
		if (trace.hitgroup == hitgroup_leftleg || trace.hitgroup == hitgroup_rightleg)
		{
			cmd->buttons |= Helpers::is_revolver(weapon) ? in_attack2 : in_attack;
		}
	}

	if (Helpers::is_pistol(weapon) && cmd->buttons & in_attack)
		cmd->command_number % 2 == 1 ? cmd->buttons |= in_attack : cmd->buttons &= ~in_attack;
}