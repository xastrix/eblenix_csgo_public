#include "events.h"

#include "vars.h"
#include "globals.h"
#include "color.h"
#include "esp.h"
#include "interfaces.h"
#include "fnv.h"

static std::vector<std::pair<uint32_t, std::function<void(c_game_event*)>>> g_event_list{};

static void listen_event(c_game_event_listener2* listener, const char* name, std::function<void(c_game_event*)> fn)
{
	if (!g_cs->m_event_manager->add_listener(listener, name))
		return;

	g_event_list.push_back({ fnv::hash(name), fn });
}

void c_event_list::init()
{
	listen_event(this, "player_hurt", [](c_game_event* event) {
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_HURT).value()) {
			auto attacker_ent_id = g_cs->m_entity_list->get_client_entity<c_base_entity*>(
				g_cs->m_engine->get_player_for_user_id(event->get_int("attacker"))
			);

			if (attacker_ent_id == g_cs->get_local())
			{
				auto ent_index = g_cs->m_engine->get_player_for_user_id(event->get_int("userid"));
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
							g_cs->m_engine->get_player_info(ent_index, &info);

							std::string player_name{ info.player_name };
							std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

							g_cs->m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[Eblenix] ");
							g_cs->m_cvar->console_printf("hit %s in the %s for %d (%d health remains)\n",
								player_name.c_str(), Helpers::hitgroup_name(hitbox).c_str(), damage, health);
						}
					}
				}
			}
		}
	});

	listen_event(this, "item_purchase", [](c_game_event* event) {
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_PURCHASE).value()) {
			auto ent_index = g_cs->m_engine->get_player_for_user_id(event->get_int("userid"));

			if (ent_index)
			{
				auto entity = g_cs->m_entity_list->get_client_entity<c_base_entity*>(ent_index);

				if (entity->get_team_num() != g_cs->get_local()->get_team_num())
				{
					player_info_t info{};
					g_cs->m_engine->get_player_info(ent_index, &info);

					std::string player_name{ info.player_name };
					std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

					const auto weapon = event->get_string("weapon");

					g_cs->m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[Eblenix] ");
					g_cs->m_cvar->console_printf("A %s purchased %s\n", player_name.c_str(), weapon != "weapon_unknown" ? weapon : "?");
				}
			}
		}
	});

	listen_event(this, "round_start", [](c_game_event*) {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
		g_esp->on_round_start_e();
	});

	listen_event(this, "bomb_planted", [](c_game_event*) { 
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = true;
	});

	listen_event(this, "bomb_exploded", [](c_game_event*) {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
	});

	listen_event(this, "bomb_defused", [](c_game_event*) {
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
	});
}

void c_event_list::fire_game_event(c_game_event* event)
{
	for (const auto& e : g_event_list)
	{
		if (fnv::hash(event->get_name()) == e.first)
			e.second(event);
	}
}

void c_event_list::undo()
{
	if (g_event_list.empty())
		return;

	g_event_list.clear();
	g_cs->m_event_manager->remove_listener(this);
}