#pragma once

#include "sdk.h"

struct events : public c_game_event_listener2 {
	void init();
	void undo();
private:
	void fire_game_event(c_game_event* event);
};

inline events g_event;