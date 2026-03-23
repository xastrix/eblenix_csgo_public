#include "luas.h"

#include "globals.h"
#include "interfaces.h"
#include "helpers.h"
#include "color.h"

#include <files.h>

void c_lua_mgr::init()
{
	undo();

	m_state = luaL_newstate();
	luaL_openlibs(m_state);

	init_api();
	refresh_scripts();
	load_startup_scripts();
}

void c_lua_mgr::refresh_scripts()
{
	std::vector<std::wstring> ls;

	if (!m_lua_list.empty())
	{
		for (const auto& script : m_lua_list)
			unload_script(script.first);

		m_lua_list.clear();
	}

	Files::make_dirs(LUA_DIRECTORY_PATHS);
	Files::get_directory_files(LUA_DIRECTORY_PATHS, ".lua", ls);

	for (const auto& f : ls)
		m_lua_list.push_back(std::make_pair(f, false));
}

void c_lua_mgr::load_startup_scripts()
{
	std::string data;

	if (!(Files::file_exists(LUA_STARTUP_LIST_PATH) == FS_OK)) {
		Files::write(LUA_STARTUP_LIST_PATH, "a", "{}");
		return;
	}

	if (!(Files::get_file_content(LUA_STARTUP_LIST_PATH, data) == FS_OK))
		return;

	for (const auto& script : Helpers::parse_json_object(data))
		load_script(std::wstring(script.begin(), script.end()));
}

void c_lua_mgr::load_script(const std::wstring& name)
{
	sol::state_view state = m_state;

	if (m_lua_list.empty())
		return;

	auto it = std::find_if(m_lua_list.begin(), m_lua_list.end(), [&](const auto& p) {
		return p.first == name;
	});

	if (it != m_lua_list.end() && it->second == true)
		return;

	state.script_file(LUA_DIRECTORY_PATHS + std::string(name.begin(), name.end()),
		[](lua_State*, sol::protected_function_result result) {
		if (!result.valid()) {
			g_cs->m_cvar->console_color_printf(color_t(255, V_UI_COL).get_revert(), "[lua] ");

			sol::error err = result;
			g_cs->m_cvar->console_printf("%s\n", err.what());
		}

		return result;
	});

	if (it != m_lua_list.end())
		it->second = true;
}

void c_lua_mgr::unload_script(const std::wstring& name)
{
	if (m_lua_list.empty())
		return;

	auto it = std::find_if(m_lua_list.begin(), m_lua_list.end(), [&](const auto& p) {
		return p.first == name;
	});

	if (it != m_lua_list.end() && it->second == false)
		return;

	for (auto& ev : m_lua_event)
	{
		int pos = 0;

		for (auto& hk : ev.second)
		{
			if (hk.id == get_script_index_by_name(name))
				ev.second.erase(ev.second.begin() + pos);

			pos++;
		}
	}

	if (it != m_lua_list.end())
		it->second = false;
}

int c_lua_mgr::get_script_index_by_name(const std::wstring& name)
{
	int id = 0;

	for (const auto& pair : m_lua_list)
	{
		if (pair.first == name)
			break;

		id++;
	}

	return id;
}

std::wstring c_lua_mgr::get_script_name_by_index(int index)
{
	int id = 0;

	for (const auto& pair : m_lua_list)
	{
		if (id == index)
			return pair.first;

		id++;
	}

	return {};
}

bool c_lua_mgr::get_loaded_by_index(int index)
{
	int id = 0;

	for (const auto& pair : m_lua_list)
	{
		if (id == index)
			return pair.second;

		id++;
	}

	return false;
}

std::string c_lua_mgr::get_script_update_datetime(const std::wstring& name)
{
	int64_t diff = Files::get_file_diff(LUA_DIRECTORY_PATHS + std::string(name.begin(), name.end()));

	int64_t s = diff / 1000;
	int64_t d = s / 86400;
	int64_t h = (s % 86400) / 3600;
	int64_t m = (s % 3600) / 60;

	std::string result;

	if (d > 0) {
		result += std::to_string(d) + "d ";
	}

	if (h > 0) {
		result += std::to_string(h) + "h ";
	}

	if (m > 0 || (!d && !h)) {
		result += std::to_string(m) + " min ago";
	}

	if (result.empty()) {
		result = "just now";
	}

	return result;
}

void c_lua_mgr::undo()
{
	if (m_state == nullptr)
		return;

	lua_gc(m_state, LUA_GCCOLLECT, 0);
	lua_close(m_state);
	
	m_state = nullptr;
}