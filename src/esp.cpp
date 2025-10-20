#include "esp.h"

#include "globals.h"
#include "var_manager.h"
#include "helpers.h"
#include "render_manager.h"
#include "interfaces.h"
#include "math.h"

#include <array>

esp g_esp;

void esp::instance()
{
	if (!g_vars.get_as<bool>("esp->enabled").value())
		return;

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.m_local)
		return;

	for (int i = 0; i <= 65; i++)
	{
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity || entity->get_health() <= 0 || entity == g_csgo.m_local) {
			reset_position(i);
			continue;
		}

		if ((entity->get_team_num() == g_csgo.m_local->get_team_num()) & !g_vars.get_as<bool>("esp->team").value())
			continue;

		if (!g_csgo.m_local->can_see_entity(entity->get_eye_pos()) & g_vars.get_as<bool>("esp->visible_only").value())
			continue;

		if (!entity->is_moving() & g_vars.get_as<bool>("esp->walking_only").value())
			continue;

		float& am{ m_anim_progress[i] };
		m_alpha[i] = am;

		if (!can_draw_player(i, am, entity))
			continue;

		player_rendering(entity);
	}
}

void esp::on_create_move(i_user_cmd* cmd)
{
	if (!g_vars.get_as<bool>("esp->enabled").value())
		return;

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.m_local)
		return;

	if (g_vars.get_as<bool>("esp->crosshair->enabled").value())
	{
		auto cl_crosshairstyle = g_csgo.m_cvar->get_convar("cl_crosshairstyle");

		auto cl_crosshaircolor = g_csgo.m_cvar->get_convar("cl_crosshaircolor");
		auto cl_crosshaircolor_r = g_csgo.m_cvar->get_convar("cl_crosshaircolor_r");
		auto cl_crosshaircolor_g = g_csgo.m_cvar->get_convar("cl_crosshaircolor_g");
		auto cl_crosshaircolor_b = g_csgo.m_cvar->get_convar("cl_crosshaircolor_b");

		std::call_once(g::update_crosshair_col, [&]() {
			g_vars.set("esp->crosshair->col->r",
				cl_crosshaircolor_r ? cl_crosshaircolor_r->get_int() : 255);

			g_vars.set("esp->crosshair->col->g",
				cl_crosshaircolor_g ? cl_crosshaircolor_g->get_int() : 255);

			g_vars.set("esp->crosshair->col->b",
				cl_crosshaircolor_b ? cl_crosshaircolor_b->get_int() : 255);
		});

		static auto set_col = [&](std::array<int, 3> color) {
			if (cl_crosshairstyle->get_int() == 1)
				cl_crosshairstyle->set_value(5);

			if (cl_crosshaircolor->get_int() != 5)
				cl_crosshaircolor->set_value(5);

			cl_crosshaircolor_r->set_value(color[0]);
			cl_crosshaircolor_g->set_value(color[1]);
			cl_crosshaircolor_b->set_value(color[2]);
		};

		if (vec3 angle; const auto target = Helpers::find_target_entity(cmd, g_vars.get_as<float>("esp->crosshair->fov").value(), angle))
		{
			auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(target));

			if (!entity || entity->get_health() <= 0 || entity == g_csgo.m_local)
				return;

			if ((entity->get_team_num() == g_csgo.m_local->get_team_num()) & !g_vars.get_as<bool>("esp->team").value())
				return;

			if (!g_csgo.m_local->can_see_entity(entity->get_eye_pos()) & g_vars.get_as<bool>("esp->visible_only").value())
				return;

			set_col({
				g_vars.get_as<int>("esp->crosshair->col->on_target->r").value(),
				g_vars.get_as<int>("esp->crosshair->col->on_target->g").value(),
				g_vars.get_as<int>("esp->crosshair->col->on_target->b").value()
				});
		}
		else
		{
			set_col({
				g_vars.get_as<int>("esp->crosshair->col->r").value(),
				g_vars.get_as<int>("esp->crosshair->col->g").value(),
				g_vars.get_as<int>("esp->crosshair->col->b").value()
				});
		}
	}
}

void esp::on_do_post_screen_effects()
{
	if (!g_vars.get_as<bool>("glow->enabled").value())
		return;

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.m_local)
		return;

	for (int i = 0; i < g_csgo.m_glow_manager->size; i++)
	{
		auto& glow = g_csgo.m_glow_manager->objects[i];

		if (glow.next_free_slot != -2)
			continue;

		const auto glow_object = reinterpret_cast<c_base_player*>(glow.entity);

		if (!glow_object || glow_object->get_dormant())
			continue;

		if (!g_csgo.m_local->can_see_entity(glow_object->get_eye_pos()) & g_vars.get_as<bool>("glow->visible_only").value())
			continue;

		const auto client_class = glow_object->get_client_class();

		if (g_vars.get_as<bool>("glow->c4").value())
		{
			if (client_class->class_id == cc4 || client_class->class_id == cplantedc4)
			{
				glow.glow_color = vec3 {
					g_vars.get_as<int>("glow->c4->col->r").value() / 255.0f,
					g_vars.get_as<int>("glow->c4->col->g").value() / 255.0f,
					g_vars.get_as<int>("glow->c4->col->b").value() / 255.0f
				};
				glow.alpha = g_vars.get_as<int>("glow->c4->col->a").value() / 255.0f;
				glow.render_when_occluded = true;
				glow.render_when_unoccluded = false;
				glow.bloom_amount = 0.8f;
			}
		}

		if (!glow_object->is_moving() & g_vars.get_as<bool>("glow->walking_only").value())
			continue;

		if (client_class->class_id == ccsplayer)
		{
			const auto is_enemy = glow_object->get_team_num() != g_csgo.m_local->get_team_num();
			const auto is_teammate = glow_object->get_team_num() == g_csgo.m_local->get_team_num();

			float glow_enemy_col[4];
			float glow_team_col[4];

			if (g_vars.get_as<bool>("glow->health_based").value())
			{
				glow_enemy_col[0] = 99 / 255.0f;
				glow_enemy_col[1] = std::min(255, glow_object->get_health() * 225 / 100) / 255.0f;
				glow_enemy_col[2] = 0.0f;

				glow_team_col[0] = 99 / 255.0f;
				glow_team_col[1] = std::min(255, glow_object->get_health() * 225 / 100) / 255.0f;
				glow_team_col[2] = 0.0f;
			}
			else
			{
				glow_enemy_col[0] = g_vars.get_as<int>("glow->enemy->col->r").value() / 255.0f;
				glow_enemy_col[1] = g_vars.get_as<int>("glow->enemy->col->g").value() / 255.0f;
				glow_enemy_col[2] = g_vars.get_as<int>("glow->enemy->col->b").value() / 255.0f;

				glow_team_col[0] = g_vars.get_as<int>("glow->team->col->r").value() / 255.0f;
				glow_team_col[1] = g_vars.get_as<int>("glow->team->col->g").value() / 255.0f;
				glow_team_col[2] = g_vars.get_as<int>("glow->team->col->b").value() / 255.0f;
			}

			glow_enemy_col[3] = g_vars.get_as<int>("glow->enemy->col->a").value() / 255.0f;
			glow_team_col[3] = g_vars.get_as<int>("glow->team->col->a").value() / 255.0f;

			if (is_enemy) {
				glow.glow_color = vec3{ glow_enemy_col[0], glow_enemy_col[1], glow_enemy_col[2] };
				glow.alpha = glow_enemy_col[3];
				glow.render_when_occluded = true;
				glow.render_when_unoccluded = false;
				glow.bloom_amount = 0.8f;
			}

			if (is_teammate && (g_vars.get_as<bool>("glow->team").value())) {
				glow.glow_color = vec3{ glow_team_col[0], glow_team_col[1], glow_team_col[2] };
				glow.alpha = glow_team_col[3];
				glow.render_when_occluded = true;
				glow.render_when_unoccluded = false;
				glow.bloom_amount = 0.8f;
			}
		}
	}
}

void esp::on_scene_end()
{
	if (!g_vars.get_as<bool>("chams->enabled").value())
		return;

	if (!g_csgo.m_engine->is_connected())
		return;

	if (!g_csgo.m_local)
		return;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity == g_csgo.m_local)
			continue;

		if (entity->get_health() <= 0)
			continue;

		if (entity->get_team_num() == g_csgo.m_local->get_team_num() & !g_vars.get_as<bool>("chams->team").value())
			continue;

		if (!entity->is_moving() & g_vars.get_as<bool>("chams->walking_only").value())
			continue;

		const auto debugambientcube = g_csgo.m_mat_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
		const auto flat = g_csgo.m_mat_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
		const auto dogtags_outline = g_csgo.m_mat_system->find_material("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);

		debugambientcube->increment_reference_count();
		flat->increment_reference_count();
		dogtags_outline->increment_reference_count();

		c_material* material{};

		switch (g_vars.get_as<int>("chams->type").value()) {
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

		float chams_col[3];

		if (g_vars.get_as<bool>("chams->health_based").value())
		{
			chams_col[0] = 99 / 255.0f;
			chams_col[1] = std::min(255, entity->get_health() * 225 / 100) / 255.0f;
			chams_col[2] = 0.0f;
		}
		else
		{
			chams_col[0] = g_vars.get_as<int>("chams->col->r").value() / 255.0f;
			chams_col[1] = g_vars.get_as<int>("chams->col->g").value() / 255.0f;
			chams_col[2] = g_vars.get_as<int>("chams->col->b").value() / 255.0f;
		}

		g_csgo.m_render_view->modulate_color(chams_col);
		g_csgo.m_render_view->set_blend(1.0f);

		material->set_material_var_flag(material_var_ignorez, !g_vars.get_as<bool>("chams->visible_only").value());

		g_csgo.m_model_render->override_material(material);
		entity->draw_model(1, 255);

		g_csgo.m_model_render->override_material(nullptr);
	}
}

void esp::on_round_start()
{
	for (int i = 0; i <= 65; i++) {
		reset_position(i);
	}
}

bool esp::can_draw_player(int index, float& anim, c_base_player* entity)
{
	float rate = g_csgo.m_globals->frame_time * 1.0f / 0.5f;

	if (!entity->get_dormant()) {
		update_position(index, entity->get_absolute_origin());

		anim = anim > 0.0f ? std::clamp(anim + rate, 0.0f, 1.0f) : 0.5f;
		m_has_seen[index] = true;
	}
	else {
		if (anim < 0.30f && !(entity->get_vec_origin().distance_to(g_csgo.m_local->get_vec_origin()) > 1499.0f)) {
			rate *= 0.02f;
		}

		anim = std::clamp(anim -= rate, 0.0f, 1.0f);

		if (m_anim_progress[index] <= 0.0f) {
			m_has_seen[index] = false;
			return false;
		}
	}

	return true;
}

void esp::player_rendering(c_base_player* entity)
{
	if (!g_csgo.m_local)
		return;

	box bbox{};
	if (!Helpers::get_bbox(entity, bbox, static_cast<bbox_type>(g_vars.get_as<int>("esp->render_type").value())))
		return;

	if (g_vars.get_as<bool>("esp->name->enabled").value())
	{
		player_info_t info{};
		g_csgo.m_engine->get_player_info(entity->index(), &info);

		auto player_name = Helpers::stws(std::string{ info.m_player_name });

		switch (g_vars.get_as<int>("esp->name->type").value()) {
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

		g_render.draw_stringW(player_name, bbox.x + (bbox.w / 2), bbox.y - 14,
			g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, color_t("esp->name->col", 255 * m_alpha[entity->index()]));
	}

	const auto background_col = color_t(3, 3, 3, 255 * m_alpha[entity->index()]);

	if (g_vars.get_as<bool>("esp->box->enabled").value())
	{
		const auto col = color_t("esp->box->col", 255 * m_alpha[entity->index()]);
		const auto outline_col = color_t("esp->box->col", 30 * m_alpha[entity->index()]);

		switch (g_vars.get_as<int>("esp->box->type").value()) {
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

	if (g_vars.get_as<bool>("esp->health->enabled").value())
	{
		const auto hp = entity->get_health() > 100 ? 100 : entity->get_health();

		if (hp)
		{
			const auto pixel_value = hp * (bbox.h + 1) / 100;

			switch (g_vars.get_as<int>("esp->health->type").value()) {
			case 0: {
				const auto col = color_t("esp->health->col", 255 * m_alpha[entity->index()]);

				g_render.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_render.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value, col);
				break;
			}
			case 1: {
				g_render.draw_filled_rect(bbox.x - 6, bbox.y - 1, 4, bbox.h + 3, background_col);
				g_render.draw_filled_rect(bbox.x - 5, bbox.y + (bbox.h + 1) - pixel_value, 2, pixel_value,
					color_t(99, std::min(255, hp * 225 / 100), 0, 255 * m_alpha[entity->index()]));
				break;
			}
			}

			if (g_vars.get_as<bool>("esp->health->battery").value())
			{
				for (int i = 0; i < 9; i++) {
					g_render.draw_line(bbox.x - 5, bbox.y + i * ((bbox.h + 1) / 9.0f) - 0.3f, bbox.x - 3,
						bbox.y + i * ((bbox.h + 1) / 9.0f), 0.0f, background_col);
				}
			}
		}
	}

	const auto armor_val = entity->get_armor_value();

	if (g_vars.get_as<bool>("esp->weapon->enabled").value())
	{
		auto weapon = entity->get_active_weapon();

		if (weapon)
		{
			const auto col = color_t("esp->weapon->col", 255 * m_alpha[entity->index()]);

			auto offset = 3;
			switch (g_vars.get_as<int>("esp->weapon->type").value()) {
			case 0: {
				if (g_vars.get_as<bool>("esp->armor->enabled").value() && armor_val > m_armor_min)
					offset += 3;

				g_render.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), we_text),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, g_render.get_font(Tahoma12px), TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			case 1: {
				if (g_vars.get_as<bool>("esp->armor->enabled").value() && armor_val > m_armor_min)
					offset += 4;

				g_render.draw_string(Helpers::get_weapon_type_by_index(weapon->item_definition_index(), we_icon),
					bbox.x + (bbox.w / 2), bbox.h + bbox.y + offset, g_render.get_font(Astriumwep16px), TEXT_OUTLINE | TEXT_CENTER_X, col);
				break;
			}
			}
		}
	}

	if (g_vars.get_as<bool>("esp->armor->enabled").value())
	{
		if (armor_val > m_armor_min)
		{
			g_render.draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 3, 4, background_col);
			g_render.draw_filled_rect(bbox.x, bbox.y + bbox.h + 4, ((bbox.w + 1) * armor_val) / 100, 2, color_t("esp->armor->col", 255 * m_alpha[entity->index()]));
		}
	}

	if (g_vars.get_as<bool>("esp->skeleton->enabled").value())
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
						switch (g_vars.get_as<int>("esp->skeleton->type").value()) {
						case 0: {
							g_render.draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								1.0f, color_t("esp->skeleton->col", 255 * m_alpha[entity->index()]));
							break;
						}
						case 1: {
							g_render.draw_line(s_parent[0], s_parent[1], s_child[0], s_child[1],
								1.0f, color_t(99, std::min(255, entity->get_health() * 225 / 100), 0, 255 * m_alpha[entity->index()])
							);
							break;
						}
						}
					}
				}
			}
		}
	}

	if (g_vars.get_as<bool>("esp->barrel->enabled").value())
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
				1.0f, color_t("esp->barrel->col", 255 * m_alpha[entity->index()]));
		}
	}

	if (g_vars.get_as<bool>("esp->offscreen_lines->enabled").value())
	{
		vec3 pos{};

		if (Math::w2s(entity->get_hitbox_position(hitbox_head), pos))
		{
			Math::find_position_rotation(pos.x, pos.y, g::screen_width, g::screen_height);

			g_render.draw_line(g::screen_width / 2, g::screen_height / 2, pos.x, pos.y, 1.0f,
				color_t("esp->offscreen_lines->col", 255 * m_alpha[entity->index()]));
		}
	}

	if (g_vars.get_as<bool>("esp->flags->enabled").value())
	{
		std::vector<std::pair<std::string, color_t>> flags, hk_flags;

		const auto col = color_t("esp->flags->col", 255 * m_alpha[entity->index()]);

		if (g_vars.get_as<bool>("esp->flags->hk").value())
		{
			if (armor_val > m_armor_min)
			{
				flags.push_back(std::pair<std::string, color_t>(entity->has_helmet() ? "HK" : "K", col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->health").value())
		{
			const auto health = entity->get_health();

			if (health)
			{
				flags.push_back(std::pair<std::string, color_t>(std::to_string(health) + "HP", col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->money").value())
		{
			const auto cash = entity->get_cash();

			if (cash)
			{
				flags.push_back(std::pair<std::string, color_t>(std::to_string(cash) + "$", col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->ammo").value())
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

		if (g_vars.get_as<bool>("esp->flags->flashed").value())
		{
			const auto flashed = entity->is_flashed();

			if (flashed)
			{
				flags.push_back(std::pair<std::string, color_t>("Flashed", col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->defusing").value())
		{
			const auto defusing = entity->is_defusing();

			if (defusing)
			{
				flags.push_back(std::pair<std::string, color_t>("Defusing", col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->distance").value())
		{
			const auto dist = g_csgo.m_local->get_vec_origin().distance_to(entity->get_vec_origin());

			if (dist)
			{
				char distance[256];
				sprintf_s(distance, "%im", static_cast<int>(dist));

				flags.push_back(std::pair<std::string, color_t>(distance, col));
			}
		}

		if (g_vars.get_as<bool>("esp->flags->scoped").value())
		{
			const auto scoped = entity->is_scoped();

			if (scoped)
			{
				flags.push_back(std::pair<std::string, color_t>("Scoped", col));
			}
		}

		auto fl_pos = 0;
		for (const auto fl : flags) {
			g_render.draw_string(fl.first, bbox.x + bbox.w + 3, bbox.y + fl_pos - 2,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE, fl.second);

			fl_pos += 10;
		}

		if (g_vars.get_as<bool>("esp->flags->items->enabled").value())
		{
			std::vector<std::pair<std::string, color_t>> inventory_item_flags, grenade_flags;

			const auto items_col = color_t("esp->flags->items->col", 255 * m_alpha[entity->index()]);

			auto get_weapons = entity->get_my_weapons();
			for (int i = 0; get_weapons[i] != 0xffffffff; i++)
			{
				const auto weapon = reinterpret_cast<c_base_attributable_item*>(g_csgo.m_entity_list->get_client_entity_handle(get_weapons[i]));

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
				g_render.draw_string(fl.first, bbox.x + bbox.w + 3 + grenade_pos, bbox.y + bbox.h - 10,
					g_render.get_font(Astriumwep16px), TEXT_OUTLINE, fl.second);

				grenade_pos += 10;
			}

			auto inventory_item_pos = 0;
			for (const auto fl : inventory_item_flags) {
				auto offset = grenade_flags.empty() ? 10 : 23;

				g_render.draw_string(fl.first, bbox.x + bbox.w + 3 + inventory_item_pos, bbox.y + bbox.h - offset,
					g_render.get_font(Astriumwep16px), TEXT_OUTLINE, fl.second);

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

	if (m_anim_progress[index] > 0.0f && m_anim_progress[index] <= 0.30f && m_has_seen[index]) {
		m_anim_progress[index] = 0.30f;
	}
}