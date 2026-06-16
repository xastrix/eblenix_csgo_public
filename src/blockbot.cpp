#include "movement.h"

#include "vars.h"
#include "interfaces.h"
#include "math.h"

void blockbot_t::think(user_cmd_t* cmd)
{
	auto best_dist = 255.0f;
	auto ent_index = -1;

	if (g_var->get_as<int>(V_KEYS_ON_HOLD_BLOCKBOT).value() <= 0)
		return;

	for (int i = 1; i <= g_cs->m_globals->max_clients; i++) {
		auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

		if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_cs->get_local())
			continue;

		if (!(GetAsyncKeyState(g_var->get_as<int>(V_KEYS_ON_HOLD_BLOCKBOT).value()) & 0x8000))
			continue;

		const auto dist = g_cs->get_local()->get_vec_origin().distance_to(entity->get_vec_origin());
		if (dist < best_dist) {
			best_dist = dist;
			ent_index = i;
		}
	}

	auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(ent_index);

	if (!entity)
		return;

	auto local_angle = g_cs->m_engine->get_view_angles();
	auto vec_forward = entity->get_absolute_origin() - g_cs->get_local()->get_absolute_origin();
	auto hitbox_pos = entity->get_hitbox_position(hitbox_upper_chest);
	auto best_speed = length(entity->get_velocity());

	if (hitbox_pos.z < g_cs->get_local()->get_vec_origin().z)
	{
		cmd->forwardmove = ((std::sin(DEG2RAD(local_angle.y)) * vec_forward.y) +
			(std::cos(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;

		cmd->sidemove = ((std::cos(DEG2RAD(local_angle.y)) * -vec_forward.y) +
			(std::sin(DEG2RAD(local_angle.y)) * vec_forward.x)) * best_speed;
	}
	else
	{
		Math::adjust_sidemove_for_yaw(vec_forward, local_angle, best_speed, cmd);
	}
}