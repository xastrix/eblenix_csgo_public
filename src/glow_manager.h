#pragma once

#include "base_entity.h"

#define ENTRY_IN_USE -2

struct glow_object_definition_t {
	int            next_free_slot;
	c_base_entity* entity;
	float          red;
	float          green;
	float          blue;
	float          alpha;
	char           u0[8];
	float          bloom_amount;
	char           u1[4];
	bool           render_when_occluded;
	bool           render_when_unoccluded;
	bool           full_bloom_renderer;
	char           u2[1];
	int            full_bloom_stencil_test_val;
	char           u3[4];
	int            split_screen_slot;

	bool is_unused() const {
		return next_free_slot != ENTRY_IN_USE;
	}

	void set_glow(float r, float g, float b, float a, float bloom_amount = 0.8f) {
		red                     = r;
		green                   = g;
		blue                    = b;
		alpha                   = a;
		render_when_occluded    = true;
		render_when_unoccluded  = false;
		bloom_amount            = bloom_amount;
	}
};

class c_glow_manager {
public:
	int get_size() {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xC);
	}

	glow_object_definition_t* glow_object_definitions;
	int first_free_slot;
};