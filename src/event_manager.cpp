#include "event_manager.h"

#include "vars.h"
#include "helpers.h"
#include "render_manager.h"
#include "esp.h"
#include "interfaces.h"
#include "fnv.h"

#include <sstream>

void event_manager::init()
{
	g_csgo.m_event_manager->add_listener(this, "player_hurt");
	g_csgo.m_event_manager->add_listener(this, "item_purchase");
	g_csgo.m_event_manager->add_listener(this, "round_start");
}

void event_manager::fire_game_event(c_game_event* event)
{
	switch (fnv::hash(event->get_name())) {
	case fnv::hash("player_hurt"): {
		if (g_vars.get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_HURT).value()) {
			auto attacker_ent_id = g_csgo.m_entity_list->get_client_entity(
				g_csgo.m_engine->get_player_for_user_id(event->get_int("attacker"))
			);

			if (attacker_ent_id == g_csgo.get_local())
			{
				auto ent_index = g_csgo.m_engine->get_player_for_user_id(event->get_int("userid"));
				auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(ent_index));

				if (entity)
				{
					auto hitbox = event->get_int("hitgroup");

					if (hitbox)
					{
						auto damage = event->get_int("dmg_health");

						if (damage)
						{
							auto health = event->get_int("health");

							if (health >= 0)
							{
								player_info_t info{};
								g_csgo.m_engine->get_player_info(entity->index(), &info);

								std::string player_name{ info.m_player_name };

								std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

								g_csgo.m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[Eblenix] ");

								std::ostringstream oss{};
								oss << "hit " << player_name << " in the " << Helpers::hitgroup_name(hitbox) << " for " << damage << " (" << health << " health remains)" << std::endl;

								g_csgo.m_cvar->console_printf(oss.str().c_str());
							}
						}
					}
				}
			}
		}
		break;
	}
	case fnv::hash("item_purchase"): {
		if (g_vars.get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_PURCHASE).value()) {
			auto ent_index = g_csgo.m_engine->get_player_for_user_id(event->get_int("userid"));

			if (ent_index)
			{
				auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(ent_index));

				if (entity->get_team_num() != g_csgo.get_local()->get_team_num())
				{
					player_info_t info{};
					g_csgo.m_engine->get_player_info(ent_index, &info);

					std::string player_name{ info.m_player_name };

					std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

					const auto weapon = event->get_string("weapon");

					if (weapon != "weapon_unknown")
					{
						g_csgo.m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[Eblenix] ");

						std::ostringstream oss{};
						oss << "A " << player_name << " purchased " << weapon << std::endl;

						g_csgo.m_cvar->console_printf(oss.str().c_str());
					}
				}
			}
		}
		break;
	}
	case fnv::hash("round_start"): {
		g_esp.on_round_start();
		break;
	}
	}
}

void event_manager::undo()
{
	g_csgo.m_event_manager->remove_listener(this);
}