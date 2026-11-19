#include "model_render.h"

void c_model_render::override_material(c_material* material)
{
	using original_fn = void(__thiscall*)(c_model_render*, c_material*, int, int);
	return (*(original_fn**)this)[1](this, material, 0, 0);
}