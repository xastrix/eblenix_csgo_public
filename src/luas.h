#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <sol.hpp>

#include <string>
#include <vector>
#include <map>

struct lua_core_t {
	int id;
	sol::protected_function fn;
};

using lua_list_t = std::vector<std::pair<std::wstring, bool>>;
using lua_event_t = std::map<std::string, std::vector<lua_core_t>>;

class c_lua_mgr {
public:
	void init();
	void refresh_scripts();

	void load_script(const std::wstring& name);
	void unload_script(const std::wstring& name);

	int get_script_index_by_name(const std::wstring& name);
	std::wstring get_script_name_by_index(int index);
	bool get_loaded_by_index(int index);

	const std::vector<lua_core_t>& operator[](const std::string& _event) const {
		static const std::vector<lua_core_t> tmp;
		auto it = m_lua_event.find(_event);
		if (it != m_lua_event.end())
			return it->second;
		return tmp;
	}

	lua_list_t get_list() {
		return m_lua_list;
	}

	static std::shared_ptr<c_lua_mgr> make_shared() {
		return std::shared_ptr<c_lua_mgr>(new c_lua_mgr());
	}

	void undo();

private:
	void init_api();
	std::string get_script_update_datetime(const std::wstring& name);

private:
	lua_State*  m_state;
	lua_event_t m_lua_event;
	lua_list_t  m_lua_list;
};

inline std::shared_ptr<c_lua_mgr> g_lua = c_lua_mgr::make_shared();

#define LUA_CALLBACK(_event) g_lua->operator[](_event)