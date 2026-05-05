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

enum _callback_list {
	CL_NONE,
	CL_ON_PREINIT,
	CL_ON_INIT,
	CL_ON_UNLOAD,
	CL_ON_PRESENT,
	CL_ON_PRESENT_END,
	CL_ON_RESET,
	CL_ON_RESET_END,
	CL_ON_CREATE_MOVE,
	CL_ON_PAINT_TRAVERSE,
	CL_ON_WND_PROC,
	maxCallbacks,
};

struct lua_core_t {
	int id;
	sol::protected_function fn;
};

using lua_list_t  = std::vector<std::pair<std::wstring, bool>>;
using lua_event_t = std::map<int, std::vector<lua_core_t>>;

class c_lua_mgr {
public:
	void init();
	void refresh_scripts();
	void reload_active_scripts();

	bool load_script(const std::wstring& name);
	bool unload_script(const std::wstring& name);

	int get_script_index_by_name(const std::wstring& name);
	std::wstring get_script_name_by_index(int index);
	bool get_loaded_by_index(int index);

	const std::vector<lua_core_t>& operator[](int callback_id) const {
		static const std::vector<lua_core_t> tmp;
		auto it = m_lua_event.find(callback_id);
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
	void load_startup_scripts();
	std::string get_script_update_datetime(const std::wstring& name);

private:
	lua_State*  m_state;
	lua_event_t m_lua_event;
	lua_list_t  m_lua_list;
};

inline std::shared_ptr<c_lua_mgr> g_lua = c_lua_mgr::make_shared();

#define LUA_CALLBACK(callback_id) g_lua->operator[](callback_id)