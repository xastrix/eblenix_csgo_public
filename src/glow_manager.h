#pragma once

#include "vec3.h"
#include "base_entity.h"

struct glow_object_definition_t {
	c_base_entity* entity;
	vec3           glow_color;
	float          alpha;
	char           unknown0[8];
	float          bloom_amount;
	char           unknown1[4];
	bool           render_when_occluded;
	bool           render_when_unoccluded;
	bool           full_bloom_render;
	char           unknown2[13];
	int            next_free_slot;
};

class c_glow_manager {
public:
	glow_object_definition_t* objects;
	char u[8];
	int size;
};