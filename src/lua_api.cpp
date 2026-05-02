#include "luas.h"

#include "globals.h"
#include "interfaces.h"
#include "renderer.h"
#include "math.h"
#include "signatures.h"
#include "helpers.h"

#include <files.h>

static void init_enums(sol::state_view& state);
static void init_usertypes(sol::state_view& state);
static void init_global_variables(sol::state_view& state);
static void init_convar_functions(sol::state_view& state);
static void init_cvar_functions(sol::state_view& state);
static void init_renderer_functions(sol::state_view& state);
static void init_engine_functions(sol::state_view& state);
static void init_entity_functions(sol::state_view& state);
static void init_entity_list_functions(sol::state_view& state);
static void init_math_functions(sol::state_view& state);
static void init_util_functions(sol::state_view& state);
static void init_global_functions(sol::state_view& state);
static std::string callback_id_to_string(int id);

void c_lua_mgr::init_api()
{
	sol::state_view state = m_state;

	state.open_libraries
	(
		sol::lib::base,
		sol::lib::string,
		sol::lib::utf8,
		sol::lib::bit32,
		sol::lib::ffi
	);

	init_enums(state);
	init_usertypes(state);
	init_global_variables(state);
	init_convar_functions(state);
	init_cvar_functions(state);
	init_renderer_functions(state);
	init_engine_functions(state);
	init_entity_functions(state);
	init_entity_list_functions(state);
	init_math_functions(state);
	init_util_functions(state);
	init_global_functions(state);

	state.set_function("register_callback", [&](sol::this_state s, int callback_id, sol::function fn)
	{
		sol::state_view state{ s };
		sol::table rs = state["debug"]["getinfo"](2, ("S"));

		std::string src = rs["source"];
		int line        = rs["currentline"];

		if (callback_id <= CL_NONE || callback_id >= maxCallbacks) {
			Helpers::console_printf_with_prefix("[lua]", "%s:%i: Failed to register callback (Invalid callback id)", src.substr(1).c_str(), line);
			return;
		}

		std::wstring name = std::filesystem::path(src.substr(1)).filename().wstring();

		m_lua_event[callback_id].push_back({ get_script_index_by_name(name), fn });

		Helpers::console_printf_with_prefix("[lua]", "Subscribed to %s in %s (Updated %s)",
			callback_id_to_string(callback_id).c_str(), std::string(name.begin(), name.end()).c_str(), get_script_update_datetime(name).c_str());
	});
}

static void init_enums(sol::state_view& state)
{
	state.new_enum("cb",
		callback_id_to_string(CL_ON_PREINIT),     CL_ON_PREINIT,
		callback_id_to_string(CL_ON_INIT),        CL_ON_INIT,
		callback_id_to_string(CL_ON_UNLOAD),      CL_ON_UNLOAD,
		callback_id_to_string(CL_ON_PRESENT),     CL_ON_PRESENT,
		callback_id_to_string(CL_ON_PRESENT_END), CL_ON_PRESENT_END,
		callback_id_to_string(CL_ON_RESET),       CL_ON_RESET,
		callback_id_to_string(CL_ON_RESET_END),   CL_ON_RESET_END,
		callback_id_to_string(CL_ON_CREATE_MOVE), CL_ON_CREATE_MOVE,
		callback_id_to_string(CL_ON_WND_PROC),    CL_ON_WND_PROC);

	state.new_enum("text_flags",
		"text_none",                              TEXT_NONE,
		"text_outline",                           TEXT_OUTLINE,
		"text_center_x",                          TEXT_CENTER_X
	);

	state.new_enum("cmd_buttons",
		"in_attack",                              in_attack,
		"in_attack2",                             in_attack2,
		"in_jump",                                in_jump,
		"in_duck",                                in_duck,
		"in_forward",                             in_forward,
		"in_back",                                in_back,
		"in_use",                                 in_use,
		"in_moveleft",                            in_moveleft,
		"in_moveright",                           in_moveright,
		"in_score",                               in_score,
		"in_bullrush",                            in_bullrush
	);

	state.new_enum("move_type",
		"walk",                                   movetype_walk,
		"fly",                                    movetype_fly,
		"noclip",                                 movetype_noclip,
		"ladder",                                 movetype_ladder,
		"observer",                               movetype_observer
	);


	state.new_enum("bbox_type",
		"box_static",                             BT_STATIC,
		"box_dynamic",                            BT_DYNAMIC
	);
}

static void init_usertypes(sol::state_view& state)
{
	state.new_usertype<vec2>("vec2", sol::constructors<vec2(float, float)>(),
		"x",             &vec2::x,
		"y",             &vec2::y
	);

	state.new_usertype<vec3>("vec3", sol::constructors<vec3(float, float, float)>(),
		"x",             &vec3::x,
		"y",             &vec3::y,
		"z",             &vec3::z
	);

	state.new_usertype<box>("bbox", sol::constructors<box(int, int, int, int)>(),
		"x",             &box::x,
		"y",             &box::y,
		"w",             &box::w,
		"h",             &box::h
	);

	state.new_usertype<color_t>("color", sol::constructors<color_t(int, int, int, int)>(),
		"r",             [](const color_t& c) { return c.get_arr()[0]; },
		"g",             [](const color_t& c) { return c.get_arr()[1]; },
		"b",             [](const color_t& c) { return c.get_arr()[2]; },
		"a",             [](const color_t& c) { return c.get_arr()[3]; });

	state.new_usertype<sprite_t>("sprite",
		"begin_draw",    [](sprite_t& sprite) { sprite.begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE); },
		"draw",          [](sprite_t& sprite, int x, int y, color_t c) { sprite.draw(x, y, c); },
		"end_draw",      [](sprite_t& sprite) { sprite.end(); },
		"on_reset",      [](sprite_t& sprite) { sprite.on_reset(); },
		"on_reset_end",  [](sprite_t& sprite) { sprite.on_reset_end(); }
	);

	state.new_usertype<player_info_t>("player_info",
		"name",          sol::readonly(&player_info_t::m_player_name),
		"friendsname",   sol::readonly(&player_info_t::m_friends_name),
		"user_id",       sol::readonly(&player_info_t::m_user_id),
		"fakeplayer",    sol::readonly(&player_info_t::m_is_fake_player),
		"ishltv",        sol::readonly(&player_info_t::m_is_hltv)
	);

	state.new_usertype<user_cmd_t>("user_cmd",
		"cmd_number",    sol::readonly(&user_cmd_t::m_command_number),
		"forward_move",  &user_cmd_t::m_forwardmove,
		"side_move",     &user_cmd_t::m_sidemove,
		"up_move",       &user_cmd_t::m_upmove,
		"buttons",       &user_cmd_t::m_buttons
	);
}

static void init_global_variables(sol::state_view& state)
{
	sol::table table = state.create_table();

	table["frame_count"]        = g_cs->m_globals->frame_count;
	table["frame_time"]         = g_cs->m_globals->frame_time;
	table["absolute_frametime"] = g_cs->m_globals->absolute_frametime;
	table["cur_time"]           = g_cs->m_globals->cur_time;
	table["real_time"]          = g_cs->m_globals->real_time;
	table["interval_per_tick"]  = g_cs->m_globals->interval_per_tick;
	table["tick_count"]         = g_cs->m_globals->tick_count;
	table["max_clients"]        = g_cs->m_globals->max_clients;

	state["globals"] = table;
}

static void init_convar_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("find_convar", [](const char* var_name) {
		return g_cs->m_cvar->get_convar(var_name);
	});

	table.set_function("console_printf", [](const char* msg) {
		return g_cs->m_cvar->console_printf(msg);
	});

	table.set_function("console_color_printf", [](const color_t& col, const char* msg) {
		return g_cs->m_cvar->console_color_printf(col.get(), msg);
	});

	state["convar"] = table;
}

static void init_cvar_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("get_int", [](convar* c) {
		return c->get_int();
	});

	table.set_function("get_float", [](convar* c) {
		return c->get_float();
	});

	table.set_function("set_int", [](convar* c, int v) {
		return c->set_value(v);
	});

	table.set_function("set_float", [](convar* c, float v) {
		return c->set_value(v);
	});

	state["cvar"] = table;
}

static void init_renderer_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

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
			sol::state_view state{ s };
			sol::table rs = state["debug"]["getinfo"](2, ("Sl"));

			std::string src = rs["source"];
			int line        = rs["currentline"];

			Helpers::console_printf_with_prefix("[lua]",
				"%s:%i: Failed to create font", src.substr(1).c_str(), line);
		}

		return ret;
	});

	table.set_function("create_sprite", [](sol::this_state s, const std::string& path, int width, int height) {
		sprite_t* ret = new sprite_t();
		
		ret->init(g_renderer->get_device(), { LUA_DIRECTORY_PATHS + path }, width, height);

		if (FAILED(ret->get_result())) {
			sol::state_view state{ s };
			sol::table rs = state["debug"]["getinfo"](2, ("Sl"));

			std::string src = rs["source"];
			int line        = rs["currentline"];

			Helpers::console_printf_with_prefix("[lua]",
				"%s:%i: Failed to create sprite", src.substr(1).c_str(), line);
		}

		return ret;
	});

	table.set_function("text", [](const std::string& str, int x, int y, ID3DXFont* font, uint8_t flags, color_t c) {
		return g_font->draw_string(str, x, y, font, flags, c);
	});

	table.set_function("get_text_width", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_width(str, font);
	});

	table.set_function("get_text_height", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_height(str, font);
	});

	table.set_function("rect", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect(x, y, w, h, c);
	});

	table.set_function("rect_cornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_cornered(x, y, w, h, radii, c);
	});

	table.set_function("rect_fill", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect_fill(x, y, w, h, c);
	});

	table.set_function("rect_fill_cornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_fill_cornered(x, y, w, h, radii, c);
	});

	table.set_function("line", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->line(x, y, w, h, c);
	});

	table.set_function("circle", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle(x, y, radius, c);
	});

	table.set_function("circle_fill", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle_fill(x, y, radius, c);
	});

	table.set_function("gradient_horizontal", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_h(x, y, w, h, c_a, c_b);
	});

	table.set_function("gradient_vertical", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_v(x, y, w, h, c_a, c_b);
	});

	table.set_function("gradient_multi", [](int x, int y, int w, int h, color_t c_a, color_t c_b, color_t c_c, color_t c_d) {
		return g_renderer->gradient_multi(x, y, w, h, c_a, c_b, c_c, c_d);
	});

	table.set_function("gradient_multi_fill", [](int x, int y, int w, int h, color_t c_a, color_t c_b, color_t c_c, color_t c_d) {
		return g_renderer->gradient_multi_fill(x, y, w, h, c_a, c_b, c_c, c_d);
	});

	state["renderer"] = table;
}

static void init_engine_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

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

	table.set_function("execute_cmd", [](const char* cmd) {
		return g_cs->m_engine->execute_cmd(cmd);
	});

	state["engine"] = table;
}

static void init_entity_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

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

	state["entity"] = table;
}

static void init_entity_list_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("get_entity", [](int index) {
		return g_cs->m_entity_list->get_client_entity<c_base_player*>(index);
	});

	table.set_function("get_entity_from_handle", [](unsigned int handle) {
		return g_cs->m_entity_list->get_client_entity_handle<void*>(handle);
	});

	table.set_function("get_highest_entity_index", []() {
		return g_cs->m_entity_list->get_highest_index();
	});

	state["entity_list"] = table;
}

static void init_math_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("world_to_screen", [](const vec3& origin, vec3& screen) {
		return Math::w2s(origin, screen);
	});

	table.set_function("get_bbox", [](c_base_player* e, box& in, _bbox_types type) {
		return Helpers::get_bbox(e, in, type);
	});

	state["engine_math"] = table;
}

static void init_util_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("find_pattern", [](const std::string& module_name, const std::string& signature) {
		return g_sig->scan_sig(module_name, signature);
	});

	state["util"] = table;
}

static void init_global_functions(sol::state_view& state)
{
	state["is_panic"] = GLOBAL(b_flags[BF_PANIC]);
	state["unload"] = []() { g::unload(); };
	state["vec2"] = [](float x, float y) { return vec2(x, y); };
	state["vec3"] = [](float x, float y, float z) { return vec3(x, y, z); };
	state["color"] = [](int r, int g, int b, int a) { return color_t(r, g, b, a); };
	state["bbox"] = [](int x, int y, int w, int h) { return box(x, y, w, h); };
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
	case CL_ON_WND_PROC: {
		return "on_wndproc";
	}
	}

	return "?";
}