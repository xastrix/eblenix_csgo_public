#include "movement.h"

#include "vars.h"
#include "interfaces.h"
#include "math.h"

#include <algorithm>

void c_move::run(user_cmd_t* cmd)
{
	if (!(g_csgo.m_engine->is_connected() && g_csgo.m_engine->is_in_game()))
		return;

	if (!g_csgo.get_local() || !g_csgo.get_local()->is_life_state())
		return;

	if (g_vars.get_as<bool>(V_MISC_MOVEMENT_BUNNYHOP).value())
		bunny_hop(cmd);

	if (g_vars.get_as<bool>(V_MISC_MOVEMENT_INFINITE_DUCK).value())
		infinite_duck(cmd);

	if (g_vars.get_as<bool>(V_MISC_MOVEMENT_AUTO_STRAFE).value())
		auto_strafe(cmd);

	if (g_vars.get_as<int>(V_KEYS_ON_HOLD_BLOCKBOT).value())
		block_players(cmd);
}

void c_move::bunny_hop(user_cmd_t* cmd)
{
	if (g_csgo.get_local()->get_move_type() == movetype_ladder ||
		g_csgo.get_local()->get_move_type() == movetype_noclip ||
		g_csgo.get_local()->get_move_type() == movetype_observer)
		return;

	static uint8_t u{};
	if (!(u & 0x1) && (u & 0x2)) {
		u &= ~0x2;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (g_csgo.get_local()->get_flags() & fl_onground) {
			u |= 0x1;
			u |= 0x2;
		}
		else {
			cmd->buttons &= ~in_jump;
			u &= ~0x1;
		}
	}
	else {
		u &= ~0x3;
	}
}

void c_move::infinite_duck(user_cmd_t* cmd)
{
	if (g_csgo.get_local()->get_move_type() == movetype_ladder ||
		g_csgo.get_local()->get_move_type() == movetype_noclip ||
		g_csgo.get_local()->get_move_type() == movetype_observer)
		return;

	cmd->buttons |= in_bullrush;
}

void c_move::auto_strafe(user_cmd_t* cmd)
{
	if (g_csgo.get_local()->get_move_type() == movetype_ladder ||
		g_csgo.get_local()->get_move_type() == movetype_noclip ||
		g_csgo.get_local()->get_move_type() == movetype_observer)
		return;

	if (cmd->buttons & in_forward || cmd->buttons & in_back ||
		cmd->buttons & in_moveleft || cmd->buttons & in_moveright)
		return;

	const auto velocity = length_2d(g_csgo.get_local()->get_velocity());

	if (velocity > 1.0f && !(g_csgo.get_local()->get_flags() & fl_onground))
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

void c_move::block_players(user_cmd_t* cmd)
{
	auto best_dist = 255.0f;
	auto ent_index = -1;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++) {
		auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

		if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_csgo.get_local())
			continue;

		/* 
		   I decided that this would be better since we can use blockbot even when the chat and console are open;
		   the only downside is that the key will work outside the game
		*/
		if (!(GetAsyncKeyState(g_vars.get_as<int>(V_KEYS_ON_HOLD_BLOCKBOT).value()) & 0x8000))
			continue;

		const auto dist = g_csgo.get_local()->get_vec_origin().distance_to(entity->get_vec_origin());
		if (dist < best_dist) {
			best_dist = dist;
			ent_index = i;
		}
	}

	auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(ent_index);

	if (!entity)
		return;

	auto local_angle = g_csgo.m_engine->get_view_angles();
	auto vec_forward = entity->get_absolute_origin() - g_csgo.get_local()->get_absolute_origin();
	auto hitbox_pos  = entity->get_hitbox_position(hitbox_upper_chest);
	auto best_speed  = length(entity->get_velocity());

	if (hitbox_pos.z < g_csgo.get_local()->get_vec_origin().z)
	{
		cmd->forwardmove = ((std::sin(DEG2RAD(local_angle.y)) * vec_forward.y) + (std::cos(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;
		cmd->sidemove = ((std::cos(DEG2RAD(local_angle.y)) * -vec_forward.y) + (std::sin(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;
	}
	else
	{
		Math::adjust_sidemove_for_yaw(vec_forward, local_angle, best_speed, cmd);
	}
}