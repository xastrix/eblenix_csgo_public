#include "entity_list.h"

void* c_entity_list::get_client_entity(int index)
{
	using original_fn = void*(__thiscall*)(c_entity_list*, int);
	return (*(original_fn**)this)[3](this, index);
}

void* c_entity_list::get_client_entity_handle(unsigned int handle)
{
	using original_fn = void*(__thiscall*)(c_entity_list*, unsigned int);
	return (*(original_fn**)this)[4](this, handle);
}

int c_entity_list::get_highest_index()
{
	using original_fn = int(__thiscall*)(c_entity_list*);
	return (*(original_fn**)this)[6](this);
}