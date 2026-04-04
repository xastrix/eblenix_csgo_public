#include "visuals.h"

#include "vars.h"
#include "globals.h"
#include "interfaces.h"
#include "renderer.h"
#include "math.h"

struct entity_object {
	entity_object(void* entity) : m_entity(entity) {}

	bool in_class_id(const _class_ids _class_id) {
		auto temp_ent{ reinterpret_cast<c_base_entity*>(m_entity) };
		return temp_ent->get_client_class()->class_id == _class_id;
	}

	bool is_network_name(const std::string& name) {
		auto temp_ent{ reinterpret_cast<c_base_entity*>(m_entity) };
		return std::string{ temp_ent->get_client_class()->network_name }.find(name) != std::string::npos;
	}

	bool world_to_screen() {
		auto temp_ent{ reinterpret_cast<c_base_entity*>(m_entity) };
		m_origin = temp_ent->get_vec_origin();

		return Math::w2s(m_origin, m_pos);
	}

	vec3 get_pos() {
		return m_pos;
	}

	vec3 get_origin() {
		return m_origin;
	}
private:
	vec3  m_pos{};
	vec3  m_origin{};
	void* m_entity{};
};

void world_esp_t::think()
{
	if (!g_var->get_as<bool>(V_VISUALS_ENABLED).value())
		return;

	for (int i = 0; i < g_cs->m_entity_list->get_highest_index(); i++)
	{
		auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

		if (!entity)
			continue;

		if (entity == g_cs->get_local())
			continue;

		if (g_var->get_as<bool>(V_VISUALS_ENGINE_RADAR).value())
			entity->get_spotted() = true;

		if (g_var->get_as<bool>(V_VISUALS_WORLD_GRENADES_ENABLED).value())
			draw_projectiles(entity);

		if (g_var->get_as<bool>(V_VISUALS_WORLD_ITEMS_ENABLED).value())
		{
			std::vector<std::pair<std::string, _class_ids>> entity_objects{
				{ "Drone",         cdrone },
				{ "Radar Jammer",  cphyspropradarjammer },
				{ "Sentry Turret", cdronegun },
				{ "Melee",         cmelee },
				{ "Cash",          citemcash },
				{ "Healthshot",    citem_healthshot },
				{ "Tablet",        ctablet },
				{ "Chicken",       cchicken },
				{ "Hostage",       chostage },
				{ "Fish",          cfish },
			};

			draw_entity_objects(entity, entity_objects);
		}

		if (g_var->get_as<bool>(V_VISUALS_WORLD_WEAPONS_ENABLED).value())
		{
			const auto model_name = std::string{ g_cs->m_model_info->get_model_name(entity->get_model()) };

			if (model_name.find("models/weapons/w_") != std::string::npos &&
				model_name.find("_dropped.mdl") != std::string::npos)
				draw_dropped_weapons(entity);
		}

		if (g_var->get_as<bool>(V_VISUALS_WORLD_C4_ENABLED).value())
		{
			if (GLOBAL(b_flags[BF_BOMB_PLANTED]))
			{
				if (entity->get_client_class()->class_id == cplantedc4)
				{
					const auto bomb = reinterpret_cast<c_base_plantedc4*>(entity);
					const auto explode_time = bomb->c4_blow() - g_cs->m_globals->cur_time;

					draw_planted_bomb(bomb, explode_time);

					GLOBAL(i_flags[IF_BOMB_TIMER]) = static_cast<int>(std::round(explode_time));
					GLOBAL(i_flags[IF_BOMB_SITE_ID]) = bomb->bomb_site();
				}
			}
		}
	}

	if (g_var->get_as<bool>(V_VISUALS_REMOVALS_SCOPE).value())
	{
		if (g_cs->get_local()->is_life_state())
		{
			if (g_cs->get_local()->is_scoped())
			{
				const auto weapon = g_cs->get_local()->get_active_weapon();

				if (weapon && Helpers::is_sniper(weapon))
				{
					vec2 screen_size = g_renderer->get_screen_size();

					const float center_y = screen_size.y * 0.5f;
					const float center_x = screen_size.x * 0.5f;

					g_renderer->line(0, center_y, screen_size.x, center_y, color_t(0, 0, 0, 155));
					g_renderer->line(center_x, 0, center_x, screen_size.y, color_t(0, 0, 0, 155));
				}
			}
		}
	}
}

void world_esp_t::on_do_post_screen_effects()
{
	if (!g_var->get_as<bool>(V_GLOW_ENABLED).value())
		return;

	if (!g_cs->m_engine->is_connected())
		return;

	if (!g_cs->get_local())
		return;

	for (int i = 0; i < g_cs->m_glow_manager->get_size(); i++)
	{
		auto& glow = g_cs->m_glow_manager->glow_object_definitions[i];

		if (glow.is_unused())
			continue;

		const auto glow_object = reinterpret_cast<c_base_player*>(glow.m_entity);

		if (!glow_object || glow_object->get_dormant())
			continue;

		if (!g_cs->get_local()->can_see_entity(glow_object->get_eye_pos()) & g_var->get_as<bool>(V_GLOW_VISIBLE_ONLY).value())
			continue;

		const auto client_class = glow_object->get_client_class();

		if (g_var->get_as<bool>(V_GLOW_C4).value())
		{
			if (client_class->class_id == cc4 || client_class->class_id == cplantedc4)
			{
				glow.set_glow(
					g_var->get_as<int>(V_GLOW_C4_COL_R).value() / 255.0f,
					g_var->get_as<int>(V_GLOW_C4_COL_G).value() / 255.0f,
					g_var->get_as<int>(V_GLOW_C4_COL_B).value() / 255.0f,
					g_var->get_as<int>(V_GLOW_C4_COL_A).value() / 255.0f
				);
			}
		}

		if (!glow_object->is_moving() & g_var->get_as<bool>(V_GLOW_WALKING_ONLY).value())
			continue;

		if (client_class->class_id == ccsplayer)
		{
			const auto is_enemy = glow_object->get_team_num() != g_cs->get_local()->get_team_num();
			const auto is_teammate = glow_object->get_team_num() == g_cs->get_local()->get_team_num();

			float glow_enemy_col[4];
			float glow_team_col[4];

			if (g_var->get_as<bool>(V_GLOW_HEALTH_BASED).value())
			{
				color_t health_col = color_t::calc_health_color(glow_object->get_health());

				glow_enemy_col[0] = health_col.get_arr()[0] / 255.0f;
				glow_enemy_col[1] = health_col.get_arr()[1] / 255.0f;
				glow_enemy_col[2] = health_col.get_arr()[2] / 255.0f;

				glow_team_col[0] = health_col.get_arr()[0] / 255.0f;
				glow_team_col[1] = health_col.get_arr()[1] / 255.0f;
				glow_team_col[2] = health_col.get_arr()[2] / 255.0f;
			}
			else
			{
				glow_enemy_col[0] = g_var->get_as<int>(V_GLOW_ENEMY_COL_R).value() / 255.0f;
				glow_enemy_col[1] = g_var->get_as<int>(V_GLOW_ENEMY_COL_G).value() / 255.0f;
				glow_enemy_col[2] = g_var->get_as<int>(V_GLOW_ENEMY_COL_B).value() / 255.0f;

				glow_team_col[0] = g_var->get_as<int>(V_GLOW_TEAM_COL_R).value() / 255.0f;
				glow_team_col[1] = g_var->get_as<int>(V_GLOW_TEAM_COL_G).value() / 255.0f;
				glow_team_col[2] = g_var->get_as<int>(V_GLOW_TEAM_COL_B).value() / 255.0f;
			}

			glow_enemy_col[3] = g_var->get_as<int>(V_GLOW_ENEMY_COL_A).value() / 255.0f;
			glow_team_col[3] = g_var->get_as<int>(V_GLOW_TEAM_COL_A).value() / 255.0f;

			const auto flash_duration = glow_object->get_flash_duration();

			if (is_enemy) {
				glow.set_glow(
					glow_enemy_col[0], glow_enemy_col[1], glow_enemy_col[2],

					flash_duration > g_var->get_as<int>(V_GLOW_ENEMY_COL_A).value() ?
					flash_duration / 255.0f : glow_enemy_col[3]
				);
			}

			if (is_teammate && g_var->get_as<bool>(V_GLOW_TEAM).value()) {
				glow.set_glow(
					glow_team_col[0], glow_team_col[1], glow_team_col[2],

					flash_duration > g_var->get_as<int>(V_GLOW_TEAM_COL_A).value() ?
					flash_duration / 255.0f : glow_team_col[3]
				);
			}
		}
	}
}

void world_esp_t::on_scene_end()
{
	if (g_var->get_as<bool>(V_CHAMS_ENABLED).value())
	{
		if (g_cs->m_engine->is_connected())
		{
			if (g_cs->get_local())
			{
				for (int i = 1; i <= g_cs->m_globals->max_clients; i++)
				{
					auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

					if (!entity)
						continue;

					if (entity == g_cs->get_local())
						continue;

					if (entity->get_health() <= 0)
						continue;

					if (entity->get_team_num() == g_cs->get_local()->get_team_num() & !g_var->get_as<bool>(V_CHAMS_TEAM).value())
						continue;

					if (!entity->is_moving() & g_var->get_as<bool>(V_CHAMS_WALKING_ONLY).value())
						continue;

					const auto debugambientcube = g_cs->m_mat_system->find_material(
						"debug/debugambientcube", TEXTURE_GROUP_MODEL
					);

					const auto flat = g_cs->m_mat_system->find_material(
						"debug/debugdrawflat", TEXTURE_GROUP_MODEL
					);

					const auto dogtags_outline = g_cs->m_mat_system->find_material(
						"models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL
					);

					debugambientcube->increment_reference_count();
					flat->increment_reference_count();
					dogtags_outline->increment_reference_count();

					c_material* material{};
					switch (g_var->get_as<int>(V_CHAMS_TYPE).value()) {
					case 0: {
						material = debugambientcube;
						break;
					}
					case 1: {
						material = flat;
						break;
					}
					case 2: {
						material = dogtags_outline;
						break;
					}
					}

					float col[3];
					if (g_var->get_as<bool>(V_CHAMS_HEALTH_BASED).value())
					{
						color_t health_col = color_t::calc_health_color(entity->get_health());

						col[0] = health_col.get_arr()[0] / 255.0f;
						col[1] = health_col.get_arr()[1] / 255.0f;
						col[2] = health_col.get_arr()[2] / 255.0f;
					}
					else
					{
						col[0] = g_var->get_as<int>(V_CHAMS_COL_R).value() / 255.0f;
						col[1] = g_var->get_as<int>(V_CHAMS_COL_G).value() / 255.0f;
						col[2] = g_var->get_as<int>(V_CHAMS_COL_B).value() / 255.0f;
					}

					g_cs->m_render_view->modulate_color(col);
					g_cs->m_render_view->set_blend(1.0f);

					material->set_material_var_flag(material_var_ignorez, !g_var->get_as<bool>(V_CHAMS_VISIBLE_ONLY).value());

					g_cs->m_model_render->override_material(material);
					entity->draw_model(1, 255);

					g_cs->m_model_render->override_material(nullptr);
				}
			}
		}
	}

	if (g_var->get_as<bool>(V_VISUALS_ENABLED).value())
	{
		if (g_cs->m_engine->is_connected())
		{
			std::vector<c_material*> smoke_materials = {
				g_cs->m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_OTHER),
				g_cs->m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_OTHER),
				g_cs->m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_OTHER),
				g_cs->m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_OTHER)
			};

			std::vector<c_material*> flash_materials = {
				g_cs->m_mat_system->find_material("effects/flashbang", TEXTURE_GROUP_OTHER),
				g_cs->m_mat_system->find_material("effects/flashbang_white", TEXTURE_GROUP_OTHER)
			};

			for (const auto& mat : smoke_materials) {
				if (mat) mat->set_material_var_flag(material_var_no_draw, g_var->get_as<bool>(V_VISUALS_REMOVALS_SMOKE).value());
			}

			for (const auto& mat : flash_materials) {
				if (mat) mat->set_material_var_flag(material_var_no_draw, g_var->get_as<bool>(V_VISUALS_REMOVALS_FLASH).value());
			}
		}
	}
}

void world_esp_t::draw_projectiles(c_base_entity* entity)
{
	entity_object projectiles{ entity };

	if (!projectiles.is_network_name("Projectile"))
		return;

	const auto studio_model = g_cs->m_model_info->get_studio_model(entity->get_model());
	const auto studio_model_name = std::string{ studio_model->m_name_char_array };

	if (!studio_model)
		return;

	if (studio_model_name.find("thrown") == std::string::npos && studio_model_name.find("dropped") == std::string::npos)
		return;

	if (!projectiles.world_to_screen())
		return;

	const auto col = color_t(V_VISUALS_WORLD_GRENADES_COL);

	auto y_dist_pos = 10;

	switch (g_var->get_as<int>(V_VISUALS_WORLD_GRENADES_TYPE).value()) {
	case 0: {
		if (studio_model_name.find("shbang") != std::string::npos)
			g_font->draw_string("Flash", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("kegrenade") != std::string::npos)
			g_font->draw_string("Smoke", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("arygrenade") != std::string::npos)
			g_font->draw_string("Incendiary", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("olotov") != std::string::npos)
			g_font->draw_string("Molotov", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("ggrenade") != std::string::npos)
			g_font->draw_string("He", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("ecoy") != std::string::npos)
			g_font->draw_string("Decoy", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		break;
	}
	case 1: {
		if (studio_model_name.find("shbang") != std::string::npos)
			g_font->draw_string("i", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("kegrenade") != std::string::npos)
			g_font->draw_string("k", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("arygrenade") != std::string::npos)
			g_font->draw_string("l", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("olotov") != std::string::npos)
			g_font->draw_string("n", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("ggrenade") != std::string::npos)
			g_font->draw_string("j", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		else if (studio_model_name.find("ecoy") != std::string::npos)
			g_font->draw_string("m", projectiles.get_pos().x, projectiles.get_pos().y,
				FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		y_dist_pos += 4;
		break;
	}
	}

	if (g_var->get_as<bool>(V_VISUALS_WORLD_GRENADES_DISTANCE).value())
	{
		char distance[256];
		sprintf_s(distance, "%im", static_cast<int>(g_cs->get_local()->get_vec_origin().distance_to(projectiles.get_origin())));

		g_font->draw_string(distance, projectiles.get_pos().x, projectiles.get_pos().y + y_dist_pos,
			FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
	}
}

void world_esp_t::draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, _class_ids>> objects)
{
	entity_object entities{ entity };

	if (!entities.world_to_screen())
		return;

	const auto col = color_t(V_VISUALS_WORLD_ITEMS_COL);

	char distance[256];
	sprintf_s(distance, "%im", static_cast<int>(g_cs->get_local()->get_vec_origin().distance_to(entities.get_origin())));

	for (const auto& obj : objects)
	{
		if (entities.in_class_id(obj.second))
		{
			g_font->draw_string(obj.first, entities.get_pos().x, entities.get_pos().y,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

			if (g_var->get_as<bool>(V_VISUALS_WORLD_ITEMS_DISTANCE).value())
			{
				g_font->draw_string(distance, entities.get_pos().x, entities.get_pos().y + 10,
					FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			}
		}
	}
}

void world_esp_t::draw_dropped_weapons(c_base_entity* entity)
{
	entity_object dropped_weapons{ entity };

	if ((dropped_weapons.is_network_name("CWeapon")) ||
		dropped_weapons.in_class_id(cdeagle) ||
		dropped_weapons.in_class_id(cak47))
	{
		if (!dropped_weapons.world_to_screen())
			return;

		const auto weapon = reinterpret_cast<c_base_weapon*>(entity);

		if (!weapon || !weapon->item_definition_index())
			return;

		const auto col = color_t(V_VISUALS_WORLD_WEAPONS_COL);

		auto y_ammo_bar_pos = 14;
		auto y_dist_pos = g_var->get_as<bool>(V_VISUALS_WORLD_WEAPONS_AMMO_BAR).value() ? 18 : 11;

		switch (g_var->get_as<int>(V_VISUALS_WORLD_WEAPONS_TYPE).value()) {
		case 0: {
			g_font->draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_TEXT), dropped_weapons.get_pos().x,
				dropped_weapons.get_pos().y, FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			break;
		}
		case 1: {
			g_font->draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_ICON), dropped_weapons.get_pos().x,
				dropped_weapons.get_pos().y, FONT(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

			y_ammo_bar_pos += 4;
			y_dist_pos += 5;
			break;
		}
		}

		if (g_var->get_as<bool>(V_VISUALS_WORLD_WEAPONS_AMMO_BAR).value())
		{
			auto width = 40;

			width *= weapon->clip1_count();
			width /= weapon->get_weapon_data()->m_weapon_max_clip;

			const auto ammo_bar_col = color_t(V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL);

			g_renderer->rect_fill(dropped_weapons.get_pos().x - 20, dropped_weapons.get_pos().y + y_ammo_bar_pos, 42, 4, color_t(3, 3, 3));
			g_renderer->rect_fill(dropped_weapons.get_pos().x - 19, dropped_weapons.get_pos().y + y_ammo_bar_pos + 1, width, 2, ammo_bar_col);
		}

		if (g_var->get_as<bool>(V_VISUALS_WORLD_WEAPONS_DISTANCE).value())
		{
			char distance[256];
			sprintf_s(distance, "%im", static_cast<int>(g_cs->get_local()->get_vec_origin().distance_to(dropped_weapons.get_origin())));

			g_font->draw_string(distance, dropped_weapons.get_pos().x, dropped_weapons.get_pos().y + y_dist_pos,
				FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
	}
}

void world_esp_t::draw_planted_bomb(c_base_plantedc4* entity, const float explode_time)
{
	entity_object planted_bomb{ entity };

	if (!planted_bomb.world_to_screen())
		return;

	const auto col = color_t(V_VISUALS_WORLD_C4_COL);

	auto time_bar_offset = 16;
	auto defuse_bar_offset = g_var->get_as<bool>(V_VISUALS_WORLD_C4_TIME_BAR).value() ? 23 : 16;

	switch (g_var->get_as<int>(V_VISUALS_WORLD_C4_TYPE).value()) {
	case 0: {
		g_font->draw_string("C4", planted_bomb.get_pos().x, planted_bomb.get_pos().y,
			FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		break;
	}
	case 1: {
		g_font->draw_string("o", planted_bomb.get_pos().x, planted_bomb.get_pos().y,
			FONT(Astriumwep25px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		time_bar_offset += 9;
		defuse_bar_offset += 9;
		break;
	}
	}

	if (explode_time <= 0.1f)
		return;

	if (!entity->bomb_ticking())
		return;

	if (g_var->get_as<bool>(V_VISUALS_WORLD_C4_DAMAGE_INDICATOR).value())
	{
		const auto damage = 500.f;
		const auto bomb_radius = damage * 3.5f;
		const auto distance_to_player = length(planted_bomb.get_origin() - g_cs->get_local()->get_eye_pos());
		const auto sigma = bomb_radius / 3.0f;
		const auto gaussian_falloff = std::exp(-distance_to_player * distance_to_player / (2.0f * sigma * sigma));

		int adjust_damage = damage * gaussian_falloff * 1.0f;
		adjust_damage = Math::get_damage_armor(adjust_damage, g_cs->get_local()->get_armor_value());

		if (adjust_damage != 0)
		{
			if (adjust_damage > g_cs->get_local()->get_health() || adjust_damage == 100)
			{
				g_font->draw_string("Dead", planted_bomb.get_pos().x, planted_bomb.get_pos().y - 12, FONT(Tahoma12px),
					TEXT_OUTLINE | TEXT_CENTER_X, color_t(245, 92, 108, 255));
			}
			else
			{
				char buf[256];
				sprintf_s(buf, "-%i HP", adjust_damage);

				g_font->draw_string(buf, planted_bomb.get_pos().x, planted_bomb.get_pos().y - 12, FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			}
		}
	}

	const auto background_col = color_t(3, 3, 3);

	if (g_var->get_as<bool>(V_VISUALS_WORLD_C4_TIME_BAR).value())
	{
		const auto time_bar_col = color_t(V_VISUALS_WORLD_C4_COL_TIME_BAR);

		g_renderer->rect_fill(planted_bomb.get_pos().x - 20, planted_bomb.get_pos().y + time_bar_offset, Helpers::get_c4_server_time(), 4, background_col);
		g_renderer->rect_fill(planted_bomb.get_pos().x - 19, planted_bomb.get_pos().y + time_bar_offset + 1, explode_time - 1, 2, time_bar_col);
	}

	if (g_var->get_as<bool>(V_VISUALS_WORLD_C4_DEFUSE_BAR).value())
	{
		auto defuser = g_cs->m_entity_list->get_client_entity_handle<c_base_player*>(entity->bomb_defuser());

		if (defuser > 0)
		{
			auto defuse_time_without_kits = 10.0f;
			auto defuse_time_with_kits = (defuse_time_without_kits - 5.0f);

			auto count_down = entity->defuse_count_down() - (g_cs->get_local()->get_tick_base() * g_cs->m_globals->interval_per_tick);
			auto max_defuse_time = defuser->has_defuser() ? defuse_time_with_kits : defuse_time_without_kits;

			const auto defuse_bar_col = color_t(V_VISUALS_WORLD_C4_COL_DEFUSE_BAR);

			g_renderer->rect_fill(planted_bomb.get_pos().x - 20, planted_bomb.get_pos().y + defuse_bar_offset, Helpers::get_c4_server_time(), 4, background_col);
			g_renderer->rect_fill(planted_bomb.get_pos().x - 19, planted_bomb.get_pos().y + defuse_bar_offset + 1, (Helpers::get_c4_server_time() * count_down / max_defuse_time) - 1, 2, defuse_bar_col);
		}
	}
}