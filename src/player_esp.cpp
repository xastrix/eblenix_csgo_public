#include "visuals.h"

#include "vars.h"
#include "interfaces.h"
#include "renderer.h"
#include "math.h"

void player_esp_t::think()
{
	box bbox;

	if (!g_var->get_as<bool>(V_ESP_ENABLED).value())
		return;

	for (int i = 0; i <= MAX_PLAYERS; i++)
	{
		auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

		if (!entity || entity->get_health() <= 0 || entity == g_cs->get_local()) {
			reset_position(i);
			continue;
		}

		if ((entity->get_team_num() == g_cs->get_local()->get_team_num()) & !g_var->get_as<bool>(V_ESP_TEAM).value())
			continue;

		if (!g_cs->get_local()->can_see_entity(entity->get_eye_pos()) & g_var->get_as<bool>(V_ESP_VISIBLE_ONLY).value())
			continue;

		if (!entity->is_moving() & g_var->get_as<bool>(V_ESP_WALKING_ONLY).value())
			continue;

		float& am{ m_anim_progress[i] };
		m_alpha[i] = am;

		calc_player_animation_progress(i, am, entity);

		if (Helpers::get_bbox(entity, bbox, static_cast<_bbox_types>(g_var->get_as<int>(V_ESP_RENDER_TYPE).value())))
			player_rendering(i, entity, bbox);
	}
}

void player_esp_t::on_round_start_e()
{
	for (int i = 0; i <= MAX_PLAYERS; i++) {
		reset_position(i);
	}
}

void player_esp_t::calc_player_animation_progress(int index, float& anim, c_base_player* entity)
{
	float rate = g_cs->m_globals->frame_time * 1.0f / 0.5f;
	auto& begin_anim = m_begin_anims[index];

	if (!entity->get_dormant()) {
		update_position(index, entity->get_absolute_origin());

		anim = anim > 0.0f ? std::clamp(anim + rate, 0.0f, 1.0f) : 0.5f;
		m_has_seen[index] = true;
	}
	else {
		if (anim < 0.30f && !(entity->get_vec_origin().distance_to(g_cs->get_local()->get_vec_origin()) > MAX_PLAYER_RENDERING_DISTANCE)) {
			rate *= 0.02f;
		}

		anim = std::clamp(anim -= rate, 0.0f, 1.0f);

		if (m_anim_progress[index] <= 0.0f) {
			m_has_seen[index] = false;
		}
	}

	if (!begin_anim.m_confirmed) {
		begin_anim.m_elapsed += rate;
		float t = begin_anim.m_elapsed / 0.5f;

		if (t > 1.0f) {
			t = 1.0f;
			begin_anim.m_confirmed = true;
		}

		begin_anim.m_final_val = t * 100.0f;
	}
}

void player_esp_t::player_rendering(int index, c_base_player* entity, box bbox)
{
	const auto health = entity->get_health();
	auto       armor  = entity->get_armor_value();
	const auto weapon = entity->get_active_weapon();

	const auto background_col = color_t(3, 3, 3, 255 * m_alpha[index]);

	if (g_var->get_as<bool>(V_ESP_NAME_ENABLED).value())
	{
		player_info_t info;
		g_cs->m_engine->get_player_info(index, &info);

		auto player_name = Helpers::stws(std::string{ info.m_player_name });

		switch (g_var->get_as<int>(V_ESP_NAME_TYPE).value()) {
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

		g_font->draw_stringW(player_name, bbox.x + (bbox.w / 2), bbox.y - 14,
			FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, color_t(V_ESP_NAME_COL, 255 * m_alpha[index]));
	}

	if (g_var->get_as<bool>(V_ESP_BOX_ENABLED).value())
	{
		const auto col = color_t(V_ESP_BOX_COL, 255 * m_alpha[index]);
		const auto outline_col = color_t(V_ESP_BOX_COL, 30 * m_alpha[index]);

		switch (g_var->get_as<int>(V_ESP_BOX_TYPE).value()) {
		case 0: {
			g_renderer->line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, background_col);
			g_renderer->line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, background_col);
			g_renderer->line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, background_col);
			g_renderer->line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, background_col);

			g_renderer->line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, col);
			g_renderer->line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, col);
			g_renderer->line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, col);
			g_renderer->line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, col);
			break;
		}
		case 1: {
			g_renderer->line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, background_col);
			g_renderer->line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, background_col);
			g_renderer->line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, background_col);
			g_renderer->line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, background_col);

			g_renderer->rect_fill(bbox.x, bbox.y, bbox.w, bbox.h - 1, outline_col);

			g_renderer->line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, col);
			g_renderer->line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, col);
			g_renderer->line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, col);
			g_renderer->line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, col);
			break;
		}
		case 2: {
			g_renderer->corner_box(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, 3, 5, background_col);
			g_renderer->corner_box(bbox.x, bbox.y, bbox.w, bbox.h, 3, 5, col);
			break;
		}
		case 3: {
			g_renderer->rect_fill(bbox.x, bbox.y, bbox.w, bbox.h - 1, outline_col);

			g_renderer->corner_box(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, 3, 5, background_col);
			g_renderer->corner_box(bbox.x, bbox.y, bbox.w, bbox.h, 3, 5, col);
			break;
		}
		}
	}

	if (g_var->get_as<bool>(V_ESP_HEALTH_ENABLED).value())
	{
		const auto max_hp = 100;
		const auto inner_hp = std::min(health, max_hp);
		const auto hp = m_begin_anims[index].m_confirmed ? inner_hp : m_begin_anims[index].m_final_val;

		if (hp)
		{
			const auto pixel_value = hp * (bbox.h + 1) / max_hp;

			switch (g_var->get_as<int>(V_ESP_HEALTH_TYPE).value()) {
			case 0: {
				g_renderer->rect_fill(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_renderer->rect_fill(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t(V_ESP_HEALTH_COL, 255 * m_alpha[index]));
				break;
			}
			case 1: {
				g_renderer->rect_fill(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_renderer->rect_fill(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t::calc_health_color(hp, 255 * m_alpha[index]));
				break;
			}
			case 2: {
				g_renderer->rect_fill(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_renderer->gradient_v(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t(V_ESP_HEALTH_GRADIENT_COL, 255 * m_alpha[index]),
					color_t(V_ESP_HEALTH_GRADIENT_COL2, 255 * m_alpha[index]));
				break;
			}
			}

			if (g_var->get_as<bool>(V_ESP_HEALTH_BATTERY).value())
			{
				int line_count = 9;
				for (int i = 0; i < line_count; i++) {
					int y = bbox.y - 1 + (i * bbox.h + 3) / line_count;
					g_renderer->line(bbox.x - 6, y, bbox.x - 3, y, background_col);
				}
			}
		}
	}

	if (g_var->get_as<bool>(V_ESP_WEAPON_ENABLED).value())
	{
		if (weapon)
		{
			const auto col = color_t(V_ESP_WEAPON_COL, 255 * m_alpha[index]);

			auto offset = 4;
			switch (g_var->get_as<int>(V_ESP_WEAPON_TYPE).value()) {
			case 0: {
				if (g_var->get_as<bool>(V_ESP_ARMOR_ENABLED).value() && armor > ARMOR_MIN_VAL)
					offset += 3;

				g_font->draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_TEXT),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, FONT(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			case 1: {
				if (g_var->get_as<bool>(V_ESP_ARMOR_ENABLED).value() && armor > ARMOR_MIN_VAL)
					offset += 5;

				g_font->draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), WE_ICON),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, FONT(Astriumwep12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			}
		}
	}

	if (g_var->get_as<bool>(V_ESP_ARMOR_ENABLED).value())
	{
		armor = m_begin_anims[index].m_confirmed ? armor : m_begin_anims[index].m_final_val;

		if (armor > ARMOR_MIN_VAL)
		{
			switch (g_var->get_as<int>(V_ESP_ARMOR_TYPE).value()) {
			case 0: {
				g_renderer->rect_fill(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 3, 4, background_col);
				g_renderer->rect_fill(bbox.x, bbox.y + bbox.h + 4, ((bbox.w + 1) * armor) / 100, 2,
					color_t(V_ESP_ARMOR_COL, 255 * m_alpha[index]));
				break;
			}
			case 1: {
				g_renderer->rect_fill(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 3, 4, background_col);
				g_renderer->gradient_h(bbox.x, bbox.y + bbox.h + 4, ((bbox.w + 1) * armor) / 100, 2,
					color_t(V_ESP_ARMOR_GRADIENT_COL, 255 * m_alpha[index]),
					color_t(V_ESP_ARMOR_GRADIENT_COL2, 255 * m_alpha[index]));
				break;
			}
			}

			if (g_var->get_as<bool>(V_ESP_ARMOR_BATTERY).value())
			{
				int start_y = bbox.y + bbox.h + 3;
				int line_count = 5;
				for (int i = 0; i < line_count; i++) {
					int x = bbox.x - 1 + (i * bbox.w + 3) / line_count;
					g_renderer->line(x, start_y, x, start_y + 3, background_col);
				}
			}
		}
	}

	if (g_var->get_as<bool>(V_ESP_SKELETON_ENABLED).value())
	{
		const auto studio_model = g_cs->m_model_info->get_studio_model(entity->get_model());

		if (studio_model)
		{
			vec3 v_parent{},
				v_child{},
				s_parent{},
				s_child{};

			for (int i = 0; i < studio_model->m_bones_count; i++)
			{
				const auto bone = studio_model->bone(i);

				if (bone && (bone->m_flags & bone_used_by_hitbox) && (bone->m_parent != -1))
				{
					v_child = entity->get_bone_position(i);
					v_parent = entity->get_bone_position(bone->m_parent);

					if (Math::w2s(v_parent, s_parent) && Math::w2s(v_child, s_child))
					{
						switch (g_var->get_as<int>(V_ESP_SKELETON_TYPE).value()) {
						case 0: {
							g_renderer->line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								color_t(V_ESP_SKELETON_COL, 255 * m_alpha[index]));
							break;
						}
						case 1: {
							g_renderer->line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								color_t::calc_health_color(entity->get_health(), 255 * m_alpha[index])
							);
							break;
						}
						}
					}
				}
			}
		}
	}

	if (g_var->get_as<bool>(V_ESP_BARREL_ENABLED).value())
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
			g_renderer->line(s_screen.x, s_screen.y, e_screen.x, e_screen.y,
				color_t(V_ESP_BARREL_COL, 255 * m_alpha[index]));
		}
	}

	if (g_var->get_as<bool>(V_ESP_SNAP_LINES_ENABLED).value())
	{
		vec3 pos{};

		if (Math::w2s(entity->get_hitbox_position(hitbox_head), pos))
		{
			vec2 screen_size = g_renderer->get_screen_size();

			Math::find_position_rotation(pos.x, pos.y, screen_size.x, screen_size.y);

			g_renderer->line(screen_size.x / 2, screen_size.y / 2, pos.x, pos.y,
				color_t(V_ESP_SNAP_LINES_COL, 255 * m_alpha[index]));
		}
	}

	if (g_var->get_as<bool>(V_ESP_FLAGS_ENABLED).value())
	{
		std::vector<std::pair<std::string, color_t>> flags;

		const auto col = color_t(V_ESP_FLAGS_COL, 255 * m_alpha[index]);

		if (g_var->get_as<bool>(V_ESP_FLAGS_HK).value())
		{
			if (armor > ARMOR_MIN_VAL)
				flags.push_back(std::pair<std::string, color_t>(entity->has_helmet() ? "HK" : "K", col));
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_HEALTH).value())
		{
			if (health)
				flags.push_back(std::pair<std::string, color_t>(std::to_string(health) + "HP", col));
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_MONEY).value())
		{
			const auto cash = entity->get_cash();

			if (cash)
				flags.push_back(std::pair<std::string, color_t>(std::to_string(cash) + "$", col));
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_AMMO).value())
		{
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

		if (g_var->get_as<bool>(V_ESP_FLAGS_FLASHED).value())
		{
			const auto flashed = entity->is_flashed();

			if (flashed)
				flags.push_back(std::pair<std::string, color_t>("Flashed", col));
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_DEFUSING).value())
		{
			const auto defusing = entity->is_defusing();

			if (defusing)
				flags.push_back(std::pair<std::string, color_t>("Defusing", col));
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_DISTANCE).value())
		{
			const auto dist = g_cs->get_local()->get_vec_origin().distance_to(entity->get_vec_origin());

			if (dist)
			{
				char distance[256];
				sprintf_s(distance, "%im", static_cast<int>(dist));

				flags.push_back(std::pair<std::string, color_t>(distance, col));
			}
		}

		if (g_var->get_as<bool>(V_ESP_FLAGS_SCOPED).value())
		{
			const auto scoped = entity->is_scoped();

			if (scoped)
				flags.push_back(std::pair<std::string, color_t>("Scoped", col));
		}

		auto fl_pos = 0;
		for (const auto fl : flags) {
			g_font->draw_string(fl.first, bbox.x + bbox.w + 3, bbox.y + fl_pos,
				FONT(SmallFonts10px), TEXT_OUTLINE, fl.second);

			fl_pos += 8;
		}
	}
}

void player_esp_t::reset_position(int index)
{
	m_has_seen[index] = false;
	m_anim_progress[index] = 0.0f;

	m_begin_anims[index].m_confirmed = false;
	m_begin_anims[index].m_elapsed = 0.0f;
}

void player_esp_t::update_position(int index, const vec3& pos)
{
	m_stored_pos[index] = pos;

	if (m_anim_progress[index] > 0.0f && m_anim_progress[index] <= 0.30f && m_has_seen[index])
		m_anim_progress[index] = 0.30f;
}