#include "hooks.h"

static create_move_fn o_create_move{};
static paint_traverse_fn o_paint_traverse{};
static present_fn o_present{};
static reset_fn o_reset{};
static do_post_screen_effects_fn o_do_post_screen_effects{};
static scene_end_fn o_scene_end{};
static screen_viewmodel_fov_change_fn o_screen_viewmodel_fov_change{};
static get_screen_aspect_ratio_fn o_get_screen_aspect_ratio{};
static draw_model_execute_fn o_draw_model_execute{};
static override_view_fn o_override_view{};
static is_connected_fn o_is_connected{};
static list_in_leaves_box_fn o_list_in_leaves_box{};
static sv_cheats_boolean_fn o_sv_cheats_boolean{};
static shutdown_fn o_shutdown{};

static bool __stdcall create_move_h(float input_sample_frametime, i_user_cmd* cmd)
{
	auto ret = o_create_move(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return ret;

	g_csgo.set_local(
		reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(g_csgo.m_engine->get_local_player()))
	);

	if (GLOBAL(initialised))
	{
		if (!GLOBAL(panic))
		{
			g_movement.run(cmd);
			g_aimbot.run(cmd);
			g_triggerbot.run(cmd);
			g_knifebot.run(cmd);

			g_esp.on_create_move(cmd);

			if (g_csgo.m_engine->is_connected())
			{
				if (g_vars.get_as<bool>(V_MISC_VISUAL_REVEAL_RANKS).value())
				{
					if (cmd->buttons & in_score)
						g_csgo.m_client->dispatch_user_message(cs_um_serverrankrevealall);
				}
			}
		}

		Math::normalize_angles(cmd->viewangles);
		Math::clamp_angles(cmd->viewangles);

		return false;
	}

	return ret;
}

static void __stdcall paint_traverse_h(unsigned int panel, bool force_repaint, bool allow_force)
{
	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		/* This is necessary so that when using the open console or chat window,
		   the menu cannot be used.
		*/
		GLOBAL(console_opened) = g_csgo.m_engine->is_console_visible();
		GLOBAL(chat_opened) = g_csgo.m_client->is_chat_raised();

		if (g_csgo.m_engine->is_connected())
		{
			if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
				g_vars.get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
			{
				if (!GLOBAL(visuals_nightmode_state))
				{
					auto world_brightness = g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS).value();
					auto sky_brightness = g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS).value();
					auto models_brightness = g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS).value();
					auto decals_brightness = g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS).value();
					auto others_brightness = g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS).value();

					for (auto i = g_csgo.m_mat_system->first_material();
						i != g_csgo.m_mat_system->invalid_material_handle(); i = g_csgo.m_mat_system->next_material(i)) {
						auto material = g_csgo.m_mat_system->get_material(i);

						if (!material)
							continue;

						if (std::string{ material->get_texture_group_name() }.find("World") != std::string::npos)
							material->color_modulate(world_brightness, world_brightness, world_brightness);

						if (std::string{ material->get_texture_group_name() }.find("SkyBox") != std::string::npos)
							material->color_modulate(sky_brightness, sky_brightness, sky_brightness);

						if (std::string{ material->get_texture_group_name() }.find("Model") != std::string::npos)
							material->color_modulate(models_brightness, models_brightness, models_brightness);

						if (std::string{ material->get_texture_group_name() }.find("Decal") != std::string::npos)
							material->color_modulate(decals_brightness, decals_brightness, decals_brightness);

						if (std::string{ material->get_texture_group_name() }.find("Other") != std::string::npos)
							material->color_modulate(others_brightness, others_brightness, others_brightness);
					}

					GLOBAL(visuals_nightmode_state) = true;
				}
			}
			else
			{
				if (GLOBAL(visuals_nightmode_state))
				{
					for (auto i = g_csgo.m_mat_system->first_material();
						i != g_csgo.m_mat_system->invalid_material_handle(); i = g_csgo.m_mat_system->next_material(i)) {
						auto material = g_csgo.m_mat_system->get_material(i);

						if (!material)
							continue;

						if (std::string{ material->get_texture_group_name() }.find("World") != std::string::npos)
							material->color_modulate(/* 1.0f */);

						if (std::string{ material->get_texture_group_name() }.find("SkyBox") != std::string::npos)
							material->color_modulate(/* 1.0f */);

						if (std::string{ material->get_texture_group_name() }.find("Model") != std::string::npos)
							material->color_modulate(/* 1.0f */);

						if (std::string{ material->get_texture_group_name() }.find("Decal") != std::string::npos)
							material->color_modulate(/* 1.0f */);

						if (std::string{ material->get_texture_group_name() }.find("Other") != std::string::npos)
							material->color_modulate(/* 1.0f */);
					}

					GLOBAL(visuals_nightmode_state) = false;
				}
			}
		}

		switch (fnv::hash(g_csgo.m_panel->get_panel_name(panel))) {
		case fnv::hash("HudZoom"): {
			if (g_csgo.m_engine->is_connected()) {
				if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
					g_vars.get_as<bool>(V_VISUALS_REMOVALS_SCOPE).value())
					return;
			}
			break;
		}
		}
	}

	return o_paint_traverse(g_csgo.m_panel, panel, force_repaint, allow_force);
}

static long D3DAPI present_h(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	std::call_once(GLOBAL(init_render_stuff), [device]() {
		if (g_render.init(device))
			g_ui.init(device);
	});

	g_render.begin_render_states();

	switch (GLOBAL(status)) {
	case gameVersionOK: {
		if (!GLOBAL(panic)) {
			g_esp.run();
			g_visuals.run();
			g_ui.run();
		}
		break;
	}
#ifndef DISABLE_CSGO_VERSION_CHECK
	case gameVersionOutdated: {
		std::ostringstream oss{};
		oss << "Hack has not been updated [" << g_csgo.m_engine->get_product_version_string() << "]";

		g_render.draw_status(oss.str(), color_t(164, 164, 164));
		break;
	}
#endif
	}

	g_render.end_render_states();

	return o_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

static long D3DAPI reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	g_ui.on_reset_sprites();
	g_render.undo();

	auto ret = o_reset(device, present_parameters);

	g_render.create_objects(device);
	g_ui.on_reset_end_sprites();

	return ret;
}

static int __stdcall do_post_screen_effects_h(int v)
{
	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_vars.get_as<bool>(V_GLOW_ENABLED).value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				if (g_csgo.get_local())
				{
					for (int i = 0; i < g_csgo.m_glow_manager->size; i++)
					{
						auto& glow = g_csgo.m_glow_manager->objects[i];

						if (glow.next_free_slot != -2)
							continue;

						const auto glow_object = reinterpret_cast<c_base_player*>(glow.entity);

						if (!glow_object || glow_object->get_dormant())
							continue;

						if (!g_csgo.get_local()->can_see_entity(glow_object->get_eye_pos()) & g_vars.get_as<bool>(V_GLOW_VISIBLE_ONLY).value())
							continue;

						const auto client_class = glow_object->get_client_class();

						if (g_vars.get_as<bool>(V_GLOW_C4).value())
						{
							if (client_class->class_id == cc4 || client_class->class_id == cplantedc4)
							{
								glow.glow_color = vec3 {
									g_vars.get_as<int>(V_GLOW_C4_COL_R).value() / 255.0f,
									g_vars.get_as<int>(V_GLOW_C4_COL_G).value() / 255.0f,
									g_vars.get_as<int>(V_GLOW_C4_COL_B).value() / 255.0f
								};
								glow.alpha = g_vars.get_as<int>(V_GLOW_C4_COL_A).value() / 255.0f;
								glow.render_when_occluded = true;
								glow.render_when_unoccluded = false;
								glow.bloom_amount = 0.8f;
							}
						}

						if (!glow_object->is_moving() & g_vars.get_as<bool>(V_GLOW_WALKING_ONLY).value())
							continue;

						if (client_class->class_id == ccsplayer)
						{
							const auto is_enemy = glow_object->get_team_num() != g_csgo.get_local()->get_team_num();
							const auto is_teammate = glow_object->get_team_num() == g_csgo.get_local()->get_team_num();

							float glow_enemy_col[4];
							float glow_team_col[4];

							if (g_vars.get_as<bool>(V_GLOW_HEALTH_BASED).value())
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
								glow_enemy_col[0] = g_vars.get_as<int>(V_GLOW_ENEMY_COL_R).value() / 255.0f;
								glow_enemy_col[1] = g_vars.get_as<int>(V_GLOW_ENEMY_COL_G).value() / 255.0f;
								glow_enemy_col[2] = g_vars.get_as<int>(V_GLOW_ENEMY_COL_B).value() / 255.0f;

								glow_team_col[0] = g_vars.get_as<int>(V_GLOW_TEAM_COL_R).value() / 255.0f;
								glow_team_col[1] = g_vars.get_as<int>(V_GLOW_TEAM_COL_G).value() / 255.0f;
								glow_team_col[2] = g_vars.get_as<int>(V_GLOW_TEAM_COL_B).value() / 255.0f;
							}

							glow_enemy_col[3] = g_vars.get_as<int>(V_GLOW_ENEMY_COL_A).value() / 255.0f;
							glow_team_col[3] = g_vars.get_as<int>(V_GLOW_TEAM_COL_A).value() / 255.0f;

							if (is_enemy) {
								glow.glow_color = vec3{ glow_enemy_col[0], glow_enemy_col[1], glow_enemy_col[2] };
								glow.alpha = glow_enemy_col[3];
								glow.render_when_occluded = true;
								glow.render_when_unoccluded = false;
								glow.bloom_amount = 0.8f;
							}

							if (is_teammate && (g_vars.get_as<bool>(V_GLOW_TEAM).value())) {
								glow.glow_color = vec3{ glow_team_col[0], glow_team_col[1], glow_team_col[2] };
								glow.alpha = glow_team_col[3];
								glow.render_when_occluded = true;
								glow.render_when_unoccluded = false;
								glow.bloom_amount = 0.8f;
							}
						}
					}
				}
			}
		}
	}

	return o_do_post_screen_effects(g_csgo.m_client_mode, v);
}

static void __stdcall scene_end_h()
{
	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_vars.get_as<bool>(V_CHAMS_ENABLED).value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				if (g_csgo.get_local())
				{
					for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
					{
						auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

						if (!entity)
							continue;

						if (entity == g_csgo.get_local())
							continue;

						if (entity->get_health() <= 0)
							continue;

						if (entity->get_team_num() == g_csgo.get_local()->get_team_num() & !g_vars.get_as<bool>(V_CHAMS_TEAM).value())
							continue;

						if (!entity->is_moving() & g_vars.get_as<bool>(V_CHAMS_WALKING_ONLY).value())
							continue;

						const auto debugambientcube = g_csgo.m_mat_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						const auto flat = g_csgo.m_mat_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						const auto dogtags_outline = g_csgo.m_mat_system->find_material("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);

						debugambientcube->increment_reference_count();
						flat->increment_reference_count();
						dogtags_outline->increment_reference_count();

						c_material* material{};

						switch (g_vars.get_as<int>(V_CHAMS_TYPE).value()) {
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

						if (g_vars.get_as<bool>(V_CHAMS_HEALTH_BASED).value())
						{
							chams_col[0] = 99 / 255.0f;
							chams_col[1] = std::min(255, entity->get_health() * 225 / 100) / 255.0f;
							chams_col[2] = 0.0f;
						}
						else
						{
							chams_col[0] = g_vars.get_as<int>(V_CHAMS_COL_R).value() / 255.0f;
							chams_col[1] = g_vars.get_as<int>(V_CHAMS_COL_G).value() / 255.0f;
							chams_col[2] = g_vars.get_as<int>(V_CHAMS_COL_B).value() / 255.0f;
						}

						g_csgo.m_render_view->modulate_color(chams_col);
						g_csgo.m_render_view->set_blend(1.0f);

						material->set_material_var_flag(material_var_ignorez, !g_vars.get_as<bool>(V_CHAMS_VISIBLE_ONLY).value());

						g_csgo.m_model_render->override_material(material);
						entity->draw_model(1, 255);

						g_csgo.m_model_render->override_material(nullptr);
					}
				}
			}
		}

		if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				std::vector<c_material*> smoke_materials = {
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_OTHER)
				};

				std::vector<c_material*> flash_materials = {
					g_csgo.m_mat_system->find_material("effects/flashbang", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("effects/flashbang_white", TEXTURE_GROUP_OTHER)
				};

				for (const auto& mat : smoke_materials)
				{
					if (mat)
						mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>(V_VISUALS_REMOVALS_SMOKE).value());
				}

				for (const auto& mat : flash_materials)
				{
					if (mat)
						mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>(V_VISUALS_REMOVALS_FLASH).value());
				}
			}
		}
	}

	return o_scene_end(g_csgo.m_render_view);
}

static float __stdcall screen_viewmodel_fov_change_h()
{
	auto ret = o_screen_viewmodel_fov_change(g_csgo.m_client_mode);

	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		return g_vars.get_as<float>(V_MISC_VISUAL_VIEWMODEL_FOV).value();
	}

	return ret;
}

static float __fastcall get_screen_aspect_ratio_h(void* _ecx, void*, int width, int height)
{
	auto ret = o_get_screen_aspect_ratio(_ecx, width, height);

	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_vars.get_as<bool>(V_MISC_VISUAL_ASPECT_RATIO).value())
		{
			return g_vars.get_as<float>(V_MISC_VISUAL_ASPECT_RATIO_VALUE).value();
		}
	}

	return ret;
}

static void __stdcall draw_model_execute_h(i_mat_render_ctx* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world)
{
	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value())
		{
			if (g_csgo.m_engine->is_playing())
			{
				auto model_name = g_csgo.m_model_info->get_model_name(info.model);

				if (g_vars.get_as<bool>(V_VISUALS_REMOVALS_SLEEVES).value())
				{
					if (std::string{ model_name }.find("sleeve") != std::string::npos) {
						g_csgo.m_render_view->set_blend(0.0f);
					}
				}

				if (g_vars.get_as<bool>(V_VISUALS_REMOVALS_HANDS).value())
				{
					if (std::string{ model_name }.find("arms") != std::string::npos) {
						g_csgo.m_render_view->set_blend(0.0f);
					}
				}
			}
		}
	}

	return o_draw_model_execute(g_csgo.m_model_render, ctx, state, info, bone_to_world);
}

static void __fastcall override_view_h(void* _this, void*, c_view_setup* view_setup)
{
	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_csgo.m_engine->is_playing())
		{
			if (g_csgo.get_local() && g_csgo.get_local()->is_life_state())
			{
				g_csgo.m_input->camera_in_third_person = [view_setup]() -> bool
				{
					const auto ret = (g_vars.get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value()) && !g_csgo.get_local()->is_scoped();

					if (ret)
						view_setup->fov = g_vars.get_as<float>(V_MISC_VISUAL_THIRDPERSON_FOV).value();

					return ret;
				}();
			}
		}
	}

	return o_override_view(g_csgo.m_client_mode, _this, view_setup);
}

static bool __fastcall is_connected_h(void* _ecx, void*)
{
	auto ret = o_is_connected(_ecx);

	if (GLOBAL(initialised) && !GLOBAL(panic))
	{
		if (g_vars.get_as<bool>(V_MISC_VISUAL_INVENTORY_UNLOCK).value())
		{
			if (_ReturnAddress() == g_sig.s_is_loadoutallowed)
				return false;
		}
	}

	return ret;
}

static int __fastcall list_leaves_in_box_h(void* bsp, void*, const vec3& mins, const vec3& maxs, unsigned short* list, int list_max)
{
	if (_ReturnAddress() != g_sig.s_list_leaves)
		return o_list_in_leaves_box(bsp, mins, maxs, list, list_max);

	const auto renderable_info = *reinterpret_cast<renderable_info_t**>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if (!renderable_info || !renderable_info->entity)
		return o_list_in_leaves_box(bsp, mins, maxs, list, list_max);

	auto entity = Helpers::call_virtual_fn<c_base_entity*>(renderable_info->entity - 0x4, 7);

	if (!entity || entity->get_client_class()->class_id != ccsplayer)
		return o_list_in_leaves_box(bsp, mins, maxs, list, list_max);

	renderable_info->flags &= ~0x100;
	renderable_info->flags2 |= 0x40;

	static const vec3 map_min = vec3{ MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT };
	static const vec3 map_max = vec3{ MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT };

	return o_list_in_leaves_box(bsp, map_min, map_max, list, list_max);
}

static bool __fastcall sv_cheats_boolean_h(convar* convar, int)
{
	if (_ReturnAddress() == g_sig.s_cam_think)
		return true;

	return o_sv_cheats_boolean(convar);
}

static void __fastcall on_shutdown_h(void* _ecx, void* _edx)
{
	GLOBAL(panic) = true;

	return o_shutdown(_ecx, _edx);
}

void hooks::init()
{
	m_status = MH_Initialize();

	if (!(MH_CreateHook(Helpers::get_virtual_fn<IDirect3DDevice9*>(g_csgo.m_device, present_fn_index),
		present_h, reinterpret_cast<void**>(&o_present)) == MH_OK)) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create present hook");
#endif
	}

	if (!(MH_CreateHook(Helpers::get_virtual_fn<IDirect3DDevice9*>(g_csgo.m_device, reset_fn_index),
		reset_h, reinterpret_cast<void**>(&o_reset)) == MH_OK)) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create reset hook");
#endif
	}

	if (!(MH_CreateHook(Helpers::get_virtual_fn<c_base_client*>(g_csgo.m_client, shutdown_fn_index),
		on_shutdown_h, reinterpret_cast<void**>(&o_shutdown)) == MH_OK)) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create on_shutdown hook");
#endif
	}

	if (GLOBAL(status) == gameVersionOK)
	{
		if (!(MH_CreateHook(Helpers::get_virtual_fn<uintptr_t*>(g_csgo.m_client_mode, create_move_fn_index),
			create_move_h, reinterpret_cast<void**>(&o_create_move)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create create_move hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<c_panel*>(g_csgo.m_panel, paint_traverse_fn_index),
			paint_traverse_h, reinterpret_cast<void**>(&o_paint_traverse)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create paint_traverse hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<uintptr_t*>(g_csgo.m_client_mode, do_post_screen_effects_fn_index),
			do_post_screen_effects_h, reinterpret_cast<void**>(&o_do_post_screen_effects)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create do_post_screen_effects hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<c_render_view*>(g_csgo.m_render_view, scene_end_fn_index),
			scene_end_h, reinterpret_cast<void**>(&o_scene_end)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create scene_end hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<uintptr_t*>(g_csgo.m_client_mode, screen_viewmodel_fov_change_fn_index),
			screen_viewmodel_fov_change_h, reinterpret_cast<void**>(&o_screen_viewmodel_fov_change)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create screen_viewmodel_fov_change hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<c_engine_client*>(g_csgo.m_engine, get_screen_aspect_ratio_fn_index),
			get_screen_aspect_ratio_h, reinterpret_cast<void**>(&o_get_screen_aspect_ratio)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create get_screen_aspect_ratio hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<c_model_render*>(g_csgo.m_model_render, draw_model_execute_fn_index),
			draw_model_execute_h, reinterpret_cast<void**>(&o_draw_model_execute)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create draw_model_execute hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<uintptr_t*>(g_csgo.m_client_mode, override_view_fn_index),
			override_view_h, reinterpret_cast<void**>(&o_override_view)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create override_view hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<c_engine_client*>(g_csgo.m_engine, is_connected_fn_index),
			is_connected_h, reinterpret_cast<void**>(&o_is_connected)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create is_connected hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<void*>(g_csgo.m_engine->get_bsp_query(), list_in_leaves_box_fn_index),
			list_leaves_in_box_h, reinterpret_cast<void**>(&o_list_in_leaves_box)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create list_leaves_in_box hook");
#endif
		}

		if (!(MH_CreateHook(Helpers::get_virtual_fn<convar*>(g_csgo.m_cvar->get_convar("sv_cheats"), sv_cheats_boolean_fn_index),
			sv_cheats_boolean_h, reinterpret_cast<void**>(&o_sv_cheats_boolean)) == MH_OK)) {
#ifdef _DEBUG
			_DBG_NOTIFY("Failed to create sv_cheats_boolean hook");
#endif
		}
	}

	MH_EnableHook(MH_ALL_HOOKS);
}

void hooks::undo()
{
	if (!(m_status == MH_OK))
		return;

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	MH_Uninitialize();
}