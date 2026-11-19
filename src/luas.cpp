#include "luas.h"

#include "globals.h"
#include "interfaces.h"
#include "helpers.h"
#include "color.h"

#include <files.h>

void c_lua_mgr::init()
{
	m_state = sol::state();
	m_state.open_libraries(sol::lib::base, sol::lib::string, sol::lib::utf8,
		sol::lib::math, sol::lib::ffi, sol::lib::os);

	m_env = sol::environment(m_state, sol::create, m_state.globals());
	
	for (const auto& lib : { "string", "utf8", "ffi" }) {
		m_env[lib] = m_state[lib];
	}

	init_api(m_state);

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

void c_lua_mgr::reload_active_scripts()
{
	for (const auto& script : m_lua_list)
	{
		if (unload_script(script.first))
			load_script(script.first);
	}
}

void c_lua_mgr::load_startup_scripts()
{
	std::string data;

	if (!(Files::exist_object(LUA_STARTUP_LIST_PATH) == FS_OK)) {
		Files::write(LUA_STARTUP_LIST_PATH, "a", "{}");
		return;
	}

	if (!(Files::read(LUA_STARTUP_LIST_PATH, "rb", data) == FS_OK))
		return;

	for (const auto& script : Helpers::parse_json_object(data))
		load_script(std::wstring(script.begin(), script.end()));
}

bool c_lua_mgr::load_script(const std::wstring& name)
{
	sol::state_view state = m_state;

	if (m_lua_list.empty())
		return false;

	auto it = std::find_if(m_lua_list.begin(), m_lua_list.end(), [&](const auto& p) {
		return p.first == name;
	});

	if (it != m_lua_list.end() && it->second == true)
		return false;

	sol::load_result res = state.load_file(LUA_DIRECTORY_PATHS + Helpers::stutf8(name));

	if (!res.valid()) {
		sol::error err = res;
		Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
		return false;
	}

	sol::protected_function target = res;
	sol::set_environment(m_env, target);

	sol::protected_function_result status = target();

	if (!status.valid()) {
		sol::error err = status;
		Helpers::console_printf_with_prefix("[lua]", "%s", err.what());
		return false;
	}

	if (it != m_lua_list.end() && status.valid())
		it->second = true;

	return it->second;
}

bool c_lua_mgr::unload_script(const std::wstring& name)
{
	if (m_lua_list.empty())
		return false;

	auto it = std::find_if(m_lua_list.begin(), m_lua_list.end(), [&](const auto& p) {
		return p.first == name;
	});

	if (it != m_lua_list.end() && it->second == false)
		return false;

	for (auto& ev : m_lua_event)
	{
		int pos = 0;

		for (auto& hk : ev.second)
		{
			if (hk.index == get_script_index_by_name(name))
				ev.second.erase(ev.second.begin() + pos);

			pos++;
		}
	}

	if (it != m_lua_list.end())
		it->second = false;

	return true;
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

std::vector<std::wstring> c_lua_mgr::get_script_list()
{
	std::vector<std::wstring> ret;

	for (const auto& script : m_lua_list)
		ret.push_back(script.first);

	return ret;
}

std::string c_lua_mgr::get_script_update_datetime(const std::wstring& name)
{
	std::string ret;

	int64_t diff = Files::get_file_diff(LUA_DIRECTORY_PATHS + std::string(name.begin(), name.end()));
	int64_t s    = diff        / 1000;
	int64_t d    = s           / 86400;
	int64_t h    = (s % 86400) / 3600;
	int64_t m    = (s % 3600)  / 60;

	if (d > 0)
	{
		if (d >= 60) {
			ret = "a very long time ago";
			return ret;
		}

		else
			ret += std::to_string(d) + "d ";
	}

	if (h > 0)
		ret += std::to_string(h) + "h ";

	if (m > 0 || (!d && !h))
		ret += std::to_string(m) + " min ago";

	if (ret.empty())
		ret = "just now";

	return ret;
}

void c_lua_mgr::undo()
{
	if (!m_state.lua_state())
		return;

	m_env = sol::lua_nil;
	
	m_state.collect_garbage();
	m_state = sol::state(nullptr);
}