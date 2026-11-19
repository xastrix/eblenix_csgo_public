#include "render_view.h"

void c_render_view::set_blend(float blend)
{
	using original_fn = void(__thiscall*)(c_render_view*, float);
	return (*(original_fn**)this)[4](this, blend);
}

void c_render_view::modulate_color(float const* blend)
{
	using original_fn = void(__thiscall*)(c_render_view*, float const*);
	return (*(original_fn**)this)[6](this, blend);
}