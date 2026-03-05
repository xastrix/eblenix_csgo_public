#pragma once

#include "matrix.h"
#include "vec3.h"

#include <cstdint>

using quaternion = float[4];
using rad_euler = float[3];

enum {
	max_studio_bones = 128
};

enum _bone_flags {
	bone_calculate_mask = 0x1f,
	bone_physically_simulated = 0x01,
	bone_physics_procedural = 0x02,
	bone_always_procedural = 0x04,
	bone_screen_align_sphere = 0x08,
	bone_screen_align_cylinder = 0x10,
	bone_used_mask = 0x0007ff00,
	bone_used_by_anything = 0x0007ff00,
	bone_used_by_hitbox = 0x00000100,
	bone_used_by_attachment = 0x00000200,
	bone_used_by_vertex_mask = 0x0003fc00,
	bone_used_by_vertex_lod0 = 0x00000400,
	bone_used_by_vertex_lod1 = 0x00000800,
	bone_used_by_vertex_lod2 = 0x00001000,
	bone_used_by_vertex_lod3 = 0x00002000,
	bone_used_by_vertex_lod4 = 0x00004000,
	bone_used_by_vertex_lod5 = 0x00008000,
	bone_used_by_vertex_lod6 = 0x00010000,
	bone_used_by_vertex_lod7 = 0x00020000,
	bone_used_by_bone_merge = 0x00040000,
	bone_type_mask = 0x00f00000,
	bone_fixed_alignment = 0x00100000,
	bone_has_saveframe_pos = 0x00200000,
	bone_has_saveframe_rot = 0x00400000
};

enum _hitgroups {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_gear = 10
};

enum _hitboxes {
	hitbox_head,
	hitbox_neck,
	hitbox_pelvis,
	hitbox_stomach,
	hitbox_lower_chest,
	hitbox_chest,
	hitbox_upper_chest,
	hitbox_right_thigh,
	hitbox_left_thigh,
	hitbox_right_calf,
	hitbox_left_calf,
	hitbox_right_foot,
	hitbox_left_foot,
	hitbox_right_hand,
	hitbox_left_hand,
	hitbox_right_upper_arm,
	hitbox_right_forearm,
	hitbox_left_upper_arm,
	hitbox_left_forearm,
	hitbox_max
};

struct studio_bone_t {
	int             m_name_index;

	inline char* const name(void) const {
		return ((char*)this) + m_name_index;
	}

	int             m_parent;
	int             m_bone_controller[6];

	vec3            m_pos;
	quaternion      m_quat;
	rad_euler       m_rotation;

	vec3            m_pos_scale;
	vec3            m_rot_scale;

	matrix3x4_t     m_pose_to_bone;
	quaternion      m_quat_alignement;
	int             m_flags;
	int             m_proc_type;
	int             m_proc_index;
	mutable int     m_physics_bone;

	inline void* procedure() const {
		if (m_proc_index == 0) return NULL;
		else return(void*)(((unsigned char*)this) + m_proc_index);
	};

	int             m_surface_prop_idx;

	inline char* const surface_prop(void) const {
		return ((char*)this) + m_surface_prop_idx;
	}

	inline int get_surface_prop(void) const {
		return m_surf_prop_lookup;
	}

	int             m_contents;
	int             m_surf_prop_lookup;
	int             u[7];
};

struct studio_box_t {
	int             m_bone;
	int             m_group;
	vec3            m_mins;
	vec3            m_maxs;
	int             m_name_index;
	int             u1[3];
	float           m_radius;
	int             u2[4];
};

struct studio_hitbox_set_t {
	int             m_name_index;
	int             m_hitbox_count;
	int             m_hitbox_index;

	inline char* const name(void) const {
		return ((char*)this) + m_name_index;
	}

	inline studio_box_t* hitbox(int i) const {
		return (studio_box_t*)(((unsigned char*)this) + m_hitbox_index) + i;
	}
};

struct studio_hdr_t {
	int            m_id;
	int            m_version;
	long           m_checksum;
	char           m_name_char_array[64];
	int            m_length;
	vec3           m_eye_pos;
	vec3           m_illium_pos;
	vec3           m_hull_mins;
	vec3           m_hull_maxs;
	vec3           m_mins;
	vec3           m_maxs;
	int            m_flags;
	int            m_bones_count;
	int            m_bone_index;
	int            m_bone_controllers_count;
	int            m_bone_controller_index;
	int            m_hitbox_sets_count;
	int            m_hitbox_set_index;
	int            m_local_anim_count;
	int            m_local_anim_index;
	int            m_local_seq_count;
	int            m_local_seq_index;
	int            m_activity_list_version;
	int            m_events_indexed;
	int            m_textures_count;
	int            m_texture_index;

	studio_hitbox_set_t* hitbox_set(int i) {
		if (i > m_hitbox_sets_count) return nullptr;
		return (studio_hitbox_set_t*)((uint8_t*)this + m_hitbox_set_index) + i;
	}

	studio_bone_t* bone(int i) {
		if (i > m_bones_count) return nullptr;
		return (studio_bone_t*)((uint8_t*)this + m_bone_index) + i;
	}
};