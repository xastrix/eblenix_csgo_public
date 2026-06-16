#include "aimbot.h"

#include "vars.h"
#include "interfaces.h"
#include "helpers.h"
#include "math.h"

void c_aimbot::run(user_cmd_t* cmd)
{
	if (!g_var->get_as<bool>(V_AIMBOT_ENABLED).value())
		return;

	if (!(g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game()))
		return;

	const auto weapon = g_cs->get_local()->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (weapon->item_definition_index() == weapon_glock ||
		weapon->item_definition_index() == weapon_elite ||
		weapon->item_definition_index() == weapon_p250 ||
		weapon->item_definition_index() == weapon_tec9 ||
		weapon->item_definition_index() == weapon_cz75a ||
		weapon->item_definition_index() == weapon_deagle ||
		weapon->item_definition_index() == weapon_usp_silencer ||
		weapon->item_definition_index() == weapon_hkp2000 ||
		weapon->item_definition_index() == weapon_fiveseven)
	{
		if (g_var->get_as<bool>(V_AIMBOT_AUTOPISTOL).value())
		{
			static bool shoot = false;

			if (shoot && (cmd->buttons & in_attack))
				cmd->buttons &= ~in_attack;

			shoot = cmd->buttons & in_attack ? true : false;
		}
	}

	if (Helpers::is_knife(weapon) ||
		Helpers::is_grenade(weapon) ||
		Helpers::is_c4(weapon) ||
		Helpers::is_non_aim(weapon))
		return;

	set_weapon_param(weapon);

	const auto target = Helpers::find_target_entity(cmd, m_fov, m_angle);

	if (!target)
		return;

	auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(target);

	if (!entity || !entity->is_life_state())
		return;

	if (!can_aim(entity, weapon))
		return;

	auto aim_punch = g_cs->get_local()->aim_punch_angle() *
		g_cs->m_cvar->get_convar("weapon_recoil_scale")->get_float();

	aim_punch.x *= m_rcs_x;
	aim_punch.y *= m_rcs_y;

	m_angle = Math::calculate_angle(g_cs->get_local()->get_eye_pos(), m_type == 1 ?
		entity->get_bone_position(Helpers::get_nearest_bone(entity, cmd)) :
		entity->get_hitbox_position(m_hitbox_id), cmd->viewangles + aim_punch);

	clamp(m_angle);

	m_angle /= m_smooth;
	cmd->viewangles += m_angle;

	if (!g_var->get_as<bool>(V_AIMBOT_SILENT).value())
		g_cs->m_engine->set_view_angles(cmd->viewangles);
}

bool c_aimbot::can_aim(c_base_player* entity, c_base_weapon* weapon)
{
	if (g_var->get_as<bool>(V_AIMBOT_VISIBLE_CHECK).value() && !g_cs->get_local()->can_see_entity(entity, entity->get_eye_pos()))
		return false;

	if (!g_var->get_as<bool>(V_AIMBOT_SMOKE_CHECK).value() && Helpers::is_behind_smoke(g_cs->get_local()->get_eye_pos(), entity->get_hitbox_position(hitbox_head)))
		return false;

	if (!g_var->get_as<bool>(V_AIMBOT_FLASH_CHECK).value() && g_cs->get_local()->is_flashed())
		return false;

	if (!g_var->get_as<bool>(V_AIMBOT_TEAMMATE_CHECK).value() && g_cs->get_local()->get_team_num() == entity->get_team_num())
		return false;

	if (!g_var->get_as<bool>(V_AIMBOT_JUMP_CHECK).value() && !(g_cs->get_local()->get_flags() & fl_onground))
		return false;

	if (g_var->get_as<bool>(V_AIMBOT_SCOPE_CHECK).value() && Helpers::is_sniper(weapon) && !g_cs->get_local()->is_scoped())
		return false;

	return true;
}

void c_aimbot::set_weapon_param(c_base_weapon* weapon)
{
	if (Helpers::is_pistol(weapon))
	{
		switch (g_var->get_as<int>(V_AIMBOT_BONE_PISTOL).value()) {
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

		m_fov = g_var->get_as<float>(V_AIMBOT_FOV_PISTOL).value();
		m_smooth = g_var->get_as<float>(V_AIMBOT_SMOOTH_PISTOL).value();

		m_type = g_var->get_as<int>(V_AIMBOT_TYPE_PISTOL).value();

		m_rcs_x = g_var->get_as<float>(V_AIMBOT_RCS_X_PISTOL).value();
		m_rcs_y = g_var->get_as<float>(V_AIMBOT_RCS_Y_PISTOL).value();
	}
	else if (Helpers::is_rifle(weapon))
	{
		switch (g_var->get_as<int>(V_AIMBOT_BONE_RIFLE).value()) {
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

		m_fov = g_var->get_as<float>(V_AIMBOT_FOV_RIFLE).value();
		m_smooth = g_var->get_as<float>(V_AIMBOT_SMOOTH_RIFLE).value();

		m_type = g_var->get_as<int>(V_AIMBOT_TYPE_RIFLE).value();

		m_rcs_x = g_var->get_as<float>(V_AIMBOT_RCS_X_RIFLE).value();
		m_rcs_y = g_var->get_as<float>(V_AIMBOT_RCS_Y_RIFLE).value();
	}
	else if (Helpers::is_sniper(weapon))
	{
		switch (g_var->get_as<int>(V_AIMBOT_BONE_SNIPER).value()) {
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

		m_fov = g_var->get_as<float>(V_AIMBOT_FOV_SNIPER).value();
		m_smooth = g_var->get_as<float>(V_AIMBOT_SMOOTH_SNIPER).value();

		m_type = g_var->get_as<int>(V_AIMBOT_TYPE_SNIPER).value();

		m_rcs_x = g_var->get_as<float>(V_AIMBOT_RCS_X_SNIPER).value();
		m_rcs_y = g_var->get_as<float>(V_AIMBOT_RCS_Y_SNIPER).value();
	}
	else if (Helpers::is_heavy(weapon))
	{
		switch (g_var->get_as<int>(V_AIMBOT_BONE_HEAVY).value()) {
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

		m_fov = g_var->get_as<float>(V_AIMBOT_FOV_HEAVY).value();
		m_smooth = g_var->get_as<float>(V_AIMBOT_SMOOTH_HEAVY).value();

		m_type = g_var->get_as<int>(V_AIMBOT_TYPE_HEAVY).value();

		m_rcs_x = g_var->get_as<float>(V_AIMBOT_RCS_X_HEAVY).value();
		m_rcs_y = g_var->get_as<float>(V_AIMBOT_RCS_Y_HEAVY).value();
	}
	else if (Helpers::is_smg(weapon))
	{
		switch (g_var->get_as<int>(V_AIMBOT_BONE_SMG).value()) {
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

		m_fov = g_var->get_as<float>(V_AIMBOT_FOV_SMG).value();
		m_smooth = g_var->get_as<float>(V_AIMBOT_SMOOTH_SMG).value();

		m_type = g_var->get_as<int>(V_AIMBOT_TYPE_SMG).value();

		m_rcs_x = g_var->get_as<float>(V_AIMBOT_RCS_X_SMG).value();
		m_rcs_y = g_var->get_as<float>(V_AIMBOT_RCS_Y_SMG).value();
	}
}