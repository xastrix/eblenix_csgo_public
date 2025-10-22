#include "visuals.h"

#include "globals.h"
#include "var_manager.h"
#include "render_manager.h"
#include "helpers.h"
#include "interfaces.h"
#include "math.h"

struct entity_object {
	entity_object(c_base_entity* entity) : _entity(entity) {}
	entity_object(c_base_plantedc4* entity) : _entity(entity) {}

	bool in_class_id(class_ids _class_id) {
		return _entity->get_client_class()->class_id == _class_id;
	}

	bool is_network_name(const std::string& name) {
		return std::string{ _entity->get_client_class()->network_name }.find(name) != std::string::npos;
	}

	bool world_to_screen() {
		_origin = _entity->get_vec_origin();
		return Math::w2s(_origin, _pos);
	}

	vec3 get_pos() {
		return _pos;
	}

	vec3 get_origin() {
		return _origin;
	}
private:
	vec3 _pos{};
	vec3 _origin{};
	c_base_entity* _entity{};
};

void visuals::run()
{
	if (!g_vars.get_as<bool>("visuals->enabled").value())
		return;

	if (g_vars.get_as<bool>("visuals->interface->status").value())
		draw_status(
#ifdef _DEBUG
			"EBLENIX [Dev]", 18
#else
			"EBLENIX", 40
#endif
		);

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.m_local)
		return;

	if (g_vars.get_as<bool>("visuals->interface->spectators").value())
		draw_spectators();

	for (int i = 0; i < g_csgo.m_entity_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity == g_csgo.m_local)
			continue;

		if (g_vars.get_as<bool>("misc->visual->engine_radar").value())
			entity->get_spotted() = true;

		if (g_vars.get_as<bool>("visuals->world->projectlines->enabled").value())
			draw_projectiles(entity);

		if (g_vars.get_as<bool>("visuals->world->items->enabled").value())
		{
			std::vector<std::pair<std::string, class_ids>> entity_objects {
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

		if (g_vars.get_as<bool>("visuals->world->weapons->enabled").value())
		{
			const auto model_name = std::string{ g_csgo.m_model_info->get_model_name(entity->get_model()) };

			if (model_name.find("models/weapons/w_") != std::string::npos &&
				model_name.find("_dropped.mdl") != std::string::npos)
				draw_dropped_weapons(entity);
		}

		if (g_vars.get_as<bool>("visuals->world->c4->enabled").value())
		{
			const auto bomb = reinterpret_cast<c_base_plantedc4*>(entity);

			if (bomb->is_bomb_planted())
			{
				const auto explode_time = bomb->c4_blow() - g_csgo.m_globals->cur_time;

				draw_planted_bomb(bomb, explode_time);

				if (explode_time >= 0.1f)
				{
					if (g_vars.get_as<bool>("visuals->interface->status").value())
						draw_status_bomb_info(bomb, explode_time);
				}
			}
		}
	}

	if (g_vars.get_as<bool>("visuals->removals->scope").value())
	{
		if (g_csgo.m_local->is_life_state())
		{
			if (g_csgo.m_local->is_scoped())
			{
				const auto weapon = g_csgo.m_local->get_active_weapon();

				if (weapon && Helpers::is_sniper(weapon))
				{
					const float X = static_cast<int>(g::screen_width * 0.5f);
					const float Y = static_cast<int>(g::screen_height * 0.5f);

					g_render.draw_line(0, Y, g::screen_width, Y, 1.0f, color_t(0, 0, 0, 155));
					g_render.draw_line(X, 0, X, g::screen_height, 1.0f, color_t(0, 0, 0, 155));
				}
			}
		}
	}
}

void visuals::draw_status(const char* title, int x_offset)
{
	const auto string_width = g_render.get_text_width(title, g_render.get_font(Tahoma12px)) + x_offset;
	const auto ui_shadow_col = color_t(0, 0, 0, g_vars.get_as<int>("ui->col->a").value());

	g_render.draw_filled_rect(g::screen_width - string_width - 15, 10, g::screen_width, 17, ui_shadow_col);

	g_render.draw_filled_rect(g::screen_width - string_width - 12, 13, 2, 11,
		g::initialized ? color_t("ui->col") : color_t(164, 164, 164));

	g_render.draw_string(title, g::screen_width - string_width - 5, 12, g_render.get_font(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

	if (g_csgo.m_engine->is_connected())
	{
		g_render.draw_filled_rect(g::screen_width - string_width - 37, 10, 20, 17, ui_shadow_col);
		g_render.draw_filled_rect(g::screen_width - string_width - 59, 10, 20, 17, ui_shadow_col);
		g_render.draw_filled_rect(g::screen_width - string_width - 81, 10, 20, 17, ui_shadow_col);

		g_render.draw_filled_rect(g::screen_width - string_width - 34, 13, 2, 11,
			g_vars.get_as<bool>("esp->enabled").value() ? color_t("ui->col") : color_t(164, 164, 164));

		g_render.draw_filled_rect(g::screen_width - string_width - 56, 13, 2, 11,
			g_vars.get_as<bool>("triggerbot->enabled").value() ? color_t("ui->col") : color_t(164, 164, 164));

		g_render.draw_filled_rect(g::screen_width - string_width - 78, 13, 2, 11,
			g_vars.get_as<bool>("aimbot->enabled").value() ? color_t("ui->col") : color_t(164, 164, 164));

		g_render.draw_string("E", g::screen_width - string_width - 30, 12,
			g_render.get_font(Verdana12px), TEXT_OUTLINE, color_t(255, 255, 255));

		g_render.draw_string("T", g::screen_width - string_width - 52, 12,
			g_render.get_font(Verdana12px), TEXT_OUTLINE, color_t(255, 255, 255));

		g_render.draw_string("A", g::screen_width - string_width - 74, 12,
			g_render.get_font(Verdana12px), TEXT_OUTLINE, color_t(255, 255, 255));
	}
}

void visuals::draw_spectators()
{
	if (!g_csgo.m_engine->is_in_game())
		return;

	const float Y = static_cast<int>(g::screen_height * 0.5f);

	g_render.draw_filled_rect(g::screen_width - 105, Y, g::screen_width, 18,
		color_t(0, 0, 0, g_vars.get_as<int>("ui->col->a").value()));

	g_render.draw_filled_rect(g::screen_width - 102, Y + 3, 2, 12, color_t("ui->col"));

	g_render.draw_string("Spectators", g::screen_width - 96, Y + 2,
		g_render.get_font(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

	auto offset = 1;
	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
	{
		const auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity == g_csgo.m_local)
			continue;

		if (entity->is_life_state())
			continue;

		if (entity->get_dormant())
			continue;

		const auto target = entity->get_observer_target();

		if (!target)
			continue;

		const auto spectator_target = g_csgo.m_entity_list->get_client_entity_handle(target);

		if (spectator_target != g_csgo.m_local)
			continue;

		g_render.draw_filled_rect(g::screen_width - 105, Y + (19 * offset), g::screen_width, 18,
			color_t(0, 0, 0, g_vars.get_as<int>("ui->col->a").value()));

		g_render.draw_filled_rect(g::screen_width - 102, Y + 3 + (19 * offset), 2, 12,
			color_t(164, 164, 164));

		player_info_t info;
		g_csgo.m_engine->get_player_info(i, &info);

		auto player_name = Helpers::stws(std::string{ info.m_player_name });

		if (player_name.length() > 14) {
			player_name = player_name.substr(0, 14) + L"...";
		}

		g_render.draw_stringW(player_name, g::screen_width - 96, Y + 2 + (19 * offset),
			g_render.get_font(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

		offset++;
	}
}

void visuals::draw_status_bomb_info(c_base_plantedc4* entity, const float explode_time)
{
	std::string bomb_info{};

	switch (entity->bomb_site()) {
	case bombA: {
		bomb_info = "C4 (A): " + std::to_string(static_cast<int>(std::round(explode_time)));
		break;
	}
	case bombB: {
		bomb_info = "C4 (B): " + std::to_string(static_cast<int>(std::round(explode_time)));
		break;
	}
	}

	const auto string_width = g_render.get_text_width(bomb_info, g_render.get_font(Tahoma12px));

	g_render.draw_filled_rect(g::screen_width - 175 - string_width, 10, string_width + 12, 17,
		color_t(0, 0, 0, g_vars.get_as<int>("ui->col->a").value()));

	g_render.draw_filled_rect(g::screen_width - 172 - string_width, 13, 2, 11, color_t("ui->col"));

	g_render.draw_string(bomb_info, g::screen_width - 167 - string_width, 12,
		g_render.get_font(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));
}

void visuals::draw_projectiles(c_base_entity* entity)
{
	entity_object projectlines{ entity };

	if (!projectlines.is_network_name("Projectile"))
		return;

	const auto studio_model = g_csgo.m_model_info->get_studio_model(entity->get_model());
	const auto studio_model_name = std::string{ studio_model->name_char_array };

	if (!studio_model)
		return;

	if (studio_model_name.find("thrown") == std::string::npos && studio_model_name.find("dropped") == std::string::npos)
		return;

	if (!projectlines.world_to_screen())
		return;

	const auto col = color_t("visuals->world->projectlines->col");

	auto y_dist_pos = 10;

	switch (g_vars.get_as<int>("visuals->world->projectlines->type").value()) {
	case 0: {
		if (studio_model_name.find("shbang") != std::string::npos) {
			g_render.draw_string("Flash", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("kegrenade") != std::string::npos) {
			g_render.draw_string("Smoke", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("arygrenade") != std::string::npos) {
			g_render.draw_string("Incendiary", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("olotov") != std::string::npos) {
			g_render.draw_string("Molotov", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("ggrenade") != std::string::npos) {
			g_render.draw_string("He", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("ecoy") != std::string::npos) {
			g_render.draw_string("Decoy", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		break;
	}
	case 1: {
		if (studio_model_name.find("shbang") != std::string::npos) {
			g_render.draw_string("i", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("kegrenade") != std::string::npos) {
			g_render.draw_string("k", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("arygrenade") != std::string::npos) {
			g_render.draw_string("l", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("olotov") != std::string::npos) {
			g_render.draw_string("n", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("ggrenade") != std::string::npos) {
			g_render.draw_string("j", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
		else if (studio_model_name.find("ecoy") != std::string::npos) {
			g_render.draw_string("m", projectlines.get_pos().x, projectlines.get_pos().y,
				g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}

		y_dist_pos += 4;
		break;
	}
	}

	if (g_vars.get_as<bool>("visuals->world->projectlines->distance").value())
	{
		char distance[256];
		sprintf_s(distance, "%im", static_cast<int>(g_csgo.m_local->get_vec_origin().distance_to(projectlines.get_origin())));

		g_render.draw_string(distance, projectlines.get_pos().x, projectlines.get_pos().y + y_dist_pos,
			g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
	}
}

void visuals::draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, class_ids>> objects)
{
	entity_object entity_objects{ entity };

	if (!entity_objects.world_to_screen())
		return;

	const auto col = color_t("visuals->world->items->col");

	char distance[256];
	sprintf_s(distance, "%im", static_cast<int>(g_csgo.m_local->get_vec_origin().distance_to(entity_objects.get_origin())));

	for (const auto& obj : objects)
	{
		if (entity_objects.in_class_id(obj.second))
		{
			g_render.draw_string(obj.first, entity_objects.get_pos().x, entity_objects.get_pos().y,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);

			if (g_vars.get_as<bool>("visuals->world->items->distance").value())
			{
				g_render.draw_string(distance, entity_objects.get_pos().x, entity_objects.get_pos().y + 10,
					g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			}
		}
	}
}

void visuals::draw_dropped_weapons(c_base_entity* entity)
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

		const auto col = color_t("visuals->world->weapons->col");

		auto y_ammo_bar_pos = 14;
		auto y_dist_pos = g_vars.get_as<bool>("visuals->world->weapons->ammo_bar").value() ? 18 : 11;

		switch (g_vars.get_as<int>("visuals->world->weapons->type").value()) {
		case 0: {
			g_render.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), we_text), dropped_weapons.get_pos().x,
				dropped_weapons.get_pos().y, g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			break;
		}
		case 1: {
			g_render.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), we_icon), dropped_weapons.get_pos().x,
				dropped_weapons.get_pos().y, g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);

			y_ammo_bar_pos += 4;
			y_dist_pos += 5;
			break;
		}
		}

		if (g_vars.get_as<bool>("visuals->world->weapons->ammo_bar").value())
		{
			auto width = 40;

			width *= weapon->clip1_count();
			width /= weapon->get_weapon_data()->weapon_max_clip;

			const auto ammo_bar_col = color_t("visuals->world->weapons->ammo_bar->col");

			g_render.draw_filled_rect(dropped_weapons.get_pos().x - 20, dropped_weapons.get_pos().y + y_ammo_bar_pos, 42, 4, color_t(3, 3, 3));
			g_render.draw_filled_rect(dropped_weapons.get_pos().x - 19, dropped_weapons.get_pos().y + y_ammo_bar_pos + 1, width, 2, ammo_bar_col);
		}

		if (g_vars.get_as<bool>("visuals->world->weapons->distance").value())
		{
			char distance[256];
			sprintf_s(distance, "%im", static_cast<int>(g_csgo.m_local->get_vec_origin().distance_to(dropped_weapons.get_origin())));

			g_render.draw_string(distance, dropped_weapons.get_pos().x, dropped_weapons.get_pos().y + y_dist_pos,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		}
	}
}

void visuals::draw_planted_bomb(c_base_plantedc4* entity, const float explode_time)
{
	entity_object planted_bomb{ entity };

	if (!planted_bomb.world_to_screen())
		return;

	const auto col = color_t("visuals->world->c4->col");

	auto time_bar_offset = 16;
	auto defuse_bar_offset = g_vars.get_as<bool>("visuals->world->c4->time_bar").value() ? 23 : 16;

	switch (g_vars.get_as<int>("visuals->world->c4->type").value()) {
	case 0: {
		g_render.draw_string("C4", planted_bomb.get_pos().x, planted_bomb.get_pos().y,
			g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
		break;
	}
	case 1: {
		g_render.draw_string("o", planted_bomb.get_pos().x, planted_bomb.get_pos().y,
			g_render.get_font(Astriumwep25px), TEXT_OUTLINE | TEXT_CENTER_X, col);

		time_bar_offset += 9;
		defuse_bar_offset += 9;
		break;
	}
	}

	if (explode_time <= 0.1f)
		return;

	if (!entity->bomb_ticking())
		return;

	if (g_vars.get_as<bool>("visuals->world->c4->damage_indicator").value())
	{
		const auto damage = 500.f;
		const auto bomb_radius = damage * 3.5f;
		const auto distance_to_player = (planted_bomb.get_origin() - g_csgo.m_local->get_eye_pos()).length();
		const auto sigma = bomb_radius / 3.0f;
		const auto gaussian_falloff = std::exp(-distance_to_player * distance_to_player / (2.0f * sigma * sigma));

		int adjust_damage = damage * gaussian_falloff * 1.0f;
		adjust_damage = Math::get_damage_armor(adjust_damage, g_csgo.m_local->get_armor_value());

		if (adjust_damage != 0)
		{
			if (adjust_damage > g_csgo.m_local->get_health() || adjust_damage == 100)
			{
				g_render.draw_string("Dead", planted_bomb.get_pos().x, planted_bomb.get_pos().y - 12, g_render.get_font(Tahoma12px),
					TEXT_OUTLINE | TEXT_CENTER_X, color_t(245, 92, 108, 255));
			}
			else
			{
				char buf[256];
				sprintf_s(buf, "-%i HP", adjust_damage);

				g_render.draw_string(buf, planted_bomb.get_pos().x, planted_bomb.get_pos().y - 12, g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
			}
		}
	}

	const auto background_col = color_t(3, 3, 3);

	if (g_vars.get_as<bool>("visuals->world->c4->time_bar").value())
	{
		const auto time_bar_col = color_t("visuals->world->c4->time_bar->col");

		g_render.draw_filled_rect(planted_bomb.get_pos().x - 20, planted_bomb.get_pos().y + time_bar_offset, Helpers::get_c4_server_time(), 4, background_col);
		g_render.draw_filled_rect(planted_bomb.get_pos().x - 19, planted_bomb.get_pos().y + time_bar_offset + 1, explode_time - 1, 2, time_bar_col);
	}

	if (g_vars.get_as<bool>("visuals->world->c4->defuse_bar").value())
	{
		if (entity->bomb_defuser() > 0)
		{
			float m_flCountDown = entity->defuse_count_down() - (g_csgo.m_local->get_tick_base() * g_csgo.m_globals->interval_per_tick);
			m_flCountDown /= 10;

			const auto defuse_bar_col = color_t("visuals->world->c4->defuse_bar->col");

			g_render.draw_filled_rect(planted_bomb.get_pos().x - 20, planted_bomb.get_pos().y + defuse_bar_offset, Helpers::get_c4_server_time(), 4, background_col);
			g_render.draw_filled_rect(planted_bomb.get_pos().x - 19, planted_bomb.get_pos().y + defuse_bar_offset + 1, Helpers::get_c4_server_time() * m_flCountDown - 1, 2, defuse_bar_col);
		}
	}
}