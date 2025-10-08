#include "hooks.h"

#include "../globals.h"
#include "../ui/ui.h"
#include "../var_manager/var_manager.h"
#include "../helpers/helpers.h"

#include "../features/aimbot/aimbot.h"
#include "../features/triggerbot/triggerbot.h"
#include "../features/knifebot/knifebot.h"
#include "../features/esp/esp.h"
#include "../features/visuals/visuals.h"
#include "../features/movement/movement.h"

#include "../../csgo/interfaces/interfaces.h"
#include "../../csgo/signatures/signatures.h"
#include "../../csgo/math/math.h"
#include "../../csgo/fnv.h"

#include <common.h>
#include <minhook/minhook.h>
#include <intrin.h>
#include <sstream>

hooks g_hooks;

using create_move_fn = bool(__stdcall*)(float, i_user_cmd*);
using paint_traverse_fn = void(__thiscall*)(c_panel*, unsigned int, bool, bool);
using present_fn = long(D3DAPI*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
using reset_fn = long(D3DAPI*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using do_post_screen_effects_fn = int(__thiscall*)(void*, int);
using scene_end_fn = void(__thiscall*)(void*);
using screen_viewmodel_fov_change_fn = float(__thiscall*)(uintptr_t*);
using get_screen_aspect_ratio_fn = float(__thiscall*)(void*, int, int);
using draw_model_execute_fn = void(__thiscall*)(c_model_render*, i_mat_render_ctx*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
using override_view_fn = void(__fastcall*)(uintptr_t*, void*, c_view_setup*);
using is_connected_fn = bool(__fastcall*)(void*);
using list_in_leaves_box_fn = int(__thiscall*)(void*, const vec3&, const vec3&, unsigned short*, int);
using sv_cheats_boolean_fn = bool(__thiscall*)(convar*);
using shutdown_fn = void(__fastcall*)(void*, void*);

create_move_fn o_create_move{};
paint_traverse_fn o_paint_traverse{};
present_fn o_present{};
reset_fn o_reset{};
do_post_screen_effects_fn o_do_post_screen_effects{};
scene_end_fn o_scene_end{};
screen_viewmodel_fov_change_fn o_screen_viewmodel_fov_change{};
get_screen_aspect_ratio_fn o_get_screen_aspect_ratio{};
draw_model_execute_fn o_draw_model_execute{};
override_view_fn o_override_view{};
is_connected_fn o_is_connected{};
list_in_leaves_box_fn o_list_in_leaves_box{};
sv_cheats_boolean_fn o_sv_cheats_boolean{};
shutdown_fn o_shutdown{};

MH_STATUS m_stat{ MH_UNKNOWN };

static bool __stdcall create_move_h(float input_sample_frametime, i_user_cmd* cmd)
{
	auto ret = o_create_move(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return ret;

	g_csgo.m_local = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(g_csgo.m_engine->get_local_player()));;

	if (g.m_initialized)
	{
		if (!g.m_panic_mode)
		{
			g_movement.instance(cmd);
			g_aimbot.instance(cmd);
			g_triggerbot.instance(cmd);
			g_knifebot.instance(cmd);
			g_esp.on_create_move(cmd);

			if (g_csgo.m_engine->is_connected())
			{
				if (g_vars.get_as<bool>("misc->visual->reveal_ranks").value())
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
	if (g.m_initialized && !g.m_panic_mode)
	{
		/* This is necessary so that when using the open console or chat window,
		   the menu cannot be used.
		*/
		g.m_console_visible = g_csgo.m_engine->is_console_visible();
		g.m_chat_visible = g_csgo.m_client->is_chat_raised();

		switch (fnv::hash(g_csgo.m_panel->get_panel_name(panel))) {
		case fnv::hash("HudZoom"): {
			if (g_csgo.m_engine->is_connected()) {
				if (g_vars.get_as<bool>("visuals->enabled").value() &&
					g_vars.get_as<bool>("visuals->removals->scope").value())
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
	std::call_once(g.m_init_render, [device]() {
		if (g_render.init(device))
			g_ui.init_sprites(device);
	});

	g_render.begin_render_states();

	switch (g.m_status) {
	case gameVersionOK: {
		if (!g.m_panic_mode) {
			g_esp.instance();
			g_visuals.instance();
			g_ui.instance();
		}
		break;
	}
	case gameVersionOutdated: {
		std::ostringstream oss{};
		oss << "Hack has not been updated [" << g_csgo.m_engine->get_product_version_string() << "]";

		g_render.draw_status(oss.str(), color_t(164, 164, 164));
		break;
	}
	}

	g_render.end_render_states();

	return o_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

static long D3DAPI reset_h(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	g_ui.on_reset_sprites();
	g_render.undo();

	auto ret = o_reset(device, present_parameters);

	g_render.create_objects();
	g_ui.on_reset_end_sprites();

	return ret;
}

static int __stdcall do_post_screen_effects_h(int v)
{
	if (g.m_initialized && !g.m_panic_mode)
		g_esp.on_do_post_screen_effects();

	return o_do_post_screen_effects(g_csgo.m_client_mode, v);
}

static void __stdcall scene_end_h()
{
	if (g.m_initialized && !g.m_panic_mode)
	{
		g_esp.on_scene_end();

		if (g_vars.get_as<bool>("visuals->enabled").value())
		{
			if (g_csgo.m_engine->is_connected())
			{
				std::vector<i_material*> smoke_materials = {
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_OTHER)
				};

				std::vector<i_material*> flash_materials = {
					g_csgo.m_mat_system->find_material("effects/flashbang", TEXTURE_GROUP_OTHER),
					g_csgo.m_mat_system->find_material("effects/flashbang_white", TEXTURE_GROUP_OTHER)
				};

				for (const auto& mat : smoke_materials)
				{
					if (mat)
						mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>("visuals->removals->smoke").value());
				}

				for (const auto& mat : flash_materials)
				{
					if (mat)
						mat->set_material_var_flag(material_var_no_draw, g_vars.get_as<bool>("visuals->removals->flash").value());
				}
			}
		}
	}

	return o_scene_end(g_csgo.m_render_view);
}

static float __stdcall screen_viewmodel_fov_change_h()
{
	auto ret = o_screen_viewmodel_fov_change(g_csgo.m_client_mode);

	if (g.m_initialized && !g.m_panic_mode)
	{
		return g_vars.get_as<float>("misc->visual->viewmodel->fov").value();
	}

	return ret;
}

static float __fastcall get_screen_aspect_ratio_h(void* _ecx, void*, int width, int height)
{
	auto ret = o_get_screen_aspect_ratio(_ecx, width, height);

	if (g.m_initialized && !g.m_panic_mode)
	{
		if (g_vars.get_as<bool>("misc->visual->aspect_ratio").value())
		{
			return g_vars.get_as<float>("misc->visual->aspect_ratio->value").value();
		}
	}

	return ret;
}

static void __stdcall draw_model_execute_h(i_mat_render_ctx* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bone_to_world)
{
	if (g.m_initialized && !g.m_panic_mode)
	{
		if (g_vars.get_as<bool>("visuals->enabled").value())
		{
			if (g_csgo.m_engine->is_playing())
			{
				auto model_name = g_csgo.m_model_info->get_model_name(info.model);

				if (g_vars.get_as<bool>("visuals->removals->sleeves").value())
				{
					if (std::string{ model_name }.find("sleeve") != std::string::npos) {
						g_csgo.m_render_view->set_blend(0.0f);
					}
				}

				if (g_vars.get_as<bool>("visuals->removals->hands").value())
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
	if (g.m_initialized && !g.m_panic_mode)
	{
		if (g_csgo.m_engine->is_playing())
		{
			if (g_csgo.m_local && g_csgo.m_local->is_life_state())
			{
				g_csgo.m_input->camera_in_third_person = [view_setup]() -> bool
				{
					const auto ret = (g_vars.get_as<bool>("misc->visual->thirdperson").value()) && !g_csgo.m_local->is_scoped();

					if (ret)
						view_setup->fov = g_vars.get_as<float>("misc->visual->thirdperson->fov").value();

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

	if (g.m_initialized && !g.m_panic_mode)
	{
		if (g_vars.get_as<bool>("misc->visual->inventory_unlock").value())
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
	g.done();

	return o_shutdown(_ecx, _edx);
}

void hooks::init()
{
	m_stat = MH_Initialize();

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

	if (g.m_status == gameVersionOK)
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
	if (!(m_stat == MH_OK)) {
#ifdef _DEBUG
		_DBG_NOTIFY("Are you sure that you called hooks::init before calling this function?");
#endif
		return;
	}

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}