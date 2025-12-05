#pragma once

#include "matrix.h"

#define TRACE_INTERFACE_VERSION "EngineTraceClient004"

#define MASK_SHOT      (0x1 | 0x4000 | 0x2000000 | 0x2 | 0x4000000 | 0x40000000)
#define	CONTENTS_GRATE 0x8

class c_base_player;

enum trace_type_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct ray_t {
	vec_aligned m_start;
	vec_aligned m_delta;
	vec_aligned m_start_offset;
	vec_aligned m_extents;
	const matrix3x4_t* m_world_axis_transform;
	bool m_is_ray;
	bool m_is_swept;

	void init(const vec3& start, const vec3& end) {
		m_delta = end - start;

		m_is_swept = (m_delta.length_sqr() != 0);

		m_extents.x = m_extents.y = m_extents.z = 0.0f;
		m_is_ray = true;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = start;
	}
};

struct csurface_t {
	const char* name;
	short surface_props;
	unsigned short flags;
};

struct cplane_t {
	vec3 normal;
	float dist;
	unsigned char type;
	unsigned char sign_bits;
	unsigned char u1[2];
};

struct trace_t {
	vec3 start;
	vec3 end;
	cplane_t plane;
	float flFraction;
	int contents;
	unsigned short disp_flags;
	bool allsolid;
	bool startSolid;
	float fraction_left_solid;
	csurface_t surface;
	int hitgroup;
	short physics_bone;
	c_base_player* entity;
	int hitbox;

	bool did_hit() const {
		return flFraction < 1.f;
	}

	bool did_hit_world() const {
		return false;
	}

	bool did_hit_non_world_entity() const {
		return entity != NULL && !did_hit_world();
	}
};

class i_trace_filter {
public:
	virtual bool should_hit_entity(c_base_player* entity, int) = 0;
	virtual trace_type_t get_trace_type() const = 0;
};

class trace_filter : public i_trace_filter {
public:
	bool should_hit_entity(c_base_player* entity, int) {
		return (entity != skip);
	}

	trace_type_t get_trace_type() const {
		return TRACE_EVERYTHING;
	}

	c_base_player* skip;
};

class c_trace_ray {
public:
	void trace_ray(const ray_t& ray, unsigned int mask, i_trace_filter* filter, trace_t* trace);
};