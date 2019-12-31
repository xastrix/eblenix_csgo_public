#include "material_system.h"

c_material* c_material_system::find_material(char const* material_name, const char* group_name, bool complain, const char* complain_prefix)
{
	using original_fn = c_material*(__thiscall*)(c_material_system*, char const*, const char*, bool, const char*);
	return (*(original_fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
}