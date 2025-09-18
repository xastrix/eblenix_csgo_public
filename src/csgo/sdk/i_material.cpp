#include "i_material.h"

void i_material::increment_reference_count()
{
	using original_fn = void(__thiscall*)(i_material*);
	return (*(original_fn**)this)[12](this);
}

void i_material::set_material_var_flag(material_var_flags flag, bool visible)
{
	using original_fn = void(__thiscall*)(i_material*, material_var_flags, bool);
	return (*(original_fn**)this)[29](this, flag, visible);
}