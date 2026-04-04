#pragma once

#include "base_entity.h"

#define ENTRY_IN_USE -2

struct glow_object_definition_t {
	int            m_next_free_slot;
	c_base_entity* m_entity;
	float          m_red;
	float          m_green;
	float          m_blue;
	float          m_alpha;
	char           u0[8];
	float          m_bloom_amount;
	char           u1[4];
	bool           m_render_when_occluded;
	bool           m_render_when_unoccluded;
	bool           m_full_bloom_renderer;
	char           u2[1];
	int            m_full_bloom_stencil_test_val;
	char           u3[4];
	int            m_split_screen_slot;

	bool is_unused() const {
		return m_next_free_slot != ENTRY_IN_USE;
	}

	void set_glow(float r, float g, float b, float a, float bloom_amount = 0.8f) {
		m_red                     = r;
		m_green                   = g;
		m_blue                    = b;
		m_alpha                   = a;
		m_render_when_occluded    = true;
		m_render_when_unoccluded  = false;
		m_bloom_amount            = bloom_amount;
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