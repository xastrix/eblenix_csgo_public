#pragma once

#include <windows.h>
#include <string>
#include <thread>

#include "color.h"
#include "base_entity.h"

using create_interface_fn = void*(*)(const char*, int*);

enum bbox_type {
	BT_STATIC,
	BT_DYNAMIC,
};

enum weapon_esp_type {
	WE_TEXT,
	WE_ICON,
};

enum _wfm_stat {
	WM_OK,
	WM_TIMEOUT,
};

struct box {
	box() = default;
	box(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

	int x, y, w, h;
};

namespace Helpers
{
	int get_nearest_bone(c_base_player* entity, user_cmd_t* cmd);
	int find_target_entity(user_cmd_t* cmd, const float fov, vec3& angle);

	bool is_pistol(c_base_weapon* weapon);
	bool is_sniper(c_base_weapon* weapon);
	bool is_auto(c_base_weapon* weapon);
	bool is_revolver(c_base_weapon* weapon);
	bool is_taser(c_base_weapon* weapon);
	bool is_smg(c_base_weapon* weapon);
	bool is_heavy(c_base_weapon* weapon);
	bool is_rifle(c_base_weapon* weapon);
	bool is_grenade(c_base_weapon* weapon);
	bool is_c4(c_base_weapon* weapon);
	bool is_knife(c_base_weapon* weapon);
	bool is_non_aim(c_base_weapon* weapon);

	bool get_bbox(c_base_player* entity, box& in, const bbox_type type);
	bool is_behind_smoke(const vec3 start_pos, const vec3 end_pos);
	bool is_chat_opened();

	void modulate_world_brightness(std::vector<std::pair<std::string, float>> models);

	int get_c4_server_time();
	float get_viewmodel_fov();

	std::string hitgroup_name(const int index);
	std::string get_weapon_type_by_index(const int index, const weapon_esp_type type);

	std::wstring stws(const std::string& string);
	std::string xor_encrypt_decrypt(const std::string& data, const std::string& key);
	std::string remove_chars_from_string(std::string string, const std::string& chars);

	_wfm_stat wait_for_module(int module_index, int ms);
	_wfm_stat wait_for_module(int module_index, int fallback_module_index, int ms);

	struct unprotect_t {
		unprotect_t(void* addr, size_t sz) : m_addr(addr), m_size(sz) {
			if (m_addr && m_size)
				VirtualProtect(m_addr, m_size, PAGE_EXECUTE_READWRITE, &m_prot);
		}

		~unprotect_t() {
			if (m_addr && m_size)
				VirtualProtect(m_addr, m_size, m_prot, &m_prot);
		}

	private:
		void*         m_addr{};
		size_t        m_size{};
		unsigned long m_prot{};
	};

	template <typename T>
	inline T& read(const uintptr_t addr)
	{
		return *reinterpret_cast<T*>(addr);
	}

	template <typename T>
	inline T read_ptr(const uintptr_t addr)
	{
		return reinterpret_cast<T>(addr);
	}

	template <typename T>
	inline void write(const uintptr_t addr, T val, bool vp = true)
	{
		unprotect_t vprotect{ vp ? reinterpret_cast<void*>(addr) : nullptr, sizeof(T) };
		*reinterpret_cast<T*>(addr) = val;
	}

	template <typename T>
	inline T get_virtual_fn(void* base, int index)
	{
		return (*static_cast<T**>(base))[index];
	}

	template <typename T, typename... Args>
	inline constexpr T call_virtual_fn(void* base, int index, Args... args)
	{
		return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(base))[index])(base, args...);
	}

	template <typename T>
	inline T get_export(const std::string& module_name, const std::string& function_name)
	{
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(module_name.c_str()), function_name.c_str()));
	}

	template <typename T>
	inline T* get_interface(const std::string& module_name, const std::string& interface_name)
	{
		const auto fn = get_export<create_interface_fn>(module_name, "CreateInterface");

		if (!fn)
			return nullptr;

		return static_cast<T*>(fn(interface_name.c_str(), {}));
	}
}