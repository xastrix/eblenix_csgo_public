#include "game_event_mgr.h"

bool c_game_event_manager2::add_listener(c_game_event_listener2* listener, const char* name, bool server_side)
{
	using original_fn = bool(__thiscall*)(c_game_event_manager2*, c_game_event_listener2*, const char*, bool);
	return (*(original_fn**)this)[3](this, listener, name, server_side);
}

int c_game_event_manager2::remove_listener(c_game_event_listener2* listener)
{
	using original_fn = int(__thiscall*)(c_game_event_manager2*, c_game_event_listener2*);
	return (*(original_fn**)this)[5](this, listener);
}