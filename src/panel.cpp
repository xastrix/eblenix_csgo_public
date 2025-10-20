#include "panel.h"

const char* c_panel::get_panel_name(unsigned int panel_id)
{
	using original_fn = const char*(__thiscall*)(c_panel*, unsigned int);
	return (*(original_fn**)this)[36](this, panel_id);
}