#pragma once

#include <windows.h>
#include <string>

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
inline void write(const uintptr_t addr, T val)
{
	*reinterpret_cast<T*>(addr) = val;
}

template <typename T>
inline T get_virtual_fn(void* base_class, int index)
{
	return (*static_cast<T**>(base_class))[index];
}

template <typename T, typename... Args>
inline constexpr T call_virtual_fn(void* base_class, int index, Args... args)
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(base_class))[index])(base_class, args...);
}

template <typename T>
inline T get_export(const std::string& module_name, const std::string& function_name)
{
	return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(module_name.c_str()), function_name.c_str()));
}