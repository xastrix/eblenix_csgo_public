#include "model_info.h"

model_t* c_model_info::get_model(int index)
{
	using original_fn = model_t*(__thiscall*)(c_model_info*, int);
	return (*(original_fn**)this)[1](this, index);
}

const char* c_model_info::get_model_name(const model_t* model)
{
	using original_fn = const char*(__thiscall*)(c_model_info*, const model_t*);
	return (*(original_fn**)this)[3](this, model);
}

studio_hdr_t* c_model_info::get_studio_model(const model_t* model)
{
	using original_fn = studio_hdr_t*(__thiscall*)(c_model_info*, const model_t*);
	return (*(original_fn**)this)[32](this, model);
}