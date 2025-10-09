#include "c_engine_client.h"

#include <common.h>

int c_engine_client::get_local_player()
{
	using original_fn = int(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[12](this);
}

int c_engine_client::get_player_for_user_id(int user_id)
{
	using original_fn = int(__thiscall*)(c_engine_client*, int);
	return (*(original_fn**)this)[9](this, user_id);
}

void c_engine_client::get_player_info(int index, player_info_t* info)
{
	using original_fn = void(__thiscall*)(c_engine_client*, int, player_info_t*);
	return (*(original_fn**)this)[8](this, index, info);
}

bool c_engine_client::is_in_game()
{
	using original_fn = bool(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[26](this);
}

bool c_engine_client::is_connected()
{
	using original_fn = bool(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[is_connected_fn_index](this);
}

bool c_engine_client::is_playing()
{
	return (is_connected() && is_in_game());
}

bool c_engine_client::is_console_visible()
{
	using original_fn = bool(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[11](this);
}

const char* c_engine_client::get_product_version_string()
{
	using original_fn = const char*(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[105](this);
}

void c_engine_client::set_view_angles(vec3& angles)
{
	using original_fn = void(__thiscall*)(c_engine_client*, vec3&);
	return (*(original_fn**)this)[19](this, angles);
}

vec3 c_engine_client::get_view_angles()
{
	vec3 temp;
	
	using original_fn = void(__thiscall*)(c_engine_client*, vec3&);
	(*(original_fn**)this)[18](this, temp);
	
	return temp;
}

void* c_engine_client::get_bsp_query()
{
	using original_fn = void*(__thiscall*)(c_engine_client*);
	return (*(original_fn**)this)[43](this);
}

void c_engine_client::exec_cmd(const char* cmd)
{
	using original_fn = void(__thiscall*)(c_engine_client*, const char*);
	return (*(original_fn**)this)[108](this, cmd);
}