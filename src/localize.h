#pragma once

#define LOCALIZE_INTERFACE_VERSION "Localize_001"

class c_localize {
public:
	wchar_t* find(const char* name) {
		using original_fn = wchar_t*(__thiscall*)(c_localize*, const char*);
		return (*(original_fn**)this)[11](this, name);
	}
};