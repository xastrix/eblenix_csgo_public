#include "base_convars.h"

convar* c_base_convars::get_convar(const char* var_name)
{
	using original_fn = convar*(__thiscall*)(c_base_convars*, const char*);
	return (*(original_fn**)this)[15](this, var_name);
}