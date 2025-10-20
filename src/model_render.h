#pragma once

#include "studio.h"
#include "material.h"

#define ENGINE_MODEL_INTERFACE_VERSION "VEngineModel016"

struct draw_model_state_t;

class c_model_render {
public:
	void override_material(c_material* material);
};