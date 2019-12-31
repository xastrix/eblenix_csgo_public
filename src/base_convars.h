#pragma once

#include "app_system.h"

#define ENGINE_CVAR_INTERFACE_VERSION "VEngineCvar007"

class convar {
public:
	float get_float() {
		using original_fn = float(__thiscall*)(convar*);
		return (*(original_fn**)this)[12](this);
	}

	int get_int() {
		using original_fn = int(__thiscall*)(convar*);
		return (*(original_fn**)this)[13](this);
	}

	void set_value(float value) {
		using original_fn = void(__thiscall*)(convar*, float);
		return (*(original_fn**)this)[15](this, value);
	}

	void set_value(int value) {
		using original_fn = void(__thiscall*)(convar*, int);
		return (*(original_fn**)this)[16](this, value);
	}

	void set_value(bool value) {
		return set_value(static_cast<int>(value));
	}
};

class c_base_convars : public c_app_system {
public:
	convar* get_convar(const char* var_name);

	template <typename... T>
	void console_color_printf(const unsigned long& clr, const char* fmt, T... args) {
		using original_fn = void(__cdecl*)(c_base_convars*, const unsigned long&, const char*, ...);
		return (*(original_fn**)this)[25](this, clr, fmt, args...);
	}

	template <typename... T>
	void console_printf(const char* fmt, T... args) {
		using original_fn = void(__cdecl*)(c_base_convars*, const char*, ...);
		return (*(original_fn**)this)[26](this, fmt, args...);
	}
};