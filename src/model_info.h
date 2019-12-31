#pragma once

#include "model.h"
#include "studio.h"

#define MODEL_INFO_INTERFACE_VERSION "VModelInfoClient004"

class c_model_info {
public:
	model_t* get_model(int index);
	const char* get_model_name(const model_t* model);
	studio_hdr_t* get_studio_model(const model_t* model);
};