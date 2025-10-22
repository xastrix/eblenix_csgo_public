#pragma once

#include "sdk.h"

struct movement {
	void run(i_user_cmd* cmd);
private:
	void bunny_hop(i_user_cmd* cmd);
	void infinite_duck(i_user_cmd* cmd);
	void auto_strafe(i_user_cmd* cmd);
	void block_players(i_user_cmd* cmd);
};

inline movement g_movement;