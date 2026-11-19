#pragma once

#include "material.h"

#define MATERIAL_SYSTEM_INTERFACE_VERSION "VMaterialSystem080"

#define TEXTURE_GROUP_OTHER               "Other textures"
#define TEXTURE_GROUP_MODEL               "Model textures"

using material_handle_t = unsigned short;
class i_mat_render_ctx;

class c_material_system {
public:
	c_material* find_material(const char* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0) {
		using original_fn = c_material*(__thiscall*)(c_material_system*, const char*, const char*, bool, const char*);
		return (*(original_fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}

	material_handle_t first_material() {
		using original_fn = material_handle_t(__thiscall*)(c_material_system*);
		return (*(original_fn**)this)[86](this);
	}

	material_handle_t next_material(material_handle_t handle) {
		using original_fn = material_handle_t(__thiscall*)(c_material_system*, material_handle_t);
		return (*(original_fn**)this)[87](this, handle);
	}

	material_handle_t invalid_material_handle() {
		using original_fn = material_handle_t(__thiscall*)(c_material_system*);
		return (*(original_fn**)this)[88](this);
	}

	c_material* get_material(material_handle_t handle) {
		using original_fn = c_material*(__thiscall*)(c_material_system*, material_handle_t);
		return (*(original_fn**)this)[89](this, handle);
	}
};