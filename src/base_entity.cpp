#include "base_entity.h"

#include "helpers.h"

c_client_renderable* c_base_entity::get_renderable()
{
	return Helpers::read_ptr<c_client_renderable*>(uintptr_t(this) + 0x4);
}

c_client_networkable* c_base_entity::get_networkable()
{
	return Helpers::read_ptr<c_client_networkable*>(uintptr_t(this) + 0x8);
}

c_client_class* c_base_entity::get_client_class()
{
	using original_fn = c_client_class*(__thiscall*)(c_client_networkable*);
	return (*(original_fn**)get_networkable())[2](get_networkable());
}

bool c_base_entity::get_dormant()
{
	using original_fn = bool(__thiscall*)(c_client_networkable*);
	return (*(original_fn**)get_networkable())[9](get_networkable());
}

int c_base_entity::get_team_num()
{
	return Helpers::read<int>(uintptr_t(this) +
		Helpers::get_netvar("DT_CSPlayer::m_iTeamNum"));
}

bool c_base_entity::setup_bones(matrix3x4_t* out, int max_bones, int mask, float time)
{
	if (!this)
		return false;

	using original_fn = bool(__thiscall*)(void*, matrix3x4_t*, int, int, float);
	return (*(original_fn**)get_renderable())[13](get_renderable(), out, max_bones, mask, time);
}

model_t* c_base_entity::get_model()
{
	using original_fn = model_t*(__thiscall*)(c_client_renderable*);
	return (*(original_fn**)get_renderable())[8](get_renderable());
}

vec3 c_base_entity::get_absolute_origin()
{
	if (!this)
		return vec3{ 0, 0, 0 };

	using original_fn = vec3&(__thiscall*)(c_base_entity*);
	return (*(original_fn**)this)[10](this);
}

vec3 c_base_entity::get_vec_origin()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		Helpers::get_netvar("DT_BasePlayer::m_vecOrigin"));
}

int c_base_entity::draw_model(int flags, uint8_t alpha)
{
	using original_fn = int(__thiscall*)(c_client_renderable*, int, uint8_t);
	return (*(original_fn**)get_renderable())[9](get_renderable(), flags, alpha);
}

vec3 c_base_entity::mins()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		Helpers::get_netvar("DT_BaseEntity::m_vecMins"));
}

vec3 c_base_entity::maxs()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		Helpers::get_netvar("DT_BaseEntity::m_vecMaxs"));
}