#include "material.h"

const char* c_material::get_texture_group_name()
{
	using original_fn = const char*(__thiscall*)(c_material*);
	return (*(original_fn**)this)[1](this);
}

void c_material::increment_reference_count()
{
	using original_fn = void(__thiscall*)(c_material*);
	return (*(original_fn**)this)[12](this);
}

void c_material::set_material_var_flag(material_var_flags flag, bool visible)
{
	using original_fn = void(__thiscall*)(c_material*, material_var_flags, bool);
	return (*(original_fn**)this)[29](this, flag, visible);
}

void c_material::color_modulate(float r, float g, float b)
{
	using original_fn = void(__thiscall*)(c_material*, float, float, float);
	return (*(original_fn**)this)[28](this, r, g, b);
}