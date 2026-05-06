#include "events.h"

#include "vars.h"
#include "globals.h"
#include "color.h"
#include "interfaces.h"
#include "fnv.h"
#ifdef LUA_ENABLED
#include "luas.h"
#endif
#include "visuals.h"

void c_event_list::init()
{
	g_cs->m_event_manager->add_listener(this, "player_hurt");
	g_cs->m_event_manager->add_listener(this, "player_death");
	g_cs->m_event_manager->add_listener(this, "player_death");
	g_cs->m_event_manager->add_listener(this, "player_disconnect");
	g_cs->m_event_manager->add_listener(this, "round_prestart");
	g_cs->m_event_manager->add_listener(this, "round_start");
	g_cs->m_event_manager->add_listener(this, "bomb_planted");
	g_cs->m_event_manager->add_listener(this, "bomb_exploded");
	g_cs->m_event_manager->add_listener(this, "bomb_defused");
}

void c_event_list::fire_game_event(c_game_event* _event)
{
	const auto name_hash = fnv::hash(_event->get_name());

	switch (fnv::hash(_event->get_name())) {
	case fnv::hash("player_hurt"): {
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_HURT).value()) {
			auto attacker_ent_id = g_cs->m_entity_list->get_client_entity<c_base_entity*>(
				g_cs->m_engine->get_player_for_user_id(_event->get_int("attacker")));

			if (attacker_ent_id == g_cs->get_local()) {
				auto ent_index = g_cs->m_engine->get_player_for_user_id(_event->get_int("userid"));
				auto hitbox = _event->get_int("hitgroup");

				if (hitbox) {
					auto damage = _event->get_int("dmg_health");

					if (damage) {
						auto health = _event->get_int("health");

						if (health >= 0) {
							player_info_t info;
							g_cs->m_engine->get_player_info(ent_index, &info);

							std::string player_name{ info.m_player_name };
							std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

							Helpers::console_printf_with_prefix("[Eblenix]", "hit %s in the %s for %d (%d health remains)",
								player_name.c_str(), Helpers::hitgroup_name(hitbox).c_str(), damage, health);
						}
					}
				}
			}
		}
		break;
	}
	case fnv::hash("item_purchase"): {
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_PURCHASE).value()) {
			auto ent_index = g_cs->m_engine->get_player_for_user_id(_event->get_int("userid"));

			if (ent_index) {
				auto entity = g_cs->m_entity_list->get_client_entity<c_base_entity*>(ent_index);

				if (entity->get_team_num() != g_cs->get_local()->get_team_num()) {
					player_info_t info;
					g_cs->m_engine->get_player_info(ent_index, &info);

					std::string player_name{ info.m_player_name };
					std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

					const auto weapon = _event->get_string("weapon");

					Helpers::console_printf_with_prefix("[Eblenix]", "A %s purchased %s",
						player_name.c_str(), weapon != "weapon_unknown" ? weapon : "?");
				}
			}
		}
		break;
	}
	case fnv::hash("round_start"): {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
		player_esp_t::get_instance().on_round_start_e();
		break;
	}
	case fnv::hash("bomb_planted"): {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = true;
		break;
	}
	case fnv::hash("bomb_exploded"): {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
		break;
	}
	case fnv::hash("bomb_defused"): {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
		break;
	}
	}

#ifdef LUA_ENABLED
	for (auto _ : LUA_CALLBACK(CL_ON_GAME_EVENTS)) {
		auto result = _.fn(_event);
		if (!result.valid()) {
			sol::error err = result;
			Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
		}
	}
#endif
}

void c_event_list::undo()
{
	g_cs->m_event_manager->remove_listener(this);
}