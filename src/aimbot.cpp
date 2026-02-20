#include "aimbot.h"

#include "vars.h"
#include "helpers.h"
#include "interfaces.h"
#include "math.h"

void c_aimbot::run(user_cmd_t* cmd)
{
	if (!g_vars.get_as<bool>(V_AIMBOT_ENABLED).value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.get_local() || !g_csgo.get_local()->is_life_state())
		return;

	const auto weapon = g_csgo.get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (Helpers::is_knife(weapon) || Helpers::is_grenade(weapon) ||
		Helpers::is_c4(weapon) || Helpers::is_non_aim(weapon))
		return;

	if (g_vars.get_as<bool>(V_AIMBOT_AUTOPISTOL).value())
		do_autopistol(cmd, weapon);
	
	set_weapon_param(weapon);

	if (const auto target = Helpers::find_target_entity(cmd, m_aim_fov, m_angle))
	{
		auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(target);

		if (!entity || !entity->is_life_state())
			return;

		if (!can_aiming(entity, weapon))
			return;

		set_aim_angle(cmd, entity, g_csgo.get_local()->aim_punch_angle());

		m_angle.normalize();
		clamp(m_angle);

		m_angle /= m_aim_smooth;
		cmd->viewangles += m_angle;

		if (!g_vars.get_as<bool>(V_AIMBOT_SILENT).value())
			g_csgo.m_engine->set_view_angles(cmd->viewangles);
	}
}

void c_aimbot::do_triggerbot(user_cmd_t* cmd)
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

	const auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	vec3 src, dst, forward, crosshair_forward;
	trace_t tr;
	ray_t ray;
	trace_filter filter;

	Math::angle_vectors(cmd->viewangles, forward);
	forward *= weapon_data->weapon_range;

	filter.e = g_csgo.get_local();

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

void c_aimbot::do_knifebot(user_cmd_t* cmd)
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
		auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

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

void c_aimbot::set_weapon_param(c_base_weapon* weapon)
{
	if (Helpers::is_pistol(weapon))
	{
		switch (g_vars.get_as<int>(V_AIMBOT_BONE_PISTOL).value()) {
		case 0: {
			m_hitbox_id = hitbox_head;
			break;
		}
		case 1: {
			m_hitbox_id = hitbox_neck;
			break;
		}
		case 2: {
			m_hitbox_id = hitbox_chest;
			break;
		}
		case 3: {
			m_hitbox_id = hitbox_stomach;
			break;
		}
		case 4: {
			m_hitbox_id = hitbox_pelvis;
			break;
		}
		}

		m_aim_fov = g_vars.get_as<float>(V_AIMBOT_FOV_PISTOL).value();
		m_aim_smooth = g_vars.get_as<float>(V_AIMBOT_SMOOTH_PISTOL).value();
		m_aim_type = g_vars.get_as<int>(V_AIMBOT_TYPE_PISTOL).value();
	}
	else if (Helpers::is_rifle(weapon))
	{
		switch (g_vars.get_as<int>(V_AIMBOT_BONE_RIFLE).value()) {
		case 0: {
			m_hitbox_id = hitbox_head;
			break;
		}
		case 1: {
			m_hitbox_id = hitbox_neck;
			break;
		}
		case 2: {
			m_hitbox_id = hitbox_chest;
			break;
		}
		case 3: {
			m_hitbox_id = hitbox_stomach;
			break;
		}
		case 4: {
			m_hitbox_id = hitbox_pelvis;
			break;
		}
		}

		m_aim_fov = g_vars.get_as<float>(V_AIMBOT_FOV_RIFLE).value();
		m_aim_smooth = g_vars.get_as<float>(V_AIMBOT_SMOOTH_RIFLE).value();
		m_aim_type = g_vars.get_as<int>(V_AIMBOT_TYPE_RIFLE).value();
	}
	else if (Helpers::is_sniper(weapon))
	{
		switch (g_vars.get_as<int>(V_AIMBOT_BONE_SNIPER).value()) {
		case 0: {
			m_hitbox_id = hitbox_head;
			break;
		}
		case 1: {
			m_hitbox_id = hitbox_neck;
			break;
		}
		case 2: {
			m_hitbox_id = hitbox_chest;
			break;
		}
		case 3: {
			m_hitbox_id = hitbox_stomach;
			break;
		}
		case 4: {
			m_hitbox_id = hitbox_pelvis;
			break;
		}
		}

		m_aim_fov = g_vars.get_as<float>(V_AIMBOT_FOV_SNIPER).value();
		m_aim_smooth = g_vars.get_as<float>(V_AIMBOT_SMOOTH_SNIPER).value();
		m_aim_type = g_vars.get_as<int>(V_AIMBOT_TYPE_SNIPER).value();
	}
	else if (Helpers::is_heavy(weapon))
	{
		switch (g_vars.get_as<int>(V_AIMBOT_BONE_HEAVY).value()) {
		case 0: {
			m_hitbox_id = hitbox_head;
			break;
		}
		case 1: {
			m_hitbox_id = hitbox_neck;
			break;
		}
		case 2: {
			m_hitbox_id = hitbox_chest;
			break;
		}
		case 3: {
			m_hitbox_id = hitbox_stomach;
			break;
		}
		case 4: {
			m_hitbox_id = hitbox_pelvis;
			break;
		}
		}

		m_aim_fov = g_vars.get_as<float>(V_AIMBOT_FOV_HEAVY).value();
		m_aim_smooth = g_vars.get_as<float>(V_AIMBOT_SMOOTH_HEAVY).value();
		m_aim_type = g_vars.get_as<int>(V_AIMBOT_TYPE_HEAVY).value();
	}
	else if (Helpers::is_smg(weapon))
	{
		switch (g_vars.get_as<int>(V_AIMBOT_BONE_SMG).value()) {
		case 0: {
			m_hitbox_id = hitbox_head;
			break;
		}
		case 1: {
			m_hitbox_id = hitbox_neck;
			break;
		}
		case 2: {
			m_hitbox_id = hitbox_chest;
			break;
		}
		case 3: {
			m_hitbox_id = hitbox_stomach;
			break;
		}
		case 4: {
			m_hitbox_id = hitbox_pelvis;
			break;
		}
		}

		m_aim_fov = g_vars.get_as<float>(V_AIMBOT_FOV_SMG).value();
		m_aim_smooth = g_vars.get_as<float>(V_AIMBOT_SMOOTH_SMG).value();
		m_aim_type = g_vars.get_as<int>(V_AIMBOT_TYPE_SMG).value();
	}
}

bool c_aimbot::can_aiming(c_base_player* entity, c_base_weapon* weapon)
{
	if (g_vars.get_as<bool>(V_AIMBOT_VISIBLE_CHECK).value() && !g_csgo.get_local()->can_see_entity(entity->get_eye_pos()))
		return false;

	if (!g_vars.get_as<bool>(V_AIMBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_csgo.get_local()->get_eye_pos(), entity->get_hitbox_position(hitbox_head)))
		return false;

	if (!g_vars.get_as<bool>(V_AIMBOT_FLASH_CHECK).value() && g_csgo.get_local()->is_flashed())
		return false;

	if (!g_vars.get_as<bool>(V_AIMBOT_TEAMMATE_CHECK).value() && g_csgo.get_local()->get_team_num() == entity->get_team_num())
		return false;

	if (!g_vars.get_as<bool>(V_AIMBOT_JUMP_CHECK).value() && !(g_csgo.get_local()->get_flags() & fl_onground))
		return false;

	if (g_vars.get_as<bool>(V_AIMBOT_SCOPE_CHECK).value() && Helpers::is_sniper(weapon) && !g_csgo.get_local()->is_scoped())
		return false;

	return true;
}

void c_aimbot::set_aim_angle(user_cmd_t* cmd, c_base_player* entity, const vec3& aim_punch)
{
	switch (m_aim_type) {
	case 0: {
		m_angle = Math::calculate_angle(g_csgo.get_local()->get_eye_pos(),
			entity->get_hitbox_position(m_hitbox_id), cmd->viewangles + aim_punch);
		break;
	}
	case 1: {
		m_angle = Math::calculate_angle(g_csgo.get_local()->get_eye_pos(),
			entity->get_bone_position(Helpers::get_nearest_bone(entity, cmd)), cmd->viewangles + aim_punch);
		break;
	}
	}
}

void c_aimbot::do_autopistol(user_cmd_t* cmd, c_base_weapon* weapon)
{
	if (!Helpers::is_pistol(weapon) || Helpers::is_revolver(weapon))
		return;

	static bool shoot{};
	if (cmd->buttons & in_attack) {
		if (shoot) cmd->buttons &= ~in_attack;
	}

	shoot = cmd->buttons & in_attack ? true : false;
}