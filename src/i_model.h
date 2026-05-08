#pragma once

#include "matrix.h"

struct model_t {
	void*              m_handle;
	
	char               m_name[260];
	
	int	               m_load_flags;
	int                m_server_count;
	int                m_type;
	int	               m_flags;
	
	vec3		       m_vec_mins;
	vec3		       m_vec_maxs;
	
	float              m_radius;
};

struct model_render_info_t {
	vec3		       m_origin;
	vec3		       m_angles;
	
	char		       u0[4];
	
	void*              m_renderable;
	const model_t*     m_model;
	const matrix3x4_t* m_model_to_world;
	const matrix3x4_t* m_lighting_offset;
	const vec3*        m_lighting_origin;
	
	int		           m_flags;
	int		           m_entity_index;
	int		           m_skin;
	int		           m_body;
	int		           m_hitboxset;
	
	unsigned short     m_instance;

	model_render_info_t() {
		m_model_to_world = nullptr;
		m_lighting_offset = nullptr;
		m_lighting_origin = nullptr;
	}
};