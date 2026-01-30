#include "hooks.h"

#include "aimbot.h"
#include "triggerbot.h"
#include "knifebot.h"
#include "esp.h"
#include "visuals.h"
#include "movement.h"

// _ReturnAddress, _AddressOfReturnAddress
#include <intrin.h>

static bool(__stdcall *o_create_move)(float, user_cmd_t*);
static bool __stdcall create_move_h(float input_sample_frametime, user_cmd_t* cmd)
{
	auto ret = o_create_move(input_sample_frametime, cmd);

	// check cmd
	if (!cmd || !cmd->command_number)
		return ret;

	// init local player
	g_csgo.init_local({ g_csgo.m_entity_list->get_client_entity<c_base_player*>(g_csgo.m_engine->get_local_player()) });

	if (GLOBAL(b_flags[BF_INITIALISED]))
	{
		if (!GLOBAL(b_flags[BF_PANIC]))
		{
			g_aimbot.run(cmd);
			g_triggerbot.run(cmd);
			g_knifebot.run(cmd);
			g_movement.run(cmd);

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

static void(__thiscall *o_paint_traverse)(c_panel*, uint32_t, bool, bool);
static void __stdcall paint_traverse_h(uint32_t panel, bool force_repaint, bool allow_force)
{
	static std::uint32_t panels[1];
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (!panels[0])
		{
			switch (fnv::hash(g_csgo.m_panel->get_panel_name(panel))) {
			case fnv::hash("HudZoom"): {
				panels[0] = panel;
				break;
			}
			}
		}

		if ((panels[0] == panel) &&
			g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_vars.get_as<bool>(V_VISUALS_REMOVALS_SCOPE).value())
			return;
	}

	o_paint_traverse(g_csgo.m_panel, panel, force_repaint, allow_force);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		// set console & chat flags
		GLOBAL(b_flags[BF_CONSOLE_OPENED]) = g_csgo.m_engine->is_console_visible();
		GLOBAL(b_flags[BF_CHAT_OPENED]) = Helpers::is_chat_opened();

		if (g_csgo.m_engine->is_connected())
		{
			if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
				g_vars.get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
			{
				if (!GLOBAL(b_flags[BF_NIGHTMODE_HOLD]))
				{
					Helpers::modulate_world_brightness({
						{ "World", g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS).value() },
						{ "SkyBox", g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS).value() },
						{ "Model", g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS).value() },
						{ "Decal", g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS).value() },
						{ "Other", g_vars.get_as<float>(V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS).value() },
					});

					GLOBAL(b_flags[BF_NIGHTMODE_HOLD]) = true;
				}
			}
			else
			{
				if (GLOBAL(b_flags[BF_NIGHTMODE_HOLD]))
				{
					Helpers::modulate_world_brightness({
						{ "World", 1.0f },
						{ "SkyBox", 1.0f },
						{ "Model", 1.0f },
						{ "Decal", 1.0f },
						{ "Other", 1.0f },
					});

					GLOBAL(b_flags[BF_NIGHTMODE_HOLD]) = false;
				}
			}
		}
	}
}

static long(D3DAPI *o_present)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
static long D3DAPI present_h(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	std::call_once(GLOBAL(of_flags[OF_INIT_RENDER_STUFF]), [device]() {
		g_font.init(device, {
			{ Tahoma12px,     12, "Tahoma",     FW_MEDIUM, ANTIALIASED_QUALITY },
			{ Verdana12px,    12, "Verdana",    FW_SEMIBOLD, ANTIALIASED_QUALITY },
			{ Astriumwep12px, 12, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
			{ Astriumwep16px, 16, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
			{ Astriumwep25px, 25, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
		});

		if (g_render.init(device))
			g_ui.init(device);
	});

	g_render.begin_render_states();

	switch (GLOBAL(status)) {
	case gameVersionOK: {
		if (!GLOBAL(b_flags[BF_PANIC])) {
			g_esp.run();
			g_visuals.run();
			g_ui.run();
		}
		break;
	}
#ifndef DISABLE_CSGO_VERSION_CHECK
	case gameVersionOutdated: {
		std::string string_ver = g_csgo.m_engine->get_product_version_string();
		std::string ss = "Hack has not been updated for " + string_ver;

		const auto string_font = g_font[Tahoma12px];
		const auto string_width = g_font.get_text_width(ss, string_font);

		g_render.draw_filled_rect(GLOBAL(screen_width) - string_width - 15, 10, GLOBAL(screen_width), 17, color_t(20, 20, 20, 140));
		g_render.draw_filled_rect(GLOBAL(screen_width) - string_width - 12, 13, 2, 11, color_t(164, 164, 164));

		g_font.draw_string(ss, GLOBAL(screen_width) - string_width - 5, 12, string_font, TEXT_OUTLINE, color_t(255, 255, 255));
		break;
	}
#endif
	}

	g_render.end_render_states();

	return o_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

static long(D3DAPI *o_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
static long D3DAPI reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	g_font.undo();
	g_ui.on_reset();
	g_render.undo();

	auto ret = o_reset(device, present_parameters);

	if (ret == D3D_OK)
	{
		g_font.init(device, {});
		g_render.create_objects(device);
		g_ui.on_reset_end();
	}

	return ret;
}

static int(__thiscall *o_do_post_screen_effects)(void*, int);
static int __stdcall do_post_screen_effects_h(int v)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
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

						if (glow.next_free_slot != ENTRY_IN_USE)
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
								glow.set_glow(
									g_vars.get_as<int>(V_GLOW_C4_COL_R).value() / 255.0f,
									g_vars.get_as<int>(V_GLOW_C4_COL_G).value() / 255.0f,
									g_vars.get_as<int>(V_GLOW_C4_COL_B).value() / 255.0f,
									g_vars.get_as<int>(V_GLOW_C4_COL_A).value() / 255.0f
								);
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

							const auto flash_duration = glow_object->get_flash_duration();

							if (is_enemy) {
								glow.set_glow(
									glow_enemy_col[0], glow_enemy_col[1], glow_enemy_col[2],

									flash_duration > g_vars.get_as<int>(V_GLOW_ENEMY_COL_A).value() ?
									flash_duration / 255.0f : glow_enemy_col[3]
								);
							}

							if (is_teammate && g_vars.get_as<bool>(V_GLOW_TEAM).value()) {
								glow.set_glow(
									glow_team_col[0], glow_team_col[1], glow_team_col[2],

									flash_duration > g_vars.get_as<int>(V_GLOW_TEAM_COL_A).value() ?
									flash_duration / 255.0f : glow_team_col[3]
								);
							}
						}
					}
				}
			}
		}
	}

	return o_do_post_screen_effects(g_csgo.m_client_mode, v);
}

static void(__thiscall *o_scene_end)(void*);
static void __stdcall scene_end_h()
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_vars.get_as<bool>(V_CHAMS_ENABLED).value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				if (g_csgo.get_local())
				{
					for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
					{
						auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

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

						const auto debugambientcube = g_csgo.m_mat_system->find_material(
							"debug/debugambientcube", TEXTURE_GROUP_MODEL
						);

						const auto flat = g_csgo.m_mat_system->find_material(
							"debug/debugdrawflat", TEXTURE_GROUP_MODEL
						);

						const auto dogtags_outline = g_csgo.m_mat_system->find_material(
							"models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL
						);

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

						float col[3];
						if (g_vars.get_as<bool>(V_CHAMS_HEALTH_BASED).value())
						{
							col[0] = 99 / 255.0f;
							col[1] = std::min(255, entity->get_health() * 225 / 100) / 255.0f;
							col[2] = 0.0f;
						}
						else
						{
							col[0] = g_vars.get_as<int>(V_CHAMS_COL_R).value() / 255.0f;
							col[1] = g_vars.get_as<int>(V_CHAMS_COL_G).value() / 255.0f;
							col[2] = g_vars.get_as<int>(V_CHAMS_COL_B).value() / 255.0f;
						}

						g_csgo.m_render_view->modulate_color(col);
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

				for (const auto& mat : smoke_materials) {
					if (mat) mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>(V_VISUALS_REMOVALS_SMOKE).value());
				}

				for (const auto& mat : flash_materials) {
					if (mat) mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>(V_VISUALS_REMOVALS_FLASH).value());
				}
			}
		}
	}

	return o_scene_end(g_csgo.m_render_view);
}

static float(__thiscall *o_screen_viewmodel_fov_change)(uintptr_t*);
static float __stdcall screen_viewmodel_fov_change_h()
{
	auto ret = o_screen_viewmodel_fov_change(g_csgo.m_client_mode);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		return g_vars.get_as<float>(V_MISC_VISUAL_VIEWMODEL_FOV).value();
	}

	return ret;
}

static float(__thiscall *o_get_screen_aspect_ratio)(c_engine_client*, int, int);
static float __stdcall get_screen_aspect_ratio_h(int width, int height)
{
	auto ret = o_get_screen_aspect_ratio(g_csgo.m_engine, width, height);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_vars.get_as<bool>(V_MISC_VISUAL_ASPECT_RATIO).value())
		{
			return (g_vars.get_as<float>(V_MISC_VISUAL_ASPECT_RATIO_VALUE).value() * 0.1f) / 2;
		}
	}

	return ret;
}

static void(__thiscall *o_draw_model_execute)(c_model_render*, i_mat_render_ctx*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
static void __stdcall draw_model_execute_h(i_mat_render_ctx* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bone_to_world)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
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

static void(__fastcall *o_override_view)(void*, void*, c_view_setup*);
static void __fastcall override_view_h(void* _ecx, void* _edx, c_view_setup* setup)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_csgo.m_engine->is_playing())
		{
			if (g_csgo.get_local() && g_csgo.get_local()->is_life_state())
			{
				// this flag is needed so that if our function is disabled,
				// we can safely enable or disable third-person view through the game itself?
				static bool s_tp;

				if (g_vars.get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value())
				{
					if (g_csgo.get_local()->is_scoped())
						g_csgo.m_input->to_first_person();

					else
						if (!g_csgo.m_input->in_third_person_mode()) {
							g_csgo.m_input->to_third_person();
							s_tp = false;
						}

						else
							setup->fov = g_vars.get_as<float>(V_MISC_VISUAL_THIRDPERSON_FOV).value();
				}

				else if (!s_tp && g_csgo.m_input->in_third_person_mode()) {
					g_csgo.m_input->to_first_person();
					s_tp = true;
				}
			}
		}
	}

	return o_override_view(g_csgo.m_client_mode, _ecx, setup);
}

static bool(__fastcall *o_is_connected)(void*);
static bool __fastcall is_connected_h(void* _ecx, void*)
{
	auto ret = o_is_connected(_ecx);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_vars.get_as<bool>(V_MISC_VISUAL_INVENTORY_UNLOCK).value())
		{
			if (_ReturnAddress() == g_sig[S_IS_LOADOUTALLOWED])
				return false;
		}
	}

	return ret;
}

static int(__thiscall *o_list_in_leaves_box)(void*, const vec3&, const vec3&, unsigned short*, int);
static int __fastcall list_leaves_in_box_h(void* bsp, void*, const vec3& mins, const vec3& maxs, unsigned short* list, int list_max)
{
	if (_ReturnAddress() != g_sig[S_LIST_LEAVES])
		return o_list_in_leaves_box(bsp, mins, maxs, list, list_max);

	const auto renderable_info = Helpers::read<renderable_info_t*>(uintptr_t(_AddressOfReturnAddress()) + 0x14);

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

static bool(__thiscall *o_sv_cheats_boolean)(convar*);
static bool __fastcall sv_cheats_boolean_h(convar* convar, int)
{
	if (_ReturnAddress() == g_sig[S_CAM_THINK])
		return true;

	return o_sv_cheats_boolean(convar);
}

static void(__thiscall *o_draw_set_color)(c_surface_draw_manager*, int, int, int, int);
static void __stdcall draw_set_color_h(int r, int g, int b, int a)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_vars.get_as<bool>(V_ESP_ENABLED).value() && g_vars.get_as<bool>(V_ESP_CROSSHAIR_ENABLED).value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				if (_ReturnAddress() == g_sig[S_CROSSHAIR_COLOR])
				{
					if (g_csgo.get_local() && g_csgo.get_local()->is_life_state())
					{
						for (int i = 1; i <= g_csgo.m_globals->max_clients; i++)
						{
							auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

							if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_csgo.get_local())
								continue;

							if ((entity->get_team_num() == g_csgo.get_local()->get_team_num()) & !g_vars.get_as<bool>(V_ESP_TEAM).value())
								continue;

							if (!g_csgo.get_local()->can_see_entity(entity->get_eye_pos()) & g_vars.get_as<bool>(V_ESP_VISIBLE_ONLY).value())
								continue;

							box bbox{};
							if (g_esp.get_player_bbox(entity, bbox))
							{
								vec2 center{ static_cast<float>(GLOBAL(screen_width)) / 2,
									static_cast<float>(GLOBAL(screen_height)) / 2 };

								if ((center.x >= bbox.x && center.x <= bbox.x + bbox.w &&
									center.y >= bbox.y && center.y <= bbox.y + bbox.h))
								{
									color_t on_target_col{ V_ESP_CROSSHAIR_COL };

									return o_draw_set_color(g_csgo.m_surface,
										on_target_col.get_arr()[0],
										on_target_col.get_arr()[1],
										on_target_col.get_arr()[2],
										a);
								}
							}
						}
					}
				}
			}
		}
	}

	return o_draw_set_color(g_csgo.m_surface, r, g, b, a);
}

static void(__thiscall *o_level_init_post_entity)(c_base_client*);
static void __fastcall level_init_post_entity_h()
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_vars.get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_vars.get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
			GLOBAL(b_flags[BF_NIGHTMODE_HOLD]) = false;
	}

	return o_level_init_post_entity(g_csgo.m_client);
}

static void(__fastcall *o_shutdown)(c_base_client*);
static void __fastcall on_shutdown_h()
{
	GLOBAL(b_flags[BF_PANIC]) = true;

	return o_shutdown(g_csgo.m_client);
}

void hooks::init()
{
	MH_Initialize();
	
	m_hooks[HK_PRESENT].hook<IDirect3DDevice9*, PRESENT_FN_INDEX>(g_csgo.m_device,
		present_h, reinterpret_cast<void**>(&o_present));

	m_hooks[HK_RESET].hook<IDirect3DDevice9*, RESET_FN_INDEX>(g_csgo.m_device,
		reset_h, reinterpret_cast<void**>(&o_reset));

	m_hooks[HK_SHUTDOWN].hook<c_base_client*, SHUTDOWN_FN_INDEX>(g_csgo.m_client,
		on_shutdown_h, reinterpret_cast<void**>(&o_shutdown));

	if (GLOBAL(status) == gameVersionOK)
	{
		m_hooks[HK_CREATEMOVE].hook<uintptr_t*, CREATE_MOVE_FN_INDEX>(g_csgo.m_client_mode,
			create_move_h, reinterpret_cast<void**>(&o_create_move));

		m_hooks[HK_PAINTTRAVERSE].hook<c_panel*, PAINT_TRAVERSE_FN_INDEX>(g_csgo.m_panel,
			paint_traverse_h, reinterpret_cast<void**>(&o_paint_traverse));

		m_hooks[HK_DOPOSTSCREENEFFECTS].hook<uintptr_t*, DO_POST_SCREEN_EFFECTS_FN_INDEX>(g_csgo.m_client_mode,
			do_post_screen_effects_h, reinterpret_cast<void**>(&o_do_post_screen_effects));

		m_hooks[HK_SCENEEND].hook<c_render_view*, SCENE_END_FN_INDEX>(g_csgo.m_render_view,
			scene_end_h, reinterpret_cast<void**>(&o_scene_end));

		m_hooks[HK_SCREENMODELFOVCHANGE].hook<uintptr_t*, SCREEN_VIEWMODEL_FOV_CHANGE_FN_INDEX>(g_csgo.m_client_mode,
			screen_viewmodel_fov_change_h, reinterpret_cast<void**>(&o_screen_viewmodel_fov_change));

		m_hooks[HK_GETSCREENASPECTRATIO].hook<c_engine_client*, GET_SCREEN_ASPECT_RATIO_FN_INDEX>(g_csgo.m_engine,
			get_screen_aspect_ratio_h, reinterpret_cast<void**>(&o_get_screen_aspect_ratio));

		m_hooks[HK_DRAWMODELEXECUTE].hook<c_model_render*, DRAW_MODEL_EXECUTE_FN_INDEX>(g_csgo.m_model_render,
			draw_model_execute_h, reinterpret_cast<void**>(&o_draw_model_execute));

		m_hooks[HK_OVERRIDEVIEW].hook<uintptr_t*, OVERRIDE_VIEW_FN_INDEX>(g_csgo.m_client_mode,
			override_view_h, reinterpret_cast<void**>(&o_override_view));

		m_hooks[HK_ISCONNECTED].hook<c_engine_client*, IS_CONNECTED_FN_INDEX>(g_csgo.m_engine,
			is_connected_h, reinterpret_cast<void**>(&o_is_connected));

		m_hooks[HK_LISTLEAVESINBOX].hook<void*, LIST_IN_LEAVES_BOX_FN_INDEX>(g_csgo.m_engine->get_bsp_query(),
			list_leaves_in_box_h, reinterpret_cast<void**>(&o_list_in_leaves_box));

		m_hooks[HK_SVCHEATSBOOLEAN].hook<convar*, SV_CHEATS_BOOLEAN_FN_INDEX>(g_csgo.m_cvar->get_convar("sv_cheats"),
			sv_cheats_boolean_h, reinterpret_cast<void**>(&o_sv_cheats_boolean));

		m_hooks[HK_DRAWSETCOLOR].hook<c_surface_draw_manager*, DRAW_SET_COLOR_FN_INDEX>(g_csgo.m_surface,
			draw_set_color_h, reinterpret_cast<void**>(&o_draw_set_color));

		m_hooks[HK_LEVELINITPOSTENTITY].hook<c_base_client*, LEVEL_INIT_POST_ENTITY_FN_INDEX>(g_csgo.m_client,
			level_init_post_entity_h, reinterpret_cast<void**>(&o_level_init_post_entity));
	}
}

void hooks::undo()
{
	for (int i = 0; i < maxHooks; i++)
		m_hooks[i].unhook();

	MH_Uninitialize();
}
