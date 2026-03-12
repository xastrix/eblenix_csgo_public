#pragma once

#include "base_entity.h"

#define ENTRY_IN_USE -2

struct glow_object_definition_t {
	void set_glow(float r, float g, float b, float a) {
		m_r = r;
		m_g = g;
		m_b = b;
		m_alpha = a;
		m_render_when_occluded = true;
		m_render_when_unoccluded = false;
		m_bloom_amount = 0.8f;
	}

	int            m_next_free_slot;
	c_base_entity* m_entity;
	float          m_r;
	float          m_g;
	float          m_b;
	float          m_alpha;
	char           u0[8];
	float          m_bloom_amount;
	char           u1[4];
	bool           m_render_when_occluded;
	bool           m_render_when_unoccluded;
	bool           m_full_bloom_renderer;
	char           u2[1];
	int            m_full_bloom_stencil_test_val;
	int            u3;
	int            m_split_screen_slot;
};

class c_glow_manager {
public:
	glow_object_definition_t* objects;
	char u[8];
	int size;
};