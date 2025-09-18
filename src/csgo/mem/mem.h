#pragma once

#include <windows.h>
#include <string>

struct mem {
	template <typename T>
	T& read(const uintptr_t addr) {
		return *reinterpret_cast<T*>(addr);
	}

	template <typename T>
	T read_ptr(const uintptr_t addr) {
		return reinterpret_cast<T>(addr);
	}

	template <typename T>
	void write(const uintptr_t addr, T val) {
		*reinterpret_cast<T*>(addr) = val;
	}

	template <typename T>
	T get_virtual_fn(void* base_class, int index) {
		return (*static_cast<T**>(base_class))[index];
	}

	template <typename T, typename... Args>
	constexpr auto call_virtual_fn(void* base_class, int index, Args... args) {
		return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(base_class))[index])(base_class, args...);
	}

	template <typename T>
	T get_export(const std::string& module_name, const std::string& function_name) {
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(module_name.c_str()), function_name.c_str()));
	}

	void write_bytes(const uintptr_t addr, const std::string& str_bytes);
};

extern mem g_mem;