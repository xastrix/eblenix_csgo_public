#pragma once

#include "material.h"

#define MATERIAL_SYSTEM_INTERFACE_VERSION "VMaterialSystem080"

#define TEXTURE_GROUP_OTHER               "Other textures"
#define TEXTURE_GROUP_MODEL               "Model textures"

class i_mat_render_ctx;

class c_material_system {
public:
	c_material* find_material(char const* material_name, const char* group_name,
		bool complain = true, const char* complain_prefix = 0);
};