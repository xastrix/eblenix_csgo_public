#pragma once

#include "game_event_mgr.h"

#include <string>
// ::shared_ptr
#include <memory>

enum _event_list {
	PLAYER_HURT,
	PLAYER_DEATH,
	PLAYER_SPAWN,
	PLAYER_CONNECT,
	PLAYER_DISCONNECT,
	PLAYER_FOOTSTEP,
	PLAYER_FALLDAMAGE,
	PLAYER_GIVEN_C4,
	HOSTAGE_HURT,
	HOSTAGE_KILLED,
	HOSTAGE_RESCUED,
	ITEM_PURCHASE,
	ROUND_PRESTART,
	ROUND_START,
	ROUND_END,
	BOMB_BEGINPLANT,
	BOMB_PLANTED,
	BOMB_EXPLODED,
	BOMB_BEGINDEFUSE,
	BOMB_ABORTDEFUSE,
	BOMB_DEFUSED,
	BOMB_DROPPED,
	BOMB_BEEP,
	maxEvents,
};

class c_event_list : public c_game_event_listener2 {
public:
	void init();
	void undo();

	static std::shared_ptr<c_event_list> make_shared() {
		return std::shared_ptr<c_event_list>(new c_event_list());
	}

private:
	void fire_game_event(c_game_event* _event);
};

inline std::string g_event_list[maxEvents] = {
	"player_hurt",
	"player_death",
	"player_spawn",
	"player_connect",
	"player_disconnect",
	"player_footstep",
	"player_falldamage",
	"player_given_c4",
	"hostage_hurt",
	"hostage_killed",
	"hostage_rescued",
	"item_purchase",
	"round_prestart",
	"round_start",
	"round_end",
	"bomb_beginplant",
	"bomb_planted",
	"bomb_exploded",
	"bomb_begindefuse",
	"bomb_abortdefuse",
	"bomb_defused",
	"bomb_dropped",
	"bomb_beep",
};
inline std::shared_ptr<c_event_list> g_event = c_event_list::make_shared();