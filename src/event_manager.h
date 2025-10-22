#pragma once

#include "sdk.h"

struct event_manager : public c_game_event_listener2 {
	void init();
	void undo();
private:
	void fire_game_event(c_game_event* event);
};

inline event_manager g_events;