#include "aimbot.h"

#include "../../var_manager/var_manager.h"
#include "../../helpers/helpers.h"

#include "../../../csgo/interfaces/interfaces.h"
#include "../../../csgo/math/math.h"

aimbot g_aimbot;

void aimbot::instance(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>("aimbot->enabled").value())
		return;

	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.m_local || !g_csgo.m_local->is_life_state())
		return;

	const auto weapon = g_csgo.m_local->get_active_weapon();

	if (!weapon || !weapon->clip1_count())
		return;

	if (g_vars.get_as<bool>("aimbot->autopistol").value()) {
		if (g_helpers.is_pistol(weapon) && !g_helpers.is_revolver(weapon)) {
			static bool shoot{};

			if (cmd->buttons & in_attack) {
				if (shoot)
					cmd->buttons &= ~in_attack;
			}

			shoot = cmd->buttons & in_attack ? true : false;
		}
	}

	if (g_helpers.is_knife(weapon) || g_helpers.is_grenade(weapon) ||
		g_helpers.is_c4(weapon) || g_helpers.is_non_aim(weapon))
		return;

	if (g_helpers.is_weapon_switching(weapon))
		return;
	
	m_state = static_cast<_aim_state>(g_vars.get_as<bool>("aimbot->all").value());
	set_weapon_param(weapon);

	if (const auto target = g_helpers.find_target_entity(cmd, m_aim_fov, m_angle))
	{
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(target));

		if (!entity || !entity->is_life_state())
			return;

		if (!can_shot(entity, weapon))
			return;

		const auto aim_punch = g_csgo.m_local->aim_punch_angle() *
			g_csgo.m_cvar->get_convar("weapon_recoil_scale")->get_float();

		set_aim_angle(cmd, entity, aim_punch);

		m_angle /= m_aim_smooth;
		cmd->viewangles += m_angle;

		if (!g_vars.get_as<bool>("aimbot->silent").value())
			g_csgo.m_engine->set_view_angles(cmd->viewangles);
	}
}

void aimbot::set_weapon_param(c_base_weapon* weapon)
{
	if (g_helpers.is_pistol(weapon))
	{
		switch (m_state) {
		case as_none: {
			switch (g_vars.get_as<int>("aimbot->bone->pistol").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->fov->pistol").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->smooth->pistol").value();
			m_aim_type = g_vars.get_as<int>("aimbot->type->pistol").value();

			break;
		}
		case as_all_weapons: {
			switch (g_vars.get_as<int>("aimbot->all->bone").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->all->fov").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->all->smooth").value();
			m_aim_type = g_vars.get_as<int>("aimbot->all->type").value();

			break;
		}
		}
	}
	else if (g_helpers.is_rifle(weapon))
	{
		switch (m_state) {
		case as_none: {
			switch (g_vars.get_as<int>("aimbot->bone->rifle").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->fov->rifle").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->smooth->rifle").value();
			m_aim_type = g_vars.get_as<int>("aimbot->type->rifle").value();

			break;
		}
		case as_all_weapons: {
			switch (g_vars.get_as<int>("aimbot->all->bone").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->all->fov").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->all->smooth").value();
			m_aim_type = g_vars.get_as<int>("aimbot->all->type").value();

			break;
		}
		}
	}
	else if (g_helpers.is_sniper(weapon))
	{
		switch (m_state) {
		case as_none: {
			switch (g_vars.get_as<int>("aimbot->bone->sniper").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->fov->sniper").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->smooth->sniper").value();
			m_aim_type = g_vars.get_as<int>("aimbot->type->sniper").value();

			break;
		}
		case as_all_weapons: {
			switch (g_vars.get_as<int>("aimbot->all->bone").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->all->fov").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->all->smooth").value();
			m_aim_type = g_vars.get_as<int>("aimbot->all->type").value();

			break;
		}
		}
	}
	else if (g_helpers.is_heavy(weapon))
	{
		switch (m_state) {
		case as_none: {
			switch (g_vars.get_as<int>("aimbot->bone->heavy").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->fov->heavy").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->smooth->heavy").value();
			m_aim_type = g_vars.get_as<int>("aimbot->type->heavy").value();

			break;
		}
		case as_all_weapons: {
			switch (g_vars.get_as<int>("aimbot->all->bone").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->all->fov").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->all->smooth").value();
			m_aim_type = g_vars.get_as<int>("aimbot->all->type").value();

			break;
		}
		}
	}
	else if (g_helpers.is_smg(weapon))
	{
		switch (m_state) {
		case as_none: {
			switch (g_vars.get_as<int>("aimbot->bone->smg").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->fov->smg").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->smooth->smg").value();
			m_aim_type = g_vars.get_as<int>("aimbot->type->smg").value();

			break;
		}
		case as_all_weapons: {
			switch (g_vars.get_as<int>("aimbot->all->bone").value()) {
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

			m_aim_fov = g_vars.get_as<float>("aimbot->all->fov").value();
			m_aim_smooth = g_vars.get_as<float>("aimbot->all->smooth").value();
			m_aim_type = g_vars.get_as<int>("aimbot->all->type").value();

			break;
		}
		}
	}
}

bool aimbot::can_shot(c_base_player* entity, c_base_weapon* weapon)
{
	if (g_vars.get_as<bool>("aimbot->visible_check").value() && !g_csgo.m_local->can_see_entity(entity->get_eye_pos()))
		return false;

	if (!g_vars.get_as<bool>("aimbot->smoke_check").value() && g_helpers.is_behind_smoke(g_csgo.m_local->get_eye_pos(), entity->get_hitbox_position(hitbox_head)))
		return false;

	if (!g_vars.get_as<bool>("aimbot->flash_check").value() && g_csgo.m_local->is_flashed())
		return false;

	if (!g_vars.get_as<bool>("aimbot->teammate_check").value() && g_csgo.m_local->get_team_num() == entity->get_team_num())
		return false;

	if (!g_vars.get_as<bool>("aimbot->jump_check").value() && g_csgo.m_local->is_in_air())
		return false;

	if (g_vars.get_as<bool>("aimbot->scope_check").value() && g_helpers.is_sniper(weapon) && !g_csgo.m_local->is_scoped())
		return false;

	return true;
}

void aimbot::set_aim_angle(i_user_cmd* cmd, c_base_player* entity, const vec3& aim_punch)
{
	switch (m_aim_type) {
	case 0: {
		m_angle = g_math.calculate_angle(g_csgo.m_local->get_eye_pos(),
			entity->get_hitbox_position(m_hitbox_id), cmd->viewangles + aim_punch);
		break;
	}
	case 1: {
		m_angle = g_math.calculate_angle(g_csgo.m_local->get_eye_pos(),
			entity->get_bone_position(g_helpers.get_nearest_bone(entity, cmd)), cmd->viewangles + aim_punch);
		break;
	}
	}
}