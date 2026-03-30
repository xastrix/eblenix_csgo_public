#include "luas.h"

#include "interfaces.h"
#include "renderer.h"
#include "math.h"

#include <files.h>

static void init_basic_things(sol::state_view& state);
static void init_usertypes(sol::state_view& state);
static void init_global_variables(sol::state_view& state);
static void init_convar_functions(sol::state_view& state);
static void init_cvar_functions(sol::state_view& state);
static void init_renderer_functions(sol::state_view& state);
static void init_engine_functions(sol::state_view& state);
static void init_entity_functions(sol::state_view& state);
static void init_entity_list_functions(sol::state_view& state);
static void init_math_functions(sol::state_view& state);

void c_lua_mgr::init_api()
{
	sol::state_view state = m_state;

	init_basic_things(state);
	init_usertypes(state);
	init_global_variables(state);
	init_convar_functions(state);
	init_cvar_functions(state);
	init_renderer_functions(state);
	init_engine_functions(state);
	init_entity_functions(state);
	init_entity_list_functions(state);
	init_math_functions(state);

	state.set_function("set_event_callback", [&](sol::this_state s, const std::string& _event, sol::function fn)
	{
		sol::state_view state{ s };
		sol::table rs = state["debug"]["getinfo"](2, ("S"));

		std::string  src  = rs["source"];
		std::wstring name = std::filesystem::path(src.substr(1)).filename().wstring();

		m_lua_event[_event].push_back({ get_script_index_by_name(name), fn });

		g_cs->m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[lua] ");
		g_cs->m_cvar->console_printf("Subscribed to %s in %s (Updated %s)\n", _event.c_str(),
			std::string(name.begin(), name.end()).c_str(), get_script_update_datetime(name).c_str());
	});
}

static void init_basic_things(sol::state_view& state)
{
	state.open_libraries
	(
		sol::lib::base,
		sol::lib::string,
		sol::lib::utf8,
		sol::lib::bit32
	);

	state.new_enum("RENDERER_TEXTFLAGS",
		"TEXT_NONE",     TEXT_NONE,
		"TEXT_OUTLINE",  TEXT_OUTLINE,
		"TEXT_CENTER_X", TEXT_CENTER_X
	);

	state.new_enum("CMD_BUTTONS",
		"IN_ATTACK",     in_attack,
		"IN_ATTACK2",    in_attack2,
		"IN_JUMP",       in_jump,
		"IN_DUCK",       in_duck,
		"IN_FORWARD",    in_forward,
		"IN_BACK",       in_back,
		"IN_USE",        in_use,
		"IN_MOVELEFT",   in_moveleft,
		"IN_MOVERIGHT",  in_moveright,
		"IN_SCORE",      in_score,
		"IN_BULLRUSH",   in_bullrush
	);

	state.new_enum("MOVE_TYPE",
		"WALK",          movetype_walk,
		"FLY",           movetype_fly,
		"NOCLIP",        movetype_noclip,
		"LADDER",        movetype_ladder,
		"OBSERVER",      movetype_observer
	);
}

static void init_usertypes(sol::state_view& state)
{
	state.new_usertype<vec2>("vec2", sol::constructors<vec2(float, float)>(),
		"x", &vec2::x, "y", &vec2::y
	);
	state["vec2"] = [](float x, float y) { return vec2(x, y); };

	state.new_usertype<vec3>("vec3", sol::constructors<vec2(float, float, float)>(),
		"x", &vec3::x, "y", &vec3::y, "z", &vec3::z
	);
	state["vec3"] = [](float x, float y, float z) { return vec3(x, y, z); };

	state.new_usertype<color_t>("color", sol::constructors<color_t(int, int, int, int)>(),
		"r", [](const color_t& c) { return c.get_arr()[0]; },
		"g", [](const color_t& c) { return c.get_arr()[1]; },
		"b", [](const color_t& c) { return c.get_arr()[2]; },
		"a", [](const color_t& c) { return c.get_arr()[3]; });
	state["color"] = [](int r, int g, int b, int a) { return color_t(r, g, b, a); };

	state.new_usertype<ID3DXFont>("font",
		"release", [](ID3DXFont* font) { if (font) font->Release(); }
	);

	state.new_usertype<player_info_t>("PlayerInfo",
		"name", sol::readonly(&player_info_t::m_player_name),
		"friendsname", sol::readonly(&player_info_t::m_friends_name),
		"user_id", sol::readonly(&player_info_t::m_user_id),
		"fakeplayer", sol::readonly(&player_info_t::m_is_fake_player),
		"ishltv", sol::readonly(&player_info_t::m_is_hltv)
	);

	state.new_usertype<user_cmd_t>("UserCmd",
		"cmd_number", sol::readonly(&user_cmd_t::m_command_number),
		"forward_move", &user_cmd_t::m_forwardmove,
		"side_move", &user_cmd_t::m_sidemove,
		"up_move", &user_cmd_t::m_upmove,
		"buttons", &user_cmd_t::m_buttons
	);
}

static void init_global_variables(sol::state_view& state)
{
	sol::table table = state.create_table();

	table["frame_count"] = g_cs->m_globals->frame_count;
	table["frame_time"] = g_cs->m_globals->frame_time;
	table["absolute_frametime"] = g_cs->m_globals->absolute_frametime;
	table["cur_time"] = g_cs->m_globals->cur_time;
	table["real_time"] = g_cs->m_globals->real_time;
	table["interval_per_tick"] = g_cs->m_globals->interval_per_tick;
	table["tick_count"] = g_cs->m_globals->tick_count;
	table["max_clients"] = g_cs->m_globals->max_clients;

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

	table.set_function("create_font", [](const std::string& name, int px, uint32_t weight, DWORD quality) {
		ID3DXFont* ret;

		D3DXCreateFontA(
			g_renderer->get_device(),
			px, 0,
			weight, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			quality,
			FF_DONTCARE,
			name.c_str(),
			&ret);

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

	state["math"] = table;
}