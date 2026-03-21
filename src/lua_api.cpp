#include "luas.h"

#include "interfaces.h"
#include "color.h"

#include <files.h>

static void init_basic_things(sol::state_view& state);
static void init_usertypes(sol::state_view& state);
static void init_util_functions(sol::state_view& state);

void c_lua_mgr::init_api()
{
	sol::state_view state = m_state;

	init_basic_things(state);
	init_usertypes(state);
	init_util_functions(state);

	state.set_function("SetEventCallback", [&](sol::this_state s, const std::string& _event, sol::function fn)
	{
		sol::state_view state{ s };
		sol::table rs = state["debug"]["getinfo"](2, ("S"));

		std::string  src  = rs["source"];
		std::wstring name = std::filesystem::path(src.substr(1)).filename().wstring();

		m_lua_event[_event].push_back({ get_script_index_by_name(name), fn });

		g_cs->m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[lua] ");
		g_cs->m_cvar->console_printf("Subscribed to %s in %s (Updated %s)\n", _event.c_str(), name.c_str(),
			get_script_update_datetime(name).c_str());
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
}

static void init_usertypes(sol::state_view& state)
{
	state.new_usertype<color_t>("Color", sol::constructors<color_t(int, int, int, int)>(),
		"r", [](const color_t& c) { return c.get_arr()[0]; },
		"g", [](const color_t& c) { return c.get_arr()[1]; },
		"b", [](const color_t& c) { return c.get_arr()[2]; },
		"a", [](const color_t& c) { return c.get_arr()[3]; });

	state["Color"] = [](int r, int g, int b, int a) { return color_t(r, g, b, a); };
}

static void init_util_functions(sol::state_view& state)
{
	sol::table ret = state.create_table();

	ret.set_function("ConsoleColorPrintf", [](const color_t& col, const char* msg) {
		return g_cs->m_cvar->console_color_printf(col.get(), msg);
	});

	state["Util"] = ret;
}