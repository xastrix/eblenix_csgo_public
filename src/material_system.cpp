#include "material_system.h"

c_material* c_material_system::find_material(char const* material_name, const char* group_name, bool complain, const char* complain_prefix)
{
	using original_fn = c_material*(__thiscall*)(c_material_system*, char const*, const char*, bool, const char*);
	return (*(original_fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
}

material_handle_t c_material_system::first_material()
{
	using original_fn = material_handle_t(__thiscall*)(c_material_system*);
	return (*(original_fn**)this)[86](this);
}

material_handle_t c_material_system::next_material(material_handle_t handle)
{
	using original_fn = material_handle_t(__thiscall*)(c_material_system*, material_handle_t);
	return (*(original_fn**)this)[87](this, handle);
}

material_handle_t c_material_system::invalid_material_handle()
{
	using original_fn = material_handle_t(__thiscall*)(c_material_system*);
	return (*(original_fn**)this)[88](this);
}

c_material* c_material_system::get_material(material_handle_t handle)
{
	using original_fn = c_material*(__thiscall*)(c_material_system*, material_handle_t);
	return (*(original_fn**)this)[89](this, handle);
}