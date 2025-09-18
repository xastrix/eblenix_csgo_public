#include "c_model_render.h"

void c_model_render::override_material(i_material* material)
{
	using original_fn = void(__thiscall*)(c_model_render*, i_material*, int, int);
	return (*(original_fn**)this)[1](this, material, 0, 0);
}