#pragma once

#define CLIENT_ENTITY_INTERFACE_VERSION "VClientEntityList003"

class c_entity_list {
public:
	template <class T>
	T get_client_entity(int index) {
		using original_fn = T(__thiscall*)(c_entity_list*, int);
		return (*(original_fn**)this)[3](this, index);
	}

	template <class T>
	T get_client_entity_handle(unsigned int handle) {
		using original_fn = T(__thiscall*)(c_entity_list*, unsigned int);
		return (*(original_fn**)this)[4](this, handle);
	}

	int get_highest_index() {
		using original_fn = int(__thiscall*)(c_entity_list*);
		return (*(original_fn**)this)[6](this);
	}
};