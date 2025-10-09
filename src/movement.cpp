#include "movement.h"

#include "var_manager.h"
#include "input_manager.h"
#include "interfaces.h"
#include "math.h"

movement g_movement;

void movement::instance(i_user_cmd* cmd)
{
	if (!g_csgo.m_engine->is_playing())
		return;

	if (!g_csgo.m_local || !g_csgo.m_local->is_life_state())
		return;

	if (g_vars.get_as<bool>("misc->movement->bunnyhop").value())
		bunny_hop(cmd);

	if (g_vars.get_as<bool>("misc->movement->infinite_duck").value())
		infinite_duck(cmd);

	if (g_vars.get_as<bool>("misc->movement->auto_strafe").value())
		auto_strafe(cmd);

	if (g_vars.get_as<bool>("misc->movement->blockbot").value())
		block_players(cmd);
}

void movement::bunny_hop(i_user_cmd* cmd)
{
	if (g_csgo.m_local->get_move_type() == movetype_ladder ||
		g_csgo.m_local->get_move_type() == movetype_noclip ||
		g_csgo.m_local->get_move_type() == movetype_observer)
		return;

	if (cmd->buttons & in_jump && g_csgo.m_local->is_in_air())
		cmd->buttons &= ~in_jump;
}

void movement::infinite_duck(i_user_cmd* cmd)
{
	if (g_csgo.m_local->get_move_type() == movetype_ladder ||
		g_csgo.m_local->get_move_type() == movetype_noclip ||
		g_csgo.m_local->get_move_type() == movetype_observer)
		return;

	cmd->buttons |= in_bullrush;
}

void movement::auto_strafe(i_user_cmd* cmd)
{
	if (g_csgo.m_local->get_move_type() == movetype_ladder ||
		g_csgo.m_local->get_move_type() == movetype_noclip ||
		g_csgo.m_local->get_move_type() == movetype_observer)
		return;

	if (cmd->buttons & in_forward || cmd->buttons & in_back ||
		cmd->buttons & in_moveleft || cmd->buttons & in_moveright)
		return;

	const auto velocity = g_csgo.m_local->get_velocity().length_2d();

	if (velocity > 1.0f && g_csgo.m_local->is_in_air())
	{
		if (!cmd->mousedx) {
			cmd->forwardmove = std::min<float>(450.0f, 5850.0f / velocity);
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.0f : 450.0f;
		}
		else {
			cmd->sidemove = cmd->mousedx < 0.0f ? -450.0f : 450.0f;
		}
	}
}

void movement::block_players(i_user_cmd* cmd)
{
	auto best_dist = 255.0f;
	auto ent_index = -1;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++) {
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_csgo.m_local)
			continue;

		const auto dist = g_csgo.m_local->get_vec_origin().distance_to(entity->get_vec_origin());
		if (dist < best_dist) {
			best_dist = dist;
			ent_index = i;
		}
	}

	auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(ent_index));

	if (!entity)
		return;

	auto local_angle = g_csgo.m_engine->get_view_angles();
	auto vec_forward = entity->get_absolute_origin() - g_csgo.m_local->get_absolute_origin();
	auto hitbox_pos  = entity->get_hitbox_position(hitbox_upper_chest);
	auto best_speed  = entity->get_velocity().length();

	if (g_csgo.m_local->is_in_air())
		return;

	if (hitbox_pos.z < g_csgo.m_local->get_vec_origin().z)
	{
		cmd->forwardmove = ((std::sin(DEG2RAD(local_angle.y)) * vec_forward.y) + (std::cos(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;
		cmd->sidemove = ((std::cos(DEG2RAD(local_angle.y)) * -vec_forward.y) + (std::sin(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;
	}
	else
	{
		auto yaw_delta = (std::atan2(vec_forward.y, vec_forward.x) * 180.0f / M_PI) - local_angle.y;

		if (yaw_delta > 180) {
			yaw_delta -= 360;
		}
		else if (yaw_delta < -180) {
			yaw_delta += 360;
		}

		if (yaw_delta > 0.25) {
			cmd->sidemove = -best_speed;
		}
		else if (yaw_delta < -0.25) {
			cmd->sidemove = best_speed;
		}
	}
}