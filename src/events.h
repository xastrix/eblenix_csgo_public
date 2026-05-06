#pragma once

#include "game_event_mgr.h"

// ::shared_ptr
#include <memory>

class c_event_list : public c_game_event_listener2 {
public:
	void init();

	static std::shared_ptr<c_event_list> make_shared() {
		return std::shared_ptr<c_event_list>(new c_event_list());
	}

	void undo();

private:
	void fire_game_event(c_game_event* _event);
};

inline std::shared_ptr<c_event_list> g_event = c_event_list::make_shared();