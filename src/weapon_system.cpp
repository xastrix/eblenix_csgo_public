#include "weapon_system.h"

weapon_info_t* c_weapon_system::get_weapon_data(unsigned int i)
{
	using original_fn = weapon_info_t*(__thiscall*)(c_weapon_system*, unsigned int);
	return (*(original_fn**)this)[2](this, i);
}