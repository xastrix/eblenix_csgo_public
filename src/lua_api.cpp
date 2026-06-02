#include "luas.h"

#include "globals.h"
#include "cfg.h"
#include "interfaces.h"
#include "math.h"
#include "signatures.h"
#include "helpers.h"
#include "ui.h"
#include "input.h"
#include "events.h"

#include <files.h>
#include <aes_clipboard.hpp>

static void init_enums(sol::environment& env);
static void init_usertypes(sol::environment& env);
static void init_global_vars_functions(sol::environment& env);
static void init_cfg_functions(sol::environment& env);
static void init_convar_functions(sol::environment& env);
static void init_cvar_functions(sol::environment& env);
static void init_renderer_functions(sol::environment& env);
static void init_engine_functions(sol::environment& env);
static void init_entity_functions(sol::environment& env);
static void init_entity_list_functions(sol::environment& env);
static void init_math_functions(sol::environment& env, sol::state_view state);
static void init_surface_functions(sol::environment& env);
static void init_clipboard_functions(sol::environment& env);
static void init_memory_functions(sol::environment& env);
static void init_cmdl_functions(sol::environment& env);
static void init_global_functions(sol::environment& env);
static void init_lists(sol::environment& env);
static void init_safe_env(sol::environment& env, sol::state_view state);

/* Convert callback id to string */
static std::string callback_id_to_string(int id);

/* Unique id for registered callbacks */
static uint64_t g_temp_id;

void c_lua_mgr::init_api(sol::state_view state)
{
	init_enums(m_env);
	init_usertypes(m_env);

	init_global_vars_functions(m_env);
	init_cfg_functions(m_env);
	init_convar_functions(m_env);
	init_cvar_functions(m_env);
	init_renderer_functions(m_env);
	init_engine_functions(m_env);
	init_entity_functions(m_env);
	init_entity_list_functions(m_env);
	init_math_functions(m_env, state);
	init_surface_functions(m_env);
	init_clipboard_functions(m_env);
	init_memory_functions(m_env);
	init_cmdl_functions(m_env);
	init_global_functions(m_env);

	init_lists(m_env);
	init_safe_env(m_env, state);

	m_env.set_function("register_callback", [&](sol::this_state s, int callback_id, sol::function fn) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (callback_id <= CL_NONE || callback_id >= maxCallbacks) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to register callback (Invalid callback id)", path.c_str(), line);
			return uint64_t{};
		}

		std::wstring name = std::filesystem::path(path).filename().wstring();

		g_temp_id++;
		m_lua_event[callback_id].push_back({ g_temp_id, get_script_index_by_name(name), false, fn });

		Helpers::console_printf_with_prefix("[lua]", "Subscribed to %s in %s (Updated %s)",
			callback_id_to_string(callback_id).c_str(), std::string(name.begin(), name.end()).c_str(), get_script_update_datetime(name).c_str());

		return g_temp_id;
	});

	m_env.set_function("unregister_callback", [&](sol::this_state s, int callback_id, uint64_t temp_id) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (callback_id <= CL_NONE || callback_id >= maxCallbacks) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to unregister callback (Invalid callback id)", path.c_str(), line);
			return;
		}

		auto it = m_lua_event.find(callback_id);
		if (it == m_lua_event.end()) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to unregister callback (Callback %s is not registered)",
				path.c_str(), line, callback_id_to_string(callback_id).c_str());
			return;
		}

		auto& vec = it->second;
		bool found_tmp = false;

		for (auto& entry : vec) {
			if (entry.tmp_id == temp_id) {
				entry.nulled = true;
				found_tmp = true;
				break;
			}
		}

		if (!found_tmp) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to unregister callback (Invalid handle)", path.c_str(), line);
			return;
		}

		std::wstring name = std::filesystem::path(path).filename().wstring();

		Helpers::console_printf_with_prefix("[lua]", "Unsubscribed from %s in %s",
			callback_id_to_string(callback_id).c_str(), std::string(name.begin(), name.end()).c_str());
	});
}

static void init_enums(sol::environment& env)
{
	env.new_enum("cb",
		callback_id_to_string(CL_ON_PREINIT),        CL_ON_PREINIT,
		callback_id_to_string(CL_ON_INIT),           CL_ON_INIT,
		callback_id_to_string(CL_ON_UNLOAD),         CL_ON_UNLOAD,
		callback_id_to_string(CL_ON_PRESENT),        CL_ON_PRESENT,
		callback_id_to_string(CL_ON_PRESENT_END),    CL_ON_PRESENT_END,
		callback_id_to_string(CL_ON_RESET),          CL_ON_RESET,
		callback_id_to_string(CL_ON_RESET_END),      CL_ON_RESET_END,
		callback_id_to_string(CL_ON_CREATE_MOVE),    CL_ON_CREATE_MOVE,
		callback_id_to_string(CL_ON_PAINT_TRAVERSE), CL_ON_PAINT_TRAVERSE,
		callback_id_to_string(CL_ON_WND_PROC),       CL_ON_WND_PROC,
		callback_id_to_string(CL_ON_GAME_EVENTS),    CL_ON_GAME_EVENTS,
		callback_id_to_string(CL_ON_FRAME_STAGE),    CL_ON_FRAME_STAGE
	);

	env.new_enum("text_flags",
		"text_none",                                 TEXT_NONE,
		"text_outline",                              TEXT_OUTLINE,
		"text_center_x",                             TEXT_CENTER_X
	);

	env.new_enum("cmd_buttons",
		"in_attack",                                 in_attack,
		"in_attack2",                                in_attack2,
		"in_jump",                                   in_jump,
		"in_duck",                                   in_duck,
		"in_forward",                                in_forward,
		"in_back",                                   in_back,
		"in_use",                                    in_use,
		"in_moveleft",                               in_moveleft,
		"in_moveright",                              in_moveright,
		"in_score",                                  in_score,
		"in_bullrush",                               in_bullrush
	);

	env.new_enum("move_type",
		"walk",                                      movetype_walk,
		"fly",                                       movetype_fly,
		"noclip",                                    movetype_noclip,
		"ladder",                                    movetype_ladder,
		"observer",                                  movetype_observer
	);

	env.new_enum("bbox_type",
		"box_static",                                BT_STATIC,
		"box_dynamic",                               BT_DYNAMIC
	);

	env.new_enum("e_list",
		g_event_list[PLAYER_HURT],                   PLAYER_HURT,
		g_event_list[PLAYER_DEATH],                  PLAYER_DEATH,
		g_event_list[PLAYER_SPAWN],                  PLAYER_SPAWN,
		g_event_list[PLAYER_CONNECT],                PLAYER_CONNECT,
		g_event_list[PLAYER_DISCONNECT],             PLAYER_DISCONNECT,
		g_event_list[PLAYER_FOOTSTEP],               PLAYER_FOOTSTEP,
		g_event_list[PLAYER_FALLDAMAGE],             PLAYER_FALLDAMAGE,
		g_event_list[PLAYER_GIVEN_C4],               PLAYER_GIVEN_C4,
		g_event_list[HOSTAGE_HURT],                  HOSTAGE_HURT,
		g_event_list[HOSTAGE_KILLED],                HOSTAGE_KILLED,
		g_event_list[HOSTAGE_RESCUED],               HOSTAGE_RESCUED,
		g_event_list[ITEM_PURCHASE],                 ITEM_PURCHASE,
		g_event_list[ROUND_PRESTART],                ROUND_PRESTART,
		g_event_list[ROUND_START],                   ROUND_START,
		g_event_list[ROUND_END],                     ROUND_END,
		g_event_list[BOMB_BEGINPLANT],               BOMB_BEGINPLANT,
		g_event_list[BOMB_PLANTED],                  BOMB_PLANTED,
		g_event_list[BOMB_EXPLODED],                 BOMB_EXPLODED,
		g_event_list[BOMB_BEGINDEFUSE],              BOMB_BEGINDEFUSE,
		g_event_list[BOMB_ABORTDEFUSE],              BOMB_ABORTDEFUSE,
		g_event_list[BOMB_DEFUSED],                  BOMB_DEFUSED,
		g_event_list[BOMB_DROPPED],                  BOMB_DROPPED,
		g_event_list[BOMB_BEEP],                     BOMB_BEEP
	);

	env.new_enum("surface_text_flags",
		"none",                                      sff_none,
		"italic",                                    sff_italic,
		"underline",                                 ssf_underline,
		"strikeout",                                 sff_strikeout,
		"symbol",                                    sff_symbol,
		"antialias",                                 sff_antialias,
		"gaussianblur",                              sff_gaussianblur,
		"rotary",                                    sff_rotary,
		"dropshadow",                                sff_dropshadow,
		"additive",                                  sff_additive,
		"outline",                                   sff_outline,
		"custom",                                    sff_custom,
		"bitmap",                                    sff_bitmap
	);
}

static void init_usertypes(sol::environment& env)
{
	env.new_usertype<vec2>("vec2", sol::constructors<vec2(float, float)>(),
		"x",             &vec2::x,
		"y",             &vec2::y
	);

	env.new_usertype<vec3>("vec3", sol::constructors<vec3(float, float, float)>(),
		"x",             &vec3::x,
		"y",             &vec3::y,
		"z",             &vec3::z
	);

	env.new_usertype<box>("bbox", sol::constructors<box(int, int, int, int)>(),
		"x",             &box::x,
		"y",             &box::y,
		"w",             &box::w,
		"h",             &box::h
	);

	env.new_usertype<c_color>("color", sol::constructors<c_color(int, int, int, int)>(),
		"r",             [](const c_color& c) { return c.get_arr()[0]; },
		"g",             [](const c_color& c) { return c.get_arr()[1]; },
		"b",             [](const c_color& c) { return c.get_arr()[2]; },
		"a",             [](const c_color& c) { return c.get_arr()[3]; });

	env.new_usertype<sprite_t>("sprite",
		"begin_draw",    [](sprite_t& sprite) { sprite.begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE); },
		"draw",          [](sprite_t& sprite, int x, int y, c_color c) { sprite.draw(x, y, c); },
		"end_draw",      [](sprite_t& sprite) { sprite.end(); },
		"on_reset",      [](sprite_t& sprite) { sprite.on_reset(); },
		"on_reset_end",  [](sprite_t& sprite) { sprite.on_reset_end(); },
		"destroy",       [](sprite_t* sprite) { if (sprite) delete sprite; }
	);

	env.new_usertype<c_game_event>("game_event",
		"get_name",      [](c_game_event* _event) { return _event->get_name(); },
		"get_int",       [](c_game_event* _event, const char* name) { return _event->get_int(name); },
		"get_string",    [](c_game_event* _event, const char* name) { return _event->get_string(name); }
	);

	env.new_usertype<player_info_t>("player_info",
		"name",          sol::readonly(&player_info_t::m_player_name),
		"friendsname",   sol::readonly(&player_info_t::m_friends_name),
		"user_id",       sol::readonly(&player_info_t::m_user_id),
		"fakeplayer",    sol::readonly(&player_info_t::m_is_fake_player),
		"ishltv",        sol::readonly(&player_info_t::m_is_hltv)
	);

	env.new_usertype<user_cmd_t>("user_cmd",
		"cmd_number",    sol::readonly(&user_cmd_t::m_command_number),
		"forward_move",  &user_cmd_t::m_forwardmove,
		"side_move",     &user_cmd_t::m_sidemove,
		"up_move",       &user_cmd_t::m_upmove,
		"buttons",       &user_cmd_t::m_buttons
	);

	env.new_usertype<module_t>("module",
		"name",          &module_t::m_name,
		"addr",          sol::readonly(&module_t::m_base_address),
		"size",          sol::readonly(&module_t::m_size)
	);
}

static void init_global_vars_functions(sol::environment& env)
{
	sol::table table = env.create();

	table["get_frame_count"]        = []() { return g_cs->m_globals->frame_count; };
	table["get_frame_time"]         = []() { return g_cs->m_globals->frame_time; };
	table["get_absolute_frametime"] = []() { return g_cs->m_globals->absolute_frametime; };
	table["get_cur_time"]           = []() { return g_cs->m_globals->cur_time; };
	table["get_real_time"]          = []() { return g_cs->m_globals->real_time; };
	table["get_interval_per_tick"]  = []() { return g_cs->m_globals->interval_per_tick; };
	table["get_tick_count"]         = []() { return g_cs->m_globals->tick_count; };
	table["get_max_clients"]        = []() { return g_cs->m_globals->max_clients; };

	env["globals"] = table;
}

static void init_cfg_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_int", [](const std::string& key) {
		return g_var->get_as<int>(key);
	});

	table.set_function("get_float", [](const std::string& key) {
		return g_var->get_as<float>(key);
	});

	table.set_function("get_bool", [](const std::string& key) {
		return g_var->get_as<bool>(key);
	});

	table.set_function("set_int", [](const std::string& key, int v) {
		return g_var->set(key, v);
	});

	table.set_function("set_float", [](const std::string& key, float v) {
		return g_var->set(key, v);
	});

	table.set_function("set_bool", [](const std::string& key, bool v) {
		return g_var->set(key, v);
	});

	table.set_function("load", [](const std::wstring& name) {
		return g_cfg->load(name);
	});

	table.set_function("save", [](const std::wstring& name) {
		return g_cfg->save(name);
	});

	env["cfg"] = table;
}

static void init_convar_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("find_convar", [](const char* var_name) {
		return g_cs->m_cvar->get_convar(var_name);
	});

	table.set_function("console_printf", [](sol::this_state s, const char* msg, sol::variadic_args as) {
		sol::state_view state{ s };

		sol::function format = state["string"]["format"];
		std::string   res    = format(msg, as);

		return g_cs->m_cvar->console_printf("%s", res.c_str());
	});

	table.set_function("console_color_printf", [](sol::this_state s, const c_color& col, const char* msg, sol::variadic_args as) {
		sol::state_view state{ s };

		sol::function format = state["string"]["format"];
		std::string   res    = format(msg, as);

		return g_cs->m_cvar->console_color_printf(col.get_revert(), "%s", res.c_str());
	});

	env["convar"] = table;
}

static void init_cvar_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_int", [](c_convar* c) {
		return c->get_int();
	});

	table.set_function("get_float", [](c_convar* c) {
		return c->get_float();
	});

	table.set_function("set_int", [](c_convar* c, int v) {
		return c->set_value(v);
	});

	table.set_function("set_float", [](c_convar* c, float v) {
		return c->set_value(v);
	});

	env["cvar"] = table;
}

static void init_renderer_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("create_font", [](sol::this_state s, const std::string& name, int px, uint32_t weight, DWORD quality) {
		ID3DXFont* ret;

		HRESULT hr = D3DXCreateFontA(
			g_renderer->get_device(),
			px, 0,
			weight, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			quality,
			FF_DONTCARE,
			name.c_str(),
			&ret);

		if (FAILED(hr)) {
			std::string src = "?.lua";
			int line = -1;

			lua_Debug ar;
			if (lua_getstack(s, 1, &ar)) {
				if (lua_getinfo(s, "Sl", &ar)) {
					src = ar.source;
					line = ar.currentline;
				}
			}

			std::string name = (src[0] == '@') ? src.substr(1) : src;

			Helpers::console_printf_with_prefix("[lua]",
				"%s:%i: Failed to create font", name.c_str(), line);
		}

		return ret;
	});

	table.set_function("create_sprite", [](sol::this_state s, const std::string& path, int width, int height) {
		sprite_t* ret = new sprite_t();
		
		ret->init(g_renderer->get_device(), { LUA_DIRECTORY_PATHS + path }, width, height);

		if (FAILED(ret->get_result())) {
			std::string src = "?.lua";
			int line = -1;

			lua_Debug ar;
			if (lua_getstack(s, 1, &ar)) {
				if (lua_getinfo(s, "Sl", &ar)) {
					src = ar.source;
					line = ar.currentline;
				}
			}

			std::string name = (src[0] == '@') ? src.substr(1) : src;

			Helpers::console_printf_with_prefix("[lua]",
				"%s:%i: Failed to create sprite", name.c_str(), line);
		}

		return ret;
	});

	table.set_function("text", [](const std::string& str, int x, int y, ID3DXFont* font, uint8_t flags, c_color c) {
		return g_font->draw_string(str, x, y, font, flags, c);
	});

	table.set_function("get_text_width", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_width(str, font);
	});

	table.set_function("get_text_height", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_height(str, font);
	});

	table.set_function("rect", [](int x, int y, int w, int h, c_color c) {
		return g_renderer->rect(x, y, w, h, c);
	});

	table.set_function("rect_cornered", [](int x, int y, int w, int h, float radii, c_color c) {
		return g_renderer->rect_cornered(x, y, w, h, radii, c);
	});

	table.set_function("rect_fill", [](int x, int y, int w, int h, c_color c) {
		return g_renderer->rect_fill(x, y, w, h, c);
	});

	table.set_function("rect_fill_cornered", [](int x, int y, int w, int h, float radii, c_color c) {
		return g_renderer->rect_fill_cornered(x, y, w, h, radii, c);
	});

	table.set_function("line", [](int x, int y, int w, int h, c_color c) {
		return g_renderer->line(x, y, w, h, c);
	});

	table.set_function("circle", [](int x, int y, float radius, c_color c) {
		return g_renderer->circle(x, y, radius, c);
	});

	table.set_function("circle_fill", [](int x, int y, float radius, c_color c) {
		return g_renderer->circle_fill(x, y, radius, c);
	});

	table.set_function("gradient_horizontal", [](int x, int y, int w, int h, c_color c_a, c_color c_b) {
		return g_renderer->gradient_h(x, y, w, h, c_a, c_b);
	});

	table.set_function("gradient_vertical", [](int x, int y, int w, int h, c_color c_a, c_color c_b) {
		return g_renderer->gradient_v(x, y, w, h, c_a, c_b);
	});

	table.set_function("gradient_multi", [](int x, int y, int w, int h, c_color c_a, c_color c_b, c_color c_c, c_color c_d) {
		return g_renderer->gradient_multi(x, y, w, h, c_a, c_b, c_c, c_d);
	});

	table.set_function("gradient_multi_fill", [](int x, int y, int w, int h, c_color c_a, c_color c_b, c_color c_c, c_color c_d) {
		return g_renderer->gradient_multi_fill(x, y, w, h, c_a, c_b, c_c, c_d);
	});

	table.set_function("get_screen_size", []() {
		return g_renderer->get_screen_size();
	});

	env["renderer"] = table;
}

static void init_engine_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_local_player", []() {
		return g_cs->m_engine->get_local_player();
	});

	table.set_function("get_player_for_userid", [](int user_id) {
		return g_cs->m_engine->get_player_for_user_id(user_id);
	});

	table.set_function("is_in_game", []() {
		return g_cs->m_engine->is_in_game();
	});

	table.set_function("is_connected", []() {
		return g_cs->m_engine->is_connected();
	});

	table.set_function("get_player_info", [](int index) {
		player_info_t ret;
		g_cs->m_engine->get_player_info(index, &ret);
		return ret;
	});

	table.set_function("get_viewangles", []() {
		return g_cs->m_engine->get_view_angles();
	});

	table.set_function("set_viewangles", [](vec3& angles) {
		return g_cs->m_engine->set_view_angles(angles);
	});

	table.set_function("execute_cmd", [](const char* cmd) {
		return g_cs->m_engine->execute_cmd(cmd);
	});

	table.set_function("get_product_version_string", []() {
		return g_cs->m_engine->get_product_version_string();
	});

	env["engine"] = table;
}

static void init_entity_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_health", [](c_base_player* e) {
		return e->get_health();
	});

	table.set_function("get_money", [](c_base_player* e) {
		return e->get_cash();
	});

	table.set_function("get_move_type", [](c_base_player* e) {
		return e->get_move_type();
	});

	table.set_function("get_velocity", [](c_base_player* e) {
		return e->get_velocity();
	});

	table.set_function("get_team", [](c_base_player* e) {
		return e->get_team_num();
	});

	table.set_function("get_flash_duration", [](c_base_player* e) {
		return e->get_flash_duration();
	});

	table.set_function("get_dormant", [](c_base_player* e) {
		return e->get_dormant();
	});

	table.set_function("is_scoped", [](c_base_player* e) {
		return e->is_scoped();
	});

	table.set_function("is_defusing", [](c_base_player* e) {
		return e->is_defusing();
	});

	table.set_function("get_vec_origin", [](c_base_player* e) {
		return e->get_vec_origin();
	});

	table.set_function("get_abs_origin", [](c_base_player* e) {
		return e->get_absolute_origin();
	});

	table.set_function("set_spotted", [](c_base_player* e, bool v) {
		return e->is_spotted() = v;
	});

	env["entity"] = table;
}

static void init_entity_list_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_entity", [](int index) {
		return g_cs->m_entity_list->get_client_entity<c_base_player*>(index);
	});

	table.set_function("get_entity_from_handle", [](unsigned int handle) {
		return g_cs->m_entity_list->get_client_entity_handle<void*>(handle);
	});

	table.set_function("get_highest_entity_index", []() {
		return g_cs->m_entity_list->get_highest_index();
	});

	env["entity_list"] = table;
}

static void init_math_functions(sol::environment& env, sol::state_view state)
{
	sol::table table = state["math"];

	table.set_function("world_to_screen", [](const vec3& origin, vec3& screen) {
		return Math::w2s(origin, screen);
	});

	table.set_function("get_bbox", [](c_base_player* e, box& in, _bbox_types type) {
		return Helpers::get_bbox(e, in, type);
	});

	env["math"] = table;
}

static void init_surface_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("set_draw_color", [](int r, int g, int b, int a) {
		return g_cs->m_surface->set_draw_color(r, g, b, a);
	});

	table.set_function("set_text_color", [](int r, int g, int b, int a) {
		return g_cs->m_surface->set_text_color(r, g, b, a);
	});

	table.set_function("draw_filled_rect", [](int x, int y, int w, int h) {
		return g_cs->m_surface->draw_filled_rect(x, y, w, h);
	});

	table.set_function("draw_outlined_rect", [](int x, int y, int w, int h) {
		return g_cs->m_surface->draw_outlined_rect(x, y, w, h);
	});

	table.set_function("draw_line", [](int x1, int y1, int x2, int y2) {
		return g_cs->m_surface->draw_line(x1, y1, x2, y2);
	});

	table.set_function("draw_text_font", [](surface_font_t font) {
		return g_cs->m_surface->draw_text_font(font);
	});

	table.set_function("draw_text_pos", [](int x, int y) {
		return g_cs->m_surface->draw_text_pos(x, y);
	});

	table.set_function("draw_render_text", [](const std::wstring& str) {
		return g_cs->m_surface->draw_render_text(str.c_str(), str.length());
	});

	table.set_function("get_screen_width", []() {
		int w, h;
		g_cs->m_surface->get_screen_size(w, h);
		return w;
	});

	table.set_function("get_screen_height", []() {
		int w, h;
		g_cs->m_surface->get_screen_size(w, h);
		return h;
	});

	table.set_function("get_cursor_pos_x", []() {
		int x, y;
		g_cs->m_surface->get_cursor_pos(x, y);
		return x;
	});

	table.set_function("get_cursor_pos_y", []() {
		int x, y;
		g_cs->m_surface->get_cursor_pos(x, y);
		return y;
	});

	table.set_function("lock_cursor", []() {
		return g_cs->m_surface->lock_cursor();
	});

	table.set_function("unlock_cursor", []() {
		return g_cs->m_surface->unlock_cursor();
	});

	table.set_function("font_create", []() {
		return g_cs->m_surface->font_create();
	});

	table.set_function("set_font_glyph", [](surface_font_t font, const char* name, int tall, int weight, int blur, int scanlines, int flags) {
		return g_cs->m_surface->set_font_glyph(font, name, tall, weight, blur, scanlines, flags);
	});

	table.set_function("play_sound", [](const char* path) {
		return g_cs->m_surface->play_sound(path);
	});

	env["surface"] = table;
}

static void init_clipboard_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("register_format", [](const std::string& format) {
		return RegisterClipboardFormatA(format.c_str());
	});

	table.set_function("aes_crypt", [](sol::this_state s, uint32_t format, const std::string& key, const std::string& plain_text) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (key.length() != 32) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Invalid aes key length (Must be 32)", path.c_str(), line);
			return;
		}

		return aes_export_encrypted_to_clipboard(format, key, plain_text);
	});

	table.set_function("aes_decrypt", [](sol::this_state s, uint32_t format, const std::string& key) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (key.length() != 32) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Invalid aes key length (Must be 32)", path.c_str(), line);
			return std::string{};
		}

		return aes_import_decrypted_from_clipboard(format, key);
	});

	table.set_function("get_data", [](sol::this_state s, uint32_t format) {
		std::string ret;
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (!OpenClipboard(nullptr)) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to open clipboard", path.c_str(), line);
			return ret;
		}

		auto h = GetClipboardData(format);

		if (h == nullptr) {
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: GetClipboardData fail", path.c_str(), line);
			return ret;
		}

		void* p = GlobalLock(h);

		if (p == nullptr) {
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: GlobalLock fail", path.c_str(), line);
			return ret;
		}

		ret = (char*)p;

		GlobalUnlock(h);
		CloseClipboard();

		return ret;
	});

	table.set_function("set_data", [](sol::this_state s, uint32_t format, const std::string& data) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (!OpenClipboard(nullptr)) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to open clipboard", path.c_str(), line);
			return;
		}

		if (!EmptyClipboard()) {
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to empty clipboard", path.c_str(), line);
			return;
		}

		auto h = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);
		if (h == nullptr) {
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: GlobalAlloc fail", path.c_str(), line);
			return;
		}

		void* p = GlobalLock(h);
		memcpy(p, data.c_str(), data.size() + 1);

		GlobalUnlock(h);

		if (SetClipboardData(format, h) == nullptr) {
			GlobalFree(h);
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: SetClipboardData fail", path.c_str(), line);
			return;
		}

		CloseClipboard();
	});

	table.set_function("set_empty", [](sol::this_state s) {
		std::string src = "?.lua";
		int line = -1;

		lua_Debug ar;
		if (lua_getstack(s, 1, &ar)) {
			if (lua_getinfo(s, "Sl", &ar)) {
				src = ar.source;
				line = ar.currentline;
			}
		}

		std::string path = (src[0] == '@') ? src.substr(1) : src;

		if (!OpenClipboard(nullptr)) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to open clipboard", path.c_str(), line);
			return;
		}

		if (!EmptyClipboard()) {
			CloseClipboard();
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to empty clipboard", path.c_str(), line);
			return;
		}

		CloseClipboard();
	});

	env["clip"] = table;
}

static void init_memory_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_module", [](const std::string& name) {
		return Helpers::get_module(name);
	});

	table.set_function("pattern_scan", [](const std::string& module_name, const std::string& signature) {
		return reinterpret_cast<uintptr_t>(g_sig->scan_sig(module_name, signature));
	});

	table.set_function("r8", [](const uintptr_t a) {
		return Helpers::read<int8_t>(a);
	});

	table.set_function("ru8", [](const uintptr_t a) {
		return Helpers::read<uint8_t>(a);
	});

	table.set_function("r16", [](const uintptr_t a) {
		return Helpers::read<int16_t>(a);
	});

	table.set_function("ru16", [](const uintptr_t a) {
		return Helpers::read<uint16_t>(a);
	});

	table.set_function("r32", [](const uintptr_t a) {
		return Helpers::read<int32_t>(a);
	});

	table.set_function("rf32", [](const uintptr_t a) {
		return Helpers::read<float>(a);
	});

	table.set_function("ru32", [](const uintptr_t a) {
		return Helpers::read<uint32_t>(a);
	});

	table.set_function("rd32", [](const uintptr_t a) {
		return Helpers::read<double>(a);
	});

	table.set_function("rb", [](const uintptr_t a) {
		return Helpers::read<bool>(a);
	});

	table.set_function("rs", [](const uintptr_t a) {
		return std::string(reinterpret_cast<const char*>(a));
	});

	env["mem"] = table;
}

static void init_cmdl_functions(sol::environment& env)
{
	sol::table table = env.create();

	table.set_function("get_cmd_line", []() {
		return g_cs->m_cmd_line->get_cmd_line();
	});

	table.set_function("get_parm_count", []() {
		return g_cs->m_cmd_line->parm_count();
	});

	table.set_function("has_parm", [](const std::string& parm) {
		return g_cs->m_cmd_line->has_parm(parm.c_str());
	});

	env["cmdl"] = table;
}

static void init_global_functions(sol::environment& env)
{
	env["ispanic"]                = []() { return GLOBAL(b_flags[BF_PANIC]); };
	
	env["menu_opened"]            = []() { return g_ui->get_menu_state(); };
	env["console_opened"]         = []() { return GLOBAL(b_flags[BF_CONSOLE_OPENED]); };
	env["chat_opened"]            = []() { return GLOBAL(b_flags[BF_CHAT_OPENED]); };

	env["hours_played"]           = []() { return GLOBAL(i_flags[IF_HOURS_IN_GAME]); };

	env["mouse_pos_x"]            = []() { return g_input->get_mouse_pos_x(); };
	env["mouse_pos_y"]            = []() { return g_input->get_mouse_pos_y(); };
	env["mouse_wheel"]            = []() { return g_input->get_mouse_wheel_accumlate(); };

	env["vec2"]                   = [](float x, float y) { return vec2(x, y); };
	env["vec3"]                   = [](float x, float y, float z) { return vec3(x, y, z); };

	env["color"]                  = [](int r, int g, int b, int a) { return c_color(r, g, b, a); };
	env["color_revert"]           = [](int r, int g, int b, int a) { return c_color(a, r, g, b); };

	env["bbox"]                   = [](int x, int y, int w, int h) { return box(x, y, w, h); };

	env["process_keybd_msg"]      = [](UINT m, WPARAM w) { return g_input->process_keybd_message(m, w); };
	env["unload"]                 = []() { return g::unload(); };
	env["print"]                  = [](sol::this_state s, const char* msg, sol::variadic_args as) {
		sol::state_view state{ s };

		sol::function format = state["string"]["format"];
		std::string   res    = format(msg, as);

		return g_cs->m_cvar->console_printf("%s", res.c_str());
	};
}

static void init_lists(sol::environment& env)
{
	sol::table e_list = env.create_with();

	for (int i = 0; i < maxEvents; i++) {
		e_list[i] = g_event_list[i];
	}

	env["event_list"] = e_list;
}

static void init_safe_env(sol::environment& env, sol::state_view state)
{
	sol::table safe_os = env.create();

	safe_os["time"]     = state["os"]["time"];
	safe_os["clock"]    = state["os"]["clock"];
	safe_os["difftime"] = state["os"]["difftime"];
	safe_os["date"]     = state["os"]["date"];

	for (const auto& fn : { "ipairs", "pairs", "next", "select",
		"tonumber", "tostring", "type", "pcall", "xpcall" }) {
		env[fn] = state[fn];
	}

	env["os"] = safe_os;
	env["_G"] = env;
}

static std::string callback_id_to_string(int id)
{
	switch (id) {
	case CL_ON_PREINIT: {
		return "on_preinit";
	}
	case CL_ON_INIT: {
		return "on_init";
	}
	case CL_ON_UNLOAD: {
		return "on_unload";
	}
	case CL_ON_PRESENT: {
		return "on_present";
	}
	case CL_ON_PRESENT_END: {
		return "on_present_end";
	}
	case CL_ON_RESET: {
		return "on_reset";
	}
	case CL_ON_RESET_END: {
		return "on_reset_end";
	}
	case CL_ON_CREATE_MOVE: {
		return "on_move";
	}
	case CL_ON_PAINT_TRAVERSE: {
		return "on_paint_traverse";
	}
	case CL_ON_WND_PROC: {
		return "on_wndproc";
	}
	case CL_ON_GAME_EVENTS: {
		return "on_game_events";
	}
	case CL_ON_FRAME_STAGE: {
		return "on_frame_stage_notify";
	}
	}

	return "?";
}