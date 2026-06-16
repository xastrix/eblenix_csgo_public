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
	for (int i = 0; i < maxEvents; i++)
		g_cs->m_event_manager->add_listener(this, g_event_list[i].c_str());
}

void c_event_list::fire_game_event(c_game_event* _event)
{
	const auto name_hash = fnv::hash(_event->get_name());

	if (name_hash == fnv::hash(g_event_list[PLAYER_HURT].c_str()))
	{
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_HURT).value())
		{
			auto attacker_ent_id = g_cs->m_entity_list->get_client_entity<c_base_entity*>(
				g_cs->m_engine->get_player_for_user_id(_event->get_int("attacker")));

			if (attacker_ent_id == g_cs->get_local())
			{
				auto ent_index = g_cs->m_engine->get_player_for_user_id(_event->get_int("userid"));

				if (ent_index)
				{
					auto hitbox = _event->get_int("hitgroup");

					if (hitbox)
					{
						auto damage = _event->get_int("dmg_health");

						if (damage)
						{
							auto health = _event->get_int("health");

							if (health >= 0)
							{
								player_info_t info;
								g_cs->m_engine->get_player_info(ent_index, &info);

								std::string player_name{ info.player_name };
								std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

								Helpers::console_printf_with_prefix("[Eblenix]", "hit %s in the %s for %d (%d health remains)",
									player_name.c_str(), Helpers::hitgroup_name(hitbox).c_str(), damage, health);
							}
						}
					}
				}
			}
		}
	}
	
	if (name_hash == fnv::hash(g_event_list[ITEM_PURCHASE].c_str()))
	{
		if (g_var->get_as<bool>(V_MISC_EVENT_LOGS_PLAYER_PURCHASE).value())
		{
			auto ent_index = g_cs->m_engine->get_player_for_user_id(_event->get_int("userid"));

			if (ent_index)
			{
				auto entity = g_cs->m_entity_list->get_client_entity<c_base_entity*>(ent_index);

				if (entity && (entity->get_team_num() != g_cs->get_local()->get_team_num()))
				{
					player_info_t info;
					g_cs->m_engine->get_player_info(ent_index, &info);

					std::string player_name{ info.player_name };
					std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

					const auto weapon = _event->get_string("weapon");

					Helpers::console_printf_with_prefix("[Eblenix]", "A %s purchased %s",
						player_name.c_str(), weapon != "weapon_unknown" ? weapon : "?");
				}
			}
		}
	}
	
	if (name_hash == fnv::hash(g_event_list[ROUND_PRESTART].c_str()))
	{
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
	}
	
	if (name_hash == fnv::hash(g_event_list[ROUND_START].c_str()))
	{
		player_esp_t::get_instance().on_round_start_e();
	}
	
	if (name_hash == fnv::hash(g_event_list[BOMB_PLANTED].c_str()))
	{
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = true;
	}
	
	if (name_hash == fnv::hash(g_event_list[BOMB_EXPLODED].c_str()) ||
		name_hash == fnv::hash(g_event_list[BOMB_DEFUSED].c_str()))
	{
		GLOBAL(b_flags[BF_BOMB_PLANTED]) = false;
	}

#ifdef LUA_ENABLED
	for (auto _ : LUA_CALLBACK(CL_ON_GAME_EVENTS)) {
		if (_.nulled) continue;

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