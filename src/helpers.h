#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <exception_handler.hpp>

#include "color.h"
#include "base_entity.h"

using create_interface_fn = void*(*)(const char*, int*);

enum _module_flags {
	MF_NONE,
	MF_DISABLE_LIB_CALLS,
	MF_CUSTOM_EXCEPTION_HANDLER,
};

enum _bbox_types {
	BT_STATIC,
	BT_DYNAMIC,
};

enum _weapon_esp_types {
	WE_TEXT,
	WE_ICON,
};

enum _wfm_stat {
	WM_OK,
	WM_TIMEOUT,
};

struct mod_t {
	mod_t(HMODULE mod, int reason) : m_mod(mod), m_reason(reason) {}
	~mod_t() {}

	bool in(int reason, uint8_t flags, std::function<bool(HMODULE)> fn) {
		if (!(m_reason == reason))
			return false;

		if (flags & MF_DISABLE_LIB_CALLS)
			DisableThreadLibraryCalls(m_mod);

		if (flags & MF_CUSTOM_EXCEPTION_HANDLER)
			SetUnhandledExceptionFilter(exception_handler::custom_exception_filter);

		return fn(m_mod);
	}

private:
	HMODULE m_mod;
	int     m_reason;
};

struct box {
	box() = default;
	box(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};

	int x, y, w, h;
};

namespace Helpers
{
	bool is_pistol(c_base_weapon* weapon);
	bool is_sniper(c_base_weapon* weapon);
	bool is_revolver(c_base_weapon* weapon);
	bool is_taser(c_base_weapon* weapon);
	bool is_smg(c_base_weapon* weapon);
	bool is_heavy(c_base_weapon* weapon);
	bool is_rifle(c_base_weapon* weapon);
	bool is_grenade(c_base_weapon* weapon);
	bool is_c4(c_base_weapon* weapon);
	bool is_knife(c_base_weapon* weapon);
	bool is_non_aim(c_base_weapon* weapon);

	/* Find the bone index on an entity that is closest to the player's view */
	int get_nearest_bone(c_base_player* entity, user_cmd_t* cmd);

	/* Find the best target entity index within a given fov */
	int find_target_entity(user_cmd_t* cmd, const float fov, vec3& angle);

	/* Retrieves the bounding box of an entity */
	bool get_bbox(c_base_player* entity, box& in, const _bbox_types type);
	
	/* Checks whether the line from start to end passes through smoke */
	bool is_behind_smoke(const vec3 start, const vec3 end);

	/* Checks if the in-game chat window is currently open */
	bool is_chat_opened();

	/* Modulates the world color/brightness for specific models */
	void modulate_world_brightness(std::vector<std::pair<std::string, float>> models);

	/* The server time remaining for the planted bomb */
	int get_c4_server_time();

	/* Gets the current field of view */
	float get_viewmodel_fov();

	/* Gets the hitgroup name by its index */
	std::string hitgroup_name(const int index);

	/* Gets the weapon type by its index */
	std::string get_weapon_type_by_index(const int index, const _weapon_esp_types type);

	/* Converts a utf-8 encoded std::string to a wide-character std::wstring */
	std::wstring stws(const std::string& string);

	/* XOR encryption/decryption */
	std::string xor_encrypt_decrypt(const std::string& data, const std::string& key);

	/* Removes all occurrences of specific characters from a string */
	std::string remove_chars_from_string(std::string string, const std::string& chars);

	/* Parses a JSON-like string to extract all string values enclosed in double quotes */
	std::vector<std::string> parse_json_object(const std::string& in);

	/* Prints a formatted message to the in-game console with a specified prefix */
	void console_printf_with_prefix(const char* prefix, const char* fmt, ...);

	/* Waits for a module specified by index to be loaded within a timeout period */
	_wfm_stat wait_for_module(int module_index, int ms);

	/* Waits for a module specified by index to be loaded, with a fallback module index */
	_wfm_stat wait_for_module(int module_index, int fallback_module_index, int ms);

	/* Returns the current local time as a formatted string "HH:MM:SS" */
	std::string get_current_time();

	/* RAII class to temporarily change memory protection of a specified memory region
	   Restores original protection when the object is destroyed */
	struct unprotect_t {
		// Changes memory protection of the address range to PAGE_EXECUTE_READWRITE
		unprotect_t(void* addr, size_t sz) : m_addr(addr), m_size(sz) {
			if (m_addr && m_size)
				VirtualProtect(m_addr, m_size, PAGE_EXECUTE_READWRITE, &m_prot);
		}

		// Restores the original memory protection
		~unprotect_t() {
			if (m_addr && m_size)
				VirtualProtect(m_addr, m_size, m_prot, &m_prot);
		}

	private:
		void*         m_addr{};   // Pointer to the memory region
		size_t        m_size{};   // Size of the memory region
		unsigned long m_prot{};   // Original memory protection flags
	};

	/* Reads a value of type T from a specified address in memory */
	template <typename T>
	inline T& read(const uintptr_t addr)
	{
		return *reinterpret_cast<T*>(addr);
	}

	/* Reads a pointer value (address) of type T from a specified address
	   @note: This is just casting the address, not dereferencing */
	template <typename T>
	inline T read_ptr(const uintptr_t addr)
	{
		return reinterpret_cast<T>(addr);
	}

	/* Writes a value of type T to a specified memory address */
	template <typename T>
	inline void write(const uintptr_t addr, T val, bool vp = true)
	{
		unprotect_t vprotect{ vp ? reinterpret_cast<void*>(addr) : nullptr, sizeof(T) };
		*reinterpret_cast<T*>(addr) = val;
	}

	/* Retrieves a virtual function from a class instance's vtable by index */
	template <typename T>
	inline T get_virtual_fn(void* base, int index)
	{
		return (*static_cast<T**>(base))[index];
	}

	/* Calls a virtual function on an object instance by index, passing any arguments */
	template <typename T, typename... Args>
	inline constexpr T call_virtual_fn(void* base, int index, Args... args)
	{
		return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(base))[index])(base, args...);
	}

	/* Retrieves an exported function from a module by name */
	template <typename T>
	inline T get_export(const std::string& module_name, const std::string& function_name)
	{
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(module_name.c_str()), function_name.c_str()));
	}

	/* Retrieves an interface from a specified module by name */
	template <typename T>
	inline T* get_interface(const std::string& module_name, const std::string& interface_name)
	{
		const auto fn = get_export<create_interface_fn>(module_name, "CreateInterface");

		if (!fn)
			return nullptr;

		return static_cast<T*>(fn(interface_name.c_str(), {}));
	}
}