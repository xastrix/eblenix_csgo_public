#pragma once

#include "matrix.h"

struct model_t {
	void* handle;
	char  name[260];
	int	  load_flags;
	int   server_count;
	int   type;
	int	  flags;
	vec3  vec_mins;
	vec3  vec_maxs;
	float radius;
};

struct model_render_info_t {
	vec3		       origin;
	vec3		       angles;
	char		       u0[0x4];
	void*              renderable;
	const model_t*     model;
	const matrix3x4_t* model_to_world;
	const matrix3x4_t* lighting_offset;
	const vec3*        lighting_origin;
	int		           flags;
	int		           entity_index;
	int		           skin;
	int		           body;
	int		           hitboxset;
	unsigned short     instance;

	model_render_info_t() {
		model_to_world = nullptr;
		lighting_offset = nullptr;
		lighting_origin = nullptr;
	}
};