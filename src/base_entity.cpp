#include "base_entity.h"

#include "netvar_manager.h"

c_client_renderable* c_base_entity::get_renderable()
{
	return reinterpret_cast<c_client_renderable*>(uintptr_t(this) + 4);
}

c_client_networkable* c_base_entity::get_networkable()
{
	return reinterpret_cast<c_client_networkable*>(uintptr_t(this) + 8);
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
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_iTeamNum"));
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
	using original_fn = model_t * (__thiscall*)(c_client_renderable*);
	return (*(original_fn**)get_renderable())[8](get_renderable());
}

vec3 c_base_entity::get_absolute_origin()
{
	if (!this)
		return vec3(0, 0, 0);

	using original_fn = vec3 & (__thiscall*)(c_base_entity*);
	return (*(original_fn**)this)[10](this);
}

vec3 c_base_entity::get_vec_origin()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_vecOrigin"));
}

int c_base_entity::index()
{
	using original_fn = int(__thiscall*)(c_client_networkable*);
	return (*(original_fn**)get_networkable())[10](get_networkable());
}

bool& c_base_entity::get_spotted()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseEntity::m_bSpotted"));
}

int c_base_entity::draw_model(int flags, uint8_t alpha)
{
	using original_fn = int(__thiscall*)(c_client_renderable*, int, uint8_t);
	return (*(original_fn**)get_renderable())[9](get_renderable(), flags, alpha);
}