#pragma once

#include "base_entity.h"
#include "matrix.h"

#define ENGINE_TRACE_INTERFACE_VERSION "EngineTraceClient004"

#define	CONTENTS_SOLID			0x1
#define	CONTENTS_WINDOW			0x2
#define	CONTENTS_GRATE          0x8
#define CONTENTS_MOVEABLE		0x4000

#define	CONTENTS_MONSTER        0x2000000
#define	CONTENTS_DEBRIS         0x4000000
#define	CONTENTS_DETAIL         0x8000000
#define	CONTENTS_TRANSLUCENT    0x10000000
#define	CONTENTS_LADDER         0x20000000
#define CONTENTS_HITBOX         0x40000000

#define	MASK_SHOT               CONTENTS_SOLID     | \
                                CONTENTS_MOVEABLE  | \
                                CONTENTS_MONSTER   | \
                                CONTENTS_WINDOW    | \
                                CONTENTS_DEBRIS    | \
                                CONTENTS_HITBOX

enum _trace_types {
	TRACE_EVERYTHING,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

struct ray_t {
	vec_aligned      start;
	vec_aligned      delta;
	vec_aligned      start_offset;
	vec_aligned      extents;
	const
		matrix3x4_t* world_axis_transform;
	bool             is_ray;
	bool             is_swept;

	ray_t() : world_axis_transform(NULL), is_ray(false), is_swept(false) {}

	ray_t(const vec3& start, const vec3& end) {
		init(start, end);
	}

	void init(const vec3& _start, const vec3& end) {
		delta = end - _start;

		is_swept = length(delta) != 0;

		extents = vec3(0.0f, 0.0f, 0.0f);
		world_axis_transform = NULL;
		is_ray = true;

		start_offset = vec3(0.0f, 0.0f, 0.0f);
		start = _start;
	}
};

struct csurface_t {
	const char*    name;
	short          surface_props;
	unsigned short flags;
};

struct cplane_t {
	vec3          normal;
	float         dist;
	unsigned char type;
	unsigned char sign_bits;
	unsigned char u1[2];
};

struct trace_t {
	vec3           start;
	vec3           end;
	cplane_t       plane;
	float          fraction;
	int            contents;
	unsigned short disp_flags;
	bool           allsolid;
	bool           startsolid;
	float          fraction_left_solid;
	csurface_t     surface;
	int            hitgroup;
	short          physics_bone;
	c_base_player* entity;
	int            hitbox;

	bool did_hit() const { return fraction < 1.0f; }
	bool did_hit_world() const { return false; }
	bool did_hit_non_world_entity() const { return entity != NULL && !did_hit_world(); }
};

class c_trace_filter {
public:
	virtual bool should_hit_entity(c_base_player* entity, int) = 0;
	virtual _trace_types get_trace_type() const = 0;
};

class trace_filter : public c_trace_filter {
public:
	bool should_hit_entity(c_base_player* entity, int) {
		return entity != fp;
	}

	_trace_types get_trace_type() const {
		return TRACE_EVERYTHING;
	}

	void* fp;
};

class c_engine_trace {
public:
	void clip_ray_to_entity(const ray_t& ray, unsigned int mask, c_base_entity* e, trace_t* trace) {
		using original_fn = void(__thiscall*)(c_engine_trace*, const ray_t&, unsigned int, c_base_entity*, trace_t*);
		return (*(original_fn**)this)[3](this, ray, mask, e, trace);
	}

	void trace_ray(const ray_t& ray, unsigned int mask, c_trace_filter* filter, trace_t* trace) {
		using original_fn = void(__thiscall*)(c_engine_trace*, const ray_t&, unsigned int, c_trace_filter*, trace_t*);
		return (*(original_fn**)this)[5](this, ray, mask, filter, trace);
	}
};