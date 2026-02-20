#pragma once

#include "game_event_mgr.h"

class c_event_list : public c_game_event_listener2 {
public:
	void init();
	void undo();

private:
	void fire_game_event(c_game_event* event);
};

inline c_event_list g_event;