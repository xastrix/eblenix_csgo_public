#include "hooks.h"

#include "aimbot.h"
#include "visuals.h"
#include "hud.h"
#include "movement.h"

#include "view_setup.h"

// _ReturnAddress, _AddressOfReturnAddress
#include <intrin.h>

static bool(__stdcall *o_create_move)(float, user_cmd_t*);
static bool __stdcall create_move_h(float input_sample_frametime, user_cmd_t* cmd)
{
	auto ret = o_create_move(input_sample_frametime, cmd);

	if (!cmd || !cmd->m_command_number)
		return ret;

	g_cs->init_local({ g_cs->m_entity_list->get_client_entity<c_base_player*>(g_cs->m_engine->get_local_player()) });

	if (GLOBAL(b_flags[BF_INITIALISED]))
	{
		if (!GLOBAL(b_flags[BF_PANIC]))
		{
			g_aimbot->run(cmd);
			g_move->run(cmd);

			if (g_cs->m_engine->is_connected())
			{
				if (g_var->get_as<bool>(V_MISC_VISUAL_REVEAL_RANKS).value())
				{
					if (cmd->m_buttons & in_score)
						g_cs->m_client->dispatch_user_message(cs_um_serverrankrevealall);
				}
			}
		}

		Math::normalize_angles(cmd->m_viewangles);
		Math::clamp_angles(cmd->m_viewangles);

		for (auto _ : LUA_CALLBACK(CL_ON_CREATE_MOVE)) {
			auto result = _.fn(cmd);
			if (!result.valid()) {
				sol::error err = result;
				Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
			}
		}

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
			switch (fnv::hash(g_cs->m_panel->get_panel_name(panel))) {
			case fnv::hash("HudZoom"): {
				panels[0] = panel;
				break;
			}
			}
		}

		if ((panels[0] == panel) &&
			g_var->get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_var->get_as<bool>(V_VISUALS_REMOVALS_SCOPE).value())
			return;
	}

	o_paint_traverse(g_cs->m_panel, panel, force_repaint, allow_force);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		GLOBAL(b_flags[BF_CONSOLE_OPENED]) = g_cs->m_engine->is_console_visible();
		GLOBAL(b_flags[BF_CHAT_OPENED]) = Helpers::is_chat_opened();

		if (g_cs->m_engine->is_connected())
		{
			if (g_var->get_as<bool>(V_VISUALS_ENABLED).value() &&
				g_var->get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
			{
				if (!GLOBAL(b_flags[BF_NIGHTMODE_HOLD]))
				{
					Helpers::modulate_world_brightness({
						{ "World", g_var->get_as<float>(V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS).value() },
						{ "SkyBox", g_var->get_as<float>(V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS).value() },
						{ "Model", g_var->get_as<float>(V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS).value() },
						{ "Decal", g_var->get_as<float>(V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS).value() },
						{ "Other", g_var->get_as<float>(V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS).value() },
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
	g_renderer->begin();

	for (auto _ : LUA_CALLBACK(CL_ON_PRESENT)) {
		auto result = _.fn();
		if (!result.valid()) {
			sol::error err = result;
			Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
		}
	}

	if (!GLOBAL(b_flags[BF_PANIC]))
	{
		g_visuals->run();
		g_hud->run();
		g_ui->run();
	}

	g_renderer->end();

	return o_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

static long(D3DAPI *o_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
static long D3DAPI reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	for (auto _ : LUA_CALLBACK(CL_ON_RESET)) {
		auto result = _.fn();
		if (!result.valid()) {
			sol::error err = result;
			Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
		}
	}

	g_font->undo();
	g_ui->on_reset();
	g_renderer->undo();

	auto ret = o_reset(device, present_parameters);

	if (ret == D3D_OK)
	{
		g_font->restore(device);
		g_renderer->restore(device);
		g_ui->on_reset_end();

		for (auto _ : LUA_CALLBACK(CL_ON_RESET_END)) {
			auto result = _.fn(device);
			if (!result.valid()) {
				sol::error err = result;
				Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
			}
		}
	}

	return ret;
}

static int(__thiscall *o_do_post_screen_effects)(void*, int);
static int __stdcall do_post_screen_effects_h(int v)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
		world_esp_t::get_instance().on_do_post_screen_effects();

	return o_do_post_screen_effects(g_cs->m_client_mode, v);
}

static void(__thiscall *o_scene_end)(void*);
static void __stdcall scene_end_h()
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
		world_esp_t::get_instance().on_scene_end();

	return o_scene_end(g_cs->m_render_view);
}

static float(__thiscall *o_screen_viewmodel_fov_change)(uintptr_t*);
static float __stdcall screen_viewmodel_fov_change_h()
{
	auto ret = o_screen_viewmodel_fov_change(g_cs->m_client_mode);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		return g_var->get_as<float>(V_MISC_VISUAL_VIEWMODEL_FOV).value();
	}

	return ret;
}

static float(__thiscall *o_get_screen_aspect_ratio)(c_engine_client*, int, int);
static float __stdcall get_screen_aspect_ratio_h(int width, int height)
{
	auto ret = o_get_screen_aspect_ratio(g_cs->m_engine, width, height);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_var->get_as<bool>(V_MISC_VISUAL_ASPECT_RATIO).value())
		{
			return (g_var->get_as<float>(V_MISC_VISUAL_ASPECT_RATIO_VALUE).value() * 0.1f) / 2;
		}
	}

	return ret;
}

static void(__thiscall *o_draw_model_execute)(c_model_render*, i_mat_render_ctx*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
static void __stdcall draw_model_execute_h(i_mat_render_ctx* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bone_to_world)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_var->get_as<bool>(V_VISUALS_ENABLED).value())
		{
			if (g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game())
			{
				auto model_name = g_cs->m_model_info->get_model_name(info.m_model);

				if (g_var->get_as<bool>(V_VISUALS_REMOVALS_SLEEVES).value())
				{
					if (std::string{ model_name }.find("sleeve") != std::string::npos) {
						g_cs->m_render_view->set_blend(0.0f);
					}
				}

				if (g_var->get_as<bool>(V_VISUALS_REMOVALS_HANDS).value())
				{
					if (std::string{ model_name }.find("arms") != std::string::npos) {
						g_cs->m_render_view->set_blend(0.0f);
					}
				}
			}
		}
	}

	return o_draw_model_execute(g_cs->m_model_render, ctx, state, info, bone_to_world);
}

static void(__fastcall *o_override_view)(void*, void*, c_view_setup*);
static void __fastcall override_view_h(void* _ecx, void* _edx, c_view_setup* setup)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_cs->m_engine->is_connected() && g_cs->m_engine->is_in_game())
		{
			if (g_cs->get_local() && g_cs->get_local()->is_life_state())
			{
				// this flag is needed so that if our function is disabled,
				// we can safely enable or disable third-person view through the game itself?
				static bool s_tp;

				if (g_var->get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value())
				{
					if (g_cs->get_local()->is_scoped())
						g_cs->m_input->to_first_person();

					else
						if (!g_cs->m_input->in_third_person_mode()) {
							g_cs->m_input->to_third_person();
							s_tp = false;
						}

						else
							setup->m_fov = g_var->get_as<float>(V_MISC_VISUAL_THIRDPERSON_FOV).value();
				}

				else if (!s_tp && g_cs->m_input->in_third_person_mode()) {
					g_cs->m_input->to_first_person();
					s_tp = true;
				}
			}
		}
	}

	return o_override_view(g_cs->m_client_mode, _ecx, setup);
}

static bool(__fastcall *o_is_connected)(void*);
static bool __fastcall is_connected_h(void* _ecx, void*)
{
	auto ret = o_is_connected(_ecx);

	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_var->get_as<bool>(V_MISC_VISUAL_INVENTORY_UNLOCK).value())
		{
			if (_ReturnAddress() == SIG(S_IS_LOADOUTALLOWED))
				return false;
		}
	}

	return ret;
}

static bool(__thiscall *o_sv_cheats_boolean)(convar*);
static bool __fastcall sv_cheats_boolean_h(convar* convar, int)
{
	if (_ReturnAddress() == SIG(S_CAM_THINK))
		return true;

	return o_sv_cheats_boolean(convar);
}

static void(__thiscall *o_draw_set_color)(c_surface_draw_manager*, int, int, int, int);
static void __stdcall draw_set_color_h(int r, int g, int b, int a)
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_var->get_as<bool>(V_ESP_ENABLED).value() && g_var->get_as<bool>(V_ESP_CROSSHAIR_ENABLED).value())
		{
			if (g_cs->m_engine->is_connected())
			{
				if (_ReturnAddress() == SIG(S_CROSSHAIR_COLOR))
				{
					if (g_cs->get_local() && g_cs->get_local()->is_life_state())
					{
						for (int i = 1; i <= g_cs->m_globals->max_clients; i++)
						{
							auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

							if (!entity || entity->get_dormant() || !entity->is_life_state() || entity == g_cs->get_local())
								continue;

							if ((entity->get_team_num() == g_cs->get_local()->get_team_num()) & !g_var->get_as<bool>(V_ESP_TEAM).value())
								continue;

							if (!g_cs->get_local()->can_see_entity(entity->get_eye_pos()) & g_var->get_as<bool>(V_ESP_VISIBLE_ONLY).value())
								continue;

							box bbox{};
							if (Helpers::get_bbox(entity, bbox, BT_STATIC))
							{
								vec2 screen_size = g_renderer->get_screen_size();
								vec2 center = { screen_size.x / 2, screen_size.y / 2 };

								if ((center.x >= bbox.x && center.x <= bbox.x + bbox.w &&
									center.y >= bbox.y && center.y <= bbox.y + bbox.h))
								{
									color_t on_target_col{ V_ESP_CROSSHAIR_COL };

									return o_draw_set_color(g_cs->m_surface,
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

	return o_draw_set_color(g_cs->m_surface, r, g, b, a);
}

static void(__thiscall *o_level_init_post_entity)(c_base_client*);
static void __fastcall level_init_post_entity_h()
{
	if (GLOBAL(b_flags[BF_INITIALISED]) && !GLOBAL(b_flags[BF_PANIC]))
	{
		if (g_var->get_as<bool>(V_VISUALS_ENABLED).value() &&
			g_var->get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
			GLOBAL(b_flags[BF_NIGHTMODE_HOLD]) = false;
	}

	return o_level_init_post_entity(g_cs->m_client);
}

static void(__fastcall *o_shutdown)(c_base_client*);
static void __fastcall on_shutdown_h()
{
	GLOBAL(b_flags[BF_PANIC]) = true;

	return o_shutdown(g_cs->m_client);
}

void c_hooks::init()
{
	MH_Initialize();

	m_hooks[HK_PRESENT].hook<IDirect3DDevice9*, PRESENT_FN_INDEX>(g_cs->m_device,
		present_h, reinterpret_cast<void**>(&o_present));

	m_hooks[HK_RESET].hook<IDirect3DDevice9*, RESET_FN_INDEX>(g_cs->m_device,
		reset_h, reinterpret_cast<void**>(&o_reset));

	m_hooks[HK_SHUTDOWN].hook<c_base_client*, SHUTDOWN_FN_INDEX>(g_cs->m_client,
		on_shutdown_h, reinterpret_cast<void**>(&o_shutdown));

	m_hooks[HK_CREATEMOVE].hook<uintptr_t*, CREATE_MOVE_FN_INDEX>(g_cs->m_client_mode,
		create_move_h, reinterpret_cast<void**>(&o_create_move));

	m_hooks[HK_PAINTTRAVERSE].hook<c_panel*, PAINT_TRAVERSE_FN_INDEX>(g_cs->m_panel,
		paint_traverse_h, reinterpret_cast<void**>(&o_paint_traverse));

	m_hooks[HK_DOPOSTSCREENEFFECTS].hook<uintptr_t*, DO_POST_SCREEN_EFFECTS_FN_INDEX>(g_cs->m_client_mode,
		do_post_screen_effects_h, reinterpret_cast<void**>(&o_do_post_screen_effects));

	m_hooks[HK_SCENEEND].hook<c_render_view*, SCENE_END_FN_INDEX>(g_cs->m_render_view,
		scene_end_h, reinterpret_cast<void**>(&o_scene_end));

	m_hooks[HK_SCREENMODELFOVCHANGE].hook<uintptr_t*, SCREEN_VIEWMODEL_FOV_CHANGE_FN_INDEX>(g_cs->m_client_mode,
		screen_viewmodel_fov_change_h, reinterpret_cast<void**>(&o_screen_viewmodel_fov_change));

	m_hooks[HK_GETSCREENASPECTRATIO].hook<c_engine_client*, GET_SCREEN_ASPECT_RATIO_FN_INDEX>(g_cs->m_engine,
		get_screen_aspect_ratio_h, reinterpret_cast<void**>(&o_get_screen_aspect_ratio));

	m_hooks[HK_DRAWMODELEXECUTE].hook<c_model_render*, DRAW_MODEL_EXECUTE_FN_INDEX>(g_cs->m_model_render,
		draw_model_execute_h, reinterpret_cast<void**>(&o_draw_model_execute));

	m_hooks[HK_OVERRIDEVIEW].hook<uintptr_t*, OVERRIDE_VIEW_FN_INDEX>(g_cs->m_client_mode,
		override_view_h, reinterpret_cast<void**>(&o_override_view));

	m_hooks[HK_ISCONNECTED].hook<c_engine_client*, IS_CONNECTED_FN_INDEX>(g_cs->m_engine,
		is_connected_h, reinterpret_cast<void**>(&o_is_connected));

	m_hooks[HK_SVCHEATSBOOLEAN].hook<convar*, SV_CHEATS_BOOLEAN_FN_INDEX>(g_cs->m_cvar->get_convar("sv_cheats"),
		sv_cheats_boolean_h, reinterpret_cast<void**>(&o_sv_cheats_boolean));

	m_hooks[HK_DRAWSETCOLOR].hook<c_surface_draw_manager*, DRAW_SET_COLOR_FN_INDEX>(g_cs->m_surface,
		draw_set_color_h, reinterpret_cast<void**>(&o_draw_set_color));

	m_hooks[HK_LEVELINITPOSTENTITY].hook<c_base_client*, LEVEL_INIT_POST_ENTITY_FN_INDEX>(g_cs->m_client,
		level_init_post_entity_h, reinterpret_cast<void**>(&o_level_init_post_entity));
}

void c_hooks::undo()
{
	for (int i = 0; i < maxHooks; i++)
		m_hooks[i].unhook();

	MH_Uninitialize();
}
