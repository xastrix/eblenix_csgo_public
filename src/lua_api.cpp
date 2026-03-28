#include "luas.h"

#include "interfaces.h"
#include "renderer.h"

#include "i_user_cmd.h"

#include <files.h>

static void init_basic_things(sol::state_view& state);
static void init_usertypes(sol::state_view& state);
static void init_global_variables(sol::state_view& state);
static void init_util_functions(sol::state_view& state);
static void init_renderer_functions(sol::state_view& state);
static void init_engine_functions(sol::state_view& state);
static void init_entity_list_functions(sol::state_view& state);

void c_lua_mgr::init_api()
{
	sol::state_view state = m_state;

	init_basic_things(state);
	init_usertypes(state);
	init_global_variables(state);
	init_util_functions(state);
	init_renderer_functions(state);
	init_engine_functions(state);
	init_entity_list_functions(state);

	state.set_function("SetEventCallback", [&](sol::this_state s, const std::string& _event, sol::function fn)
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

	state["Color"] = [](int r, int g, int b, int a) { return color_t(r, g, b, a); };
}

static void init_usertypes(sol::state_view& state)
{
	state.new_usertype<vec2>("Vec2",
		"x", &vec2::x, "y", &vec2::y
	);

	state.new_usertype<vec3>("Vec3",
		"x", &vec3::x, "y", &vec3::y, "z", &vec3::z
	);

	state.new_usertype<color_t>("Color", sol::constructors<color_t(int, int, int, int)>(),
		"r", [](const color_t& c) { return c.get_arr()[0]; },
		"g", [](const color_t& c) { return c.get_arr()[1]; },
		"b", [](const color_t& c) { return c.get_arr()[2]; },
		"a", [](const color_t& c) { return c.get_arr()[3]; });

	state.new_usertype<ID3DXFont>("Font",
		"Release", [](ID3DXFont* font) { if (font) font->Release(); }
	);

	state.new_usertype<player_info_t>("PlayerInfo",
		"Name", sol::readonly(&player_info_t::m_player_name),
		"FriendsName", sol::readonly(&player_info_t::m_friends_name),
		"UserID", sol::readonly(&player_info_t::m_user_id),
		"Fakeplayer", sol::readonly(&player_info_t::m_is_fake_player),
		"IsHLTV", sol::readonly(&player_info_t::m_is_hltv)
	);

	state.new_usertype<user_cmd_t>("UserCmd",
		"CommandNumber", sol::readonly(&user_cmd_t::m_command_number),
		"ForwardMove", &user_cmd_t::m_forwardmove,
		"SideMove", &user_cmd_t::m_sidemove,
		"UpMove", &user_cmd_t::m_upmove,
		"Buttons", &user_cmd_t::m_buttons
	);
}

static void init_global_variables(sol::state_view& state)
{
	sol::table table = state.create_table();

	table["FrameCount"] = g_cs->m_globals->frame_count;
	table["FrameTime"] = g_cs->m_globals->frame_time;
	table["AbsoluteFrameTime"] = g_cs->m_globals->absolute_frametime;
	table["CurTime"] = g_cs->m_globals->cur_time;
	table["RealTime"] = g_cs->m_globals->real_time;
	table["IntervalPerTick"] = g_cs->m_globals->interval_per_tick;
	table["TickCount"] = g_cs->m_globals->tick_count;
	table["MaxClients"] = g_cs->m_globals->max_clients;

	state["Globals"] = table;
}

static void init_util_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("ConsolePrintf", [](const char* msg) {
		return g_cs->m_cvar->console_printf(msg);
	});

	table.set_function("ConsoleColorPrintf", [](const color_t& col, const char* msg) {
		return g_cs->m_cvar->console_color_printf(col.get(), msg);
	});

	state["Util"] = table;
}

static void init_renderer_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("CreateFont", [](const std::string& name, int px, uint32_t weight, DWORD quality) {
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

	table.set_function("Text", [](const std::string& str, int x, int y, ID3DXFont* font, uint8_t flags, color_t c) {
		return g_font->draw_string(str, x, y, font, flags, c);
	});

	table.set_function("GetTextWidth", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_width(str, font);
	});

	table.set_function("GetTextHeight", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_height(str, font);
	});

	table.set_function("Rect", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect(x, y, w, h, c);
	});

	table.set_function("RectCornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_cornered(x, y, w, h, radii, c);
	});

	table.set_function("RectFill", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect_fill(x, y, w, h, c);
	});

	table.set_function("RectFillCornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_fill_cornered(x, y, w, h, radii, c);
	});

	table.set_function("Line", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->line(x, y, w, h, c);
	});

	table.set_function("Circle", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle(x, y, radius, c);
	});

	table.set_function("CircleFill", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle_fill(x, y, radius, c);
	});

	table.set_function("GradientH", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_h(x, y, w, h, c_a, c_b);
	});

	table.set_function("GradientV", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_v(x, y, w, h, c_a, c_b);
	});

	state["Renderer"] = table;
}

static void init_engine_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("GetLocalPlayer", []() {
		return g_cs->m_engine->get_local_player();
	});

	table.set_function("GetPlayerForUserid", [](int user_id) {
		return g_cs->m_engine->get_player_for_user_id(user_id);
	});

	table.set_function("IsInGame", []() {
		return g_cs->m_engine->is_in_game();
	});

	table.set_function("IsConnected", []() {
		return g_cs->m_engine->is_connected();
	});

	table.set_function("GetPlayerInfo", [](int index) {
		player_info_t ret;
		g_cs->m_engine->get_player_info(index, &ret);
		return ret;
	});

	table.set_function("GetViewangles", []() {
		return g_cs->m_engine->get_view_angles();
	});

	table.set_function("ExecuteCmd", [](const char* cmd) {
		return g_cs->m_engine->execute_cmd(cmd);
	});

	state["Engine"] = table;
}

static void init_entity_list_functions(sol::state_view& state)
{
	sol::table table = state.create_table();

	table.set_function("GetEntity", [](int index) {
		return g_cs->m_entity_list->get_client_entity<c_base_player*>(index);
	});

	table.set_function("GetEntityFromHandle", [](unsigned int handle) {
		return g_cs->m_entity_list->get_client_entity_handle<void*>(handle);
	});

	table.set_function("GetHighestEntityIndex", []() {
		return g_cs->m_entity_list->get_highest_index();
	});

	state["EntityList"] = table;
}