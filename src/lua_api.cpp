#include "luas.h"

#include "interfaces.h"
#include "renderer.h"

#include <files.h>

static void init_basic_things(sol::state_view& state);
static void init_usertypes(sol::state_view& state);
static void init_util_functions(sol::state_view& state);
static void init_renderer_functions(sol::state_view& state);

void c_lua_mgr::init_api()
{
	sol::state_view state = m_state;

	init_basic_things(state);
	init_usertypes(state);
	init_util_functions(state);
	init_renderer_functions(state);

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

	state.new_enum("RENDERER_TEXT_FLAGS",
		"TEXT_NONE",     TEXT_NONE,
		"TEXT_OUTLINE",  TEXT_OUTLINE,
		"TEXT_CENTER_X", TEXT_CENTER_X);
}

static void init_usertypes(sol::state_view& state)
{
	state.new_usertype<color_t>("Color", sol::constructors<color_t(int, int, int, int)>(),
		"r", [](const color_t& c) { return c.get_arr()[0]; },
		"g", [](const color_t& c) { return c.get_arr()[1]; },
		"b", [](const color_t& c) { return c.get_arr()[2]; },
		"a", [](const color_t& c) { return c.get_arr()[3]; });

	state.new_usertype<ID3DXFont>("Font",
		"Release", [](ID3DXFont* font) { if (font) font->Release(); }
	);

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

static void init_renderer_functions(sol::state_view& state)
{
	sol::table ret = state.create_table();

	ret.set_function("CreateFont", [](const std::string& name, int px, uint32_t weight, DWORD quality) {
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

	ret.set_function("Text", [](const std::string& str, int x, int y, ID3DXFont* font, uint8_t flags, color_t c) {
		return g_font->draw_string(str, x, y, font, flags, c);
	});

	ret.set_function("GetTextWidth", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_width(str, font);
	});

	ret.set_function("GetTextHeight", [](const std::string& str, ID3DXFont* font) {
		return g_font->get_text_height(str, font);
	});

	ret.set_function("Rect", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect(x, y, w, h, c);
	});

	ret.set_function("RectCornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_cornered(x, y, w, h, radii, c);
	});

	ret.set_function("RectFill", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->rect_fill(x, y, w, h, c);
	});

	ret.set_function("RectFillCornered", [](int x, int y, int w, int h, float radii, color_t c) {
		return g_renderer->rect_fill_cornered(x, y, w, h, radii, c);
	});

	ret.set_function("Line", [](int x, int y, int w, int h, color_t c) {
		return g_renderer->line(x, y, w, h, c);
	});

	ret.set_function("Circle", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle(x, y, radius, c);
	});

	ret.set_function("CircleFill", [](int x, int y, float radius, color_t c) {
		return g_renderer->circle_fill(x, y, radius, c);
	});

	ret.set_function("GradientH", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_h(x, y, w, h, c_a, c_b);
	});

	ret.set_function("GradientV", [](int x, int y, int w, int h, color_t c_a, color_t c_b) {
		return g_renderer->gradient_v(x, y, w, h, c_a, c_b);
	});

	state["Renderer"] = ret;
}