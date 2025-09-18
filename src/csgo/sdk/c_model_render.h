#pragma once

#include "c_studio.h"
#include "i_material.h"

#include "../math/matrix.h"

#define ENGINE_MODEL_INTERFACE_VERSION "VEngineModel016"

struct draw_model_state_t;

class c_model_render {
public:
	void override_material(i_material* material);
};