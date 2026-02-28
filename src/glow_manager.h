#pragma once

#include "base_entity.h"

#define ENTRY_IN_USE -2

struct glow_object_definition_t {
	void set_glow(float r, float g, float b, float a) {
		_r = r;
		_g = g;
		_b = b;
		alpha = a;
		render_when_occluded = true;
		render_when_unoccluded = false;
		bloom_amount = 0.8f;
	}

	int            next_free_slot;
	c_base_entity* entity;
	float          _r;
	float          _g;
	float          _b;
	float          alpha;
	char           u0[8];
	float          bloom_amount;
	char           u1[4];
	bool           render_when_occluded;
	bool           render_when_unoccluded;
	bool           full_bloom_renderer;
	char           u2[1];
	int            full_bloom_stencil_test_val;
	int            u3;
	int            split_screen_slot;
};

class c_glow_manager {
public:
	glow_object_definition_t* objects;
	char u[8];
	int size;
};