#pragma once

#include "material.h"

#define MATERIAL_SYSTEM_INTERFACE_VERSION "VMaterialSystem080"

#define TEXTURE_GROUP_OTHER               "Other textures"
#define TEXTURE_GROUP_MODEL               "Model textures"

using material_handle_t = unsigned short;
class i_mat_render_ctx;

class c_material_system {
public:
	c_material* find_material(char const* material_name, const char* group_name,
		bool complain = true, const char* complain_prefix = 0);
	material_handle_t first_material();
	material_handle_t next_material(material_handle_t handle);
	material_handle_t invalid_material_handle();
	c_material* get_material(material_handle_t handle);
};