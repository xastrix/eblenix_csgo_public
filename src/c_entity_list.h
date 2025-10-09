#pragma once

#define CLIENT_ENTITY_INTERFACE_VERSION "VClientEntityList003"

class c_entity_list {
public:
	void* get_client_entity(int index);
	void* get_client_entity_handle(unsigned int handle);
	int get_highest_index();
};