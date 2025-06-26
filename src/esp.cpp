#include "esp.h"

#include "globals.h"
#include "vars.h"
#include "renderer.h"
#include "interfaces.h"
#include "math.h"

#include <array>

void esp::run()
{
	if (!g_vars.get_as<bool>(V_ESP_ENABLED).value())
		return;

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.get_local())
		return;

	for (int i = 0; i <= MAX_PLAYERS; i++)
	{
		auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

		if (!entity || entity->get_health() <= 0 || entity == g_csgo.get_local()) {
			reset_position(i);
			continue;
		}

		if ((entity->get_team_num() == g_csgo.get_local()->get_team_num()) & !g_vars.get_as<bool>(V_ESP_TEAM).value())
			continue;

		if (!g_csgo.get_local()->can_see_entity(entity->get_eye_pos()) & g_vars.get_as<bool>(V_ESP_VISIBLE_ONLY).value())
			continue;

		if (!entity->is_moving() & g_vars.get_as<bool>(V_ESP_WALKING_ONLY).value())
			continue;

		float& am{ m_anim_progress[i] };
		m_alpha[i] = am;

		// calculate alpha of esp when we can see players
		calc_player_animation_progress(i, am, entity);

		box bbox{};
		if (get_player_bbox(entity, bbox))
			player_rendering(i, entity, bbox);
	}
}

void esp::on_round_start_e()
{
	for (int i = 0; i <= MAX_PLAYERS; i++) {
		reset_position(i);
	}
}

void esp::calc_player_animation_progress(int index, float& anim, c_base_player* entity)
{
	float rate = g_csgo.m_globals->frame_time * 1.0f / 0.5f;

	if (!entity->get_dormant()) {
		update_position(index, entity->get_absolute_origin());

		anim = anim > 0.0f ? std::clamp(anim + rate, 0.0f, 1.0f) : 0.5f;
		m_has_seen[index] = true;
	}
	else {
		if (anim < 0.30f && !(entity->get_vec_origin().distance_to(g_csgo.get_local()->get_vec_origin()) > MAX_PLAYER_RENDERING_DISTANCE)) {
			rate *= 0.02f;
		}

		anim = std::clamp(anim -= rate, 0.0f, 1.0f);

		if (m_anim_progress[index] <= 0.0f) {
			m_has_seen[index] = false;
		}
	}
}

bool esp::get_player_bbox(c_base_player* entity, box& out)
{
	if (!Helpers::get_bbox(entity, out, static_cast<bbox_type>(g_vars.get_as<int>(V_ESP_RENDER_TYPE).value())))
		return false;

	return true;
}

void esp::player_rendering(int index, c_base_player* entity, box bbox)
{
	if (g_vars.get_as<bool>(V_ESP_NAME_ENABLED).value())
	{
		player_info_t info;
		g_csgo.m_engine->get_player_info(index, &info);

		auto player_name = Helpers::stws(std::string{ info.player_name });

		switch (g_vars.get_as<int>(V_ESP_NAME_TYPE).value()) {
		case 1: {
			std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);
			break;
		}
		case 2: {
			std::transform(player_name.begin(), player_name.end(), player_name.begin(), toupper);
			break;
		}
		}

		if (player_name.length() > 10)
			player_name = player_name.substr(0, 10) + L"...";

		g_font.draw_stringW(player_name, bbox.x + (bbox.w / 2), bbox.y - 14,
			g_font[Tahoma12px], TEXT_OUTLINE | TEXT_CENTER_X, color_t(V_ESP_NAME_COL, 255 * m_alpha[index]));
	}

	const auto background_col = color_t(3, 3, 3, 255 * m_alpha[index]);

	if (g_vars.get_as<bool>(V_ESP_BOX_ENABLED).value())
	{
		const auto col = color_t(V_ESP_BOX_COL, 255 * m_alpha[index]);
		const auto outline_col = color_t(V_ESP_BOX_COL, 30 * m_alpha[index]);

		switch (g_vars.get_as<int>(V_ESP_BOX_TYPE).value()) {
		case 0: {
			g_render.draw_line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, 1.0f, background_col);
			g_render.draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, background_col);
			g_render.draw_line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, 1.0f, background_col);
			g_render.draw_line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, background_col);
			
			g_render.draw_line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, 1.0f, col);
			g_render.draw_line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			g_render.draw_line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, 1.0f, col);
			g_render.draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			break;
		}
		case 1: {
			g_render.draw_line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, 1.0f, background_col);
			g_render.draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, background_col);
			g_render.draw_line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, 1.0f, background_col);
			g_render.draw_line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, background_col);

			g_render.draw_filled_rect(bbox.x, bbox.y, bbox.w, bbox.h - 1, outline_col);

			g_render.draw_line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, 1.0f, col);
			g_render.draw_line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			g_render.draw_line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, 1.0f, col);
			g_render.draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
			break;
		}
		case 2: {
			g_render.draw_corner_box(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, 3, 5, background_col);
			g_render.draw_corner_box(bbox.x, bbox.y, bbox.w, bbox.h, 3, 5, col);
			break;
		}
		case 3: {
			g_render.draw_filled_rect(bbox.x, bbox.y, bbox.w, bbox.h - 1, outline_col);
			g_render.draw_corner_box(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, 3, 5, background_col);
			g_render.draw_corner_box(bbox.x, bbox.y, bbox.w, bbox.h, 3, 5, col);
			break;
		}
		}
	}

	if (g_vars.get_as<bool>(V_ESP_HEALTH_ENABLED).value())
	{
		const auto hp = entity->get_health() > 100 ? 100 : entity->get_health();

		if (hp)
		{
			const auto pixel_value = hp * (bbox.h + 1) / 100;

			switch (g_vars.get_as<int>(V_ESP_HEALTH_TYPE).value()) {
			case 0: {
				const auto col = color_t(V_ESP_HEALTH_COL, 255 * m_alpha[index]);

				g_render.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_render.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value, col);
				break;
			}
			case 1: {
				g_render.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_render.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t(99, std::min(255, hp * 225 / 100), 0, 255 * m_alpha[index]));
				break;
			}
			}

			if (g_vars.get_as<bool>(V_ESP_HEALTH_BATTERY).value())
			{
				for (int i = 0; i < 9; i++) {
					g_render.draw_line(bbox.x - 5, bbox.y + i * ((bbox.h + 1) / 9.0f) - 1.0f, bbox.x - 3,
						bbox.y + i * ((bbox.h + 1) / 9.0f) - 1.0f, 0.0f, background_col);
				}
			}
		}
	}

	const auto armor_val = entity->get_armor_value();

	if (g_vars.get_as<bool>(V_ESP_WEAPON_ENABLED).value())
	{
		auto weapon = entity->get_active_weapon();

		if (weapon)
		{
			const auto col = color_t(V_ESP_WEAPON_COL, 255 * m_alpha[index]);

			auto offset = 4;
			switch (g_vars.get_as<int>(V_ESP_WEAPON_TYPE).value()) {
			case 0: {
				if (g_vars.get_as<bool>(V_ESP_ARMOR_ENABLED).value() && armor_val > m_armor_min)
					offset += 3;

				g_font.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_TEXT),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, g_font[Tahoma12px], TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			case 1: {
				if (g_vars.get_as<bool>(V_ESP_ARMOR_ENABLED).value() && armor_val > m_armor_min)
					offset += 5;

				g_font.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_ICON),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, g_font[Astriumwep12px], TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			}
		}
	}

	if (g_vars.get_as<bool>(V_ESP_ARMOR_ENABLED).value())
	{
		if (armor_val > m_armor_min)
		{
			g_render.draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 3, 4, background_col);
			g_render.draw_filled_rect(bbox.x, bbox.y + bbox.h + 4, ((bbox.w + 1) * armor_val) / 100, 2, color_t(V_ESP_ARMOR_COL, 255 * m_alpha[index]));
		}
	}

	if (g_vars.get_as<bool>(V_ESP_SKELETON_ENABLED).value())
	{
		const auto studio_model = g_csgo.m_model_info->get_studio_model(entity->get_model());

		if (studio_model)
		{
			vec3 v_parent{},
				 v_child{},
				 s_parent{},
				 s_child{};

			for (int i = 0; i < studio_model->bones_count; i++)
			{
				const auto bone = studio_model->bone(i);

				if (bone && (bone->flags & bone_used_by_hitbox) && (bone->parent != -1))
				{
					v_child = entity->get_bone_position(i);
					v_parent = entity->get_bone_position(bone->parent);

					if (Math::w2s(v_parent, s_parent) && Math::w2s(v_child, s_child))
					{
						switch (g_vars.get_as<int>(V_ESP_SKELETON_TYPE).value()) {
						case 0: {
							g_render.draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								1.0f, color_t(V_ESP_SKELETON_COL, 255 * m_alpha[index]));
							break;
						}
						case 1: {
							g_render.draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								1.0f, color_t(99, std::min(255, entity->get_health() * 225 / 100), 0, 255 * m_alpha[index])
							);
							break;
						}
						}
					}
				}
			}
		}
	}

	if (g_vars.get_as<bool>(V_ESP_BARREL_ENABLED).value())
	{
		vec3 start{},
			 s_screen{},
			 end{},
			 e_screen{},
			 forward{};

		Math::angle_vectors(entity->get_eye_angles(), forward);

		start = entity->get_hitbox_position(hitbox_head);
		end = start + (forward * 500.0f);

		if (Math::w2s(start, s_screen) && Math::w2s(end, e_screen))
		{
			g_render.draw_line(s_screen.x, s_screen.y, e_screen.x, e_screen.y,
				1.0f, color_t(V_ESP_BARREL_COL, 255 * m_alpha[index]));
		}
	}

	if (g_vars.get_as<bool>(V_ESP_SNAP_LINES_ENABLED).value())
	{
		vec3 pos{};

		if (Math::w2s(entity->get_hitbox_position(hitbox_head), pos))
		{
			Math::find_position_rotation(pos.x, pos.y, GLOBAL(screen_width), GLOBAL(screen_height));

			g_render.draw_line(GLOBAL(screen_width) / 2, GLOBAL(screen_height) / 2, pos.x, pos.y, 1.0f,
				color_t(V_ESP_SNAP_LINES_COL, 255 * m_alpha[index]));
		}
	}

	if (g_vars.get_as<bool>(V_ESP_FLAGS_ENABLED).value())
	{
		std::vector<std::pair<std::string, color_t>> flags, hk_flags;

		const auto col = color_t(V_ESP_FLAGS_COL, 255 * m_alpha[index]);

		if (g_vars.get_as<bool>(V_ESP_FLAGS_HK).value())
		{
			if (armor_val > m_armor_min)
			{
				flags.push_back(std::pair<std::string, color_t>(entity->has_helmet() ? "HK" : "K", col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_HEALTH).value())
		{
			const auto health = entity->get_health();

			if (health)
			{
				flags.push_back(std::pair<std::string, color_t>(std::to_string(health) + "HP", col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_MONEY).value())
		{
			const auto cash = entity->get_cash();

			if (cash)
			{
				flags.push_back(std::pair<std::string, color_t>(std::to_string(cash) + "$", col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_AMMO).value())
		{
			const auto weapon = entity->get_active_weapon();

			if (weapon)
			{
				auto weapon_first_ammo = weapon->clip1_count();
				auto weapon_second_ammo = weapon->primary_reserve_ammo_acount();

				if (Helpers::is_knife(weapon) || Helpers::is_taser(weapon) ||
					Helpers::is_c4(weapon) || Helpers::is_non_aim(weapon) ||
					Helpers::is_grenade(weapon))
					weapon_first_ammo = 0, weapon_second_ammo = 0;

				flags.push_back(std::pair<std::string, color_t>(std::to_string(weapon_first_ammo) + "/" + std::to_string(weapon_second_ammo), col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_FLASHED).value())
		{
			const auto flashed = entity->is_flashed();

			if (flashed)
			{
				flags.push_back(std::pair<std::string, color_t>("Flashed", col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_DEFUSING).value())
		{
			const auto defusing = entity->is_defusing();

			if (defusing)
			{
				flags.push_back(std::pair<std::string, color_t>("Defusing", col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_DISTANCE).value())
		{
			const auto dist = g_csgo.get_local()->get_vec_origin().distance_to(entity->get_vec_origin());

			if (dist)
			{
				char distance[256];
				sprintf_s(distance, "%im", static_cast<int>(dist));

				flags.push_back(std::pair<std::string, color_t>(distance, col));
			}
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_SCOPED).value())
		{
			const auto scoped = entity->is_scoped();

			if (scoped)
			{
				flags.push_back(std::pair<std::string, color_t>("Scoped", col));
			}
		}

		auto fl_pos = 0;
		for (const auto fl : flags) {
			g_font.draw_string(fl.first, bbox.x + bbox.w + 3, bbox.y + fl_pos - 2,
				g_font[Tahoma12px], TEXT_OUTLINE, fl.second);

			fl_pos += 10;
		}

		if (g_vars.get_as<bool>(V_ESP_FLAGS_ITEMS_ENABLED).value())
		{
			std::vector<std::pair<std::string, color_t>> inventory_item_flags, grenade_flags;

			const auto items_col = color_t(V_ESP_FLAGS_ITEMS_COL, 255 * m_alpha[index]);

			auto get_weapons = entity->get_my_weapons();
			for (int i = 0; get_weapons[i] != 0xffffffff; i++)
			{
				const auto weapon = g_csgo.m_entity_list->get_client_entity_handle<c_base_attributable_item*>(get_weapons[i]);

				switch (weapon->item_definition_index()) {
				case weapon_flashbang: {
					grenade_flags.push_back(std::pair<std::string, color_t>("i", items_col));
					break;
				}
				case weapon_smokegrenade: {
					grenade_flags.push_back(std::pair<std::string, color_t>("k", items_col));
					break;
				}
				case weapon_incgrenade: {
					grenade_flags.push_back(std::pair<std::string, color_t>("n", items_col));
					break;
				}
				case weapon_molotov: {
					grenade_flags.push_back(std::pair<std::string, color_t>("l", items_col));
					break;
				}
				case weapon_fraggrenade:
				case weapon_hegrenade: {
					grenade_flags.push_back(std::pair<std::string, color_t>("j", items_col));
					break;
				}
				case weapon_decoy: {
					grenade_flags.push_back(std::pair<std::string, color_t>("m", items_col));
					break;
				}
				case weapon_taser: {
					grenade_flags.push_back(std::pair<std::string, color_t>("h", items_col));
					break;
				}
				}
			}

			if (entity->get_team_num() == teamT)
			{
				if (entity->has_c4())
				{
					inventory_item_flags.push_back(std::pair<std::string, color_t>("o", items_col));
				}
			}

			if (entity->get_team_num() == teamCT)
			{
				if (entity->has_defuser())
				{
					inventory_item_flags.push_back(std::pair<std::string, color_t>("r", items_col));
				}
			}

			auto grenade_pos = 0;
			for (const auto fl : grenade_flags) {
				g_font.draw_string(fl.first, bbox.x + bbox.w + 3 + grenade_pos, bbox.y + bbox.h - 10,
					g_font[Astriumwep16px], TEXT_OUTLINE, fl.second);

				grenade_pos += 10;
			}

			auto inventory_item_pos = 0;
			for (const auto fl : inventory_item_flags) {
				auto offset = grenade_flags.empty() ? 10 : 23;

				g_font.draw_string(fl.first, bbox.x + bbox.w + 3 + inventory_item_pos, bbox.y + bbox.h - offset,
					g_font[Astriumwep16px], TEXT_OUTLINE, fl.second);

				inventory_item_pos += 18;
			}
		}
	}
}

void esp::reset_position(int index)
{
	m_has_seen[index] = false;
	m_anim_progress[index] = 0.0f;
}

void esp::update_position(int index, const vec3& pos)
{
	m_stored_pos[index] = pos;

	if (m_anim_progress[index] > 0.0f && m_anim_progress[index] <= 0.30f && m_has_seen[index])
		m_anim_progress[index] = 0.30f;
}