#pragma once

#include "sdk.h"

struct movement {
	void run(user_cmd_t* cmd);
private:
	void bunny_hop(user_cmd_t* cmd);
	void infinite_duck(user_cmd_t* cmd);
	void auto_strafe(user_cmd_t* cmd);
	void block_players(user_cmd_t* cmd);
};

inline movement g_movement;