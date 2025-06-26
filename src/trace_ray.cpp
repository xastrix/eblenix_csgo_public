#include "trace_ray.h"

void c_trace_ray::trace_ray(const ray_t& ray, unsigned int mask, i_trace_filter* filter, trace_t* trace)
{
	using original_fn = void(__thiscall*)(c_trace_ray*, const ray_t&, unsigned int, i_trace_filter*, trace_t*);
	return (*(original_fn**)this)[5](this, ray, mask, filter, trace);
}