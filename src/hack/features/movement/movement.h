#pragma once

#include "../../../csgo/sdk/sdk.h"

struct movement {
	void instance(i_user_cmd* cmd);
private:
	void bunny_hop(i_user_cmd* cmd);
	void infinite_duck(i_user_cmd* cmd);
	void auto_strafe(i_user_cmd* cmd);
	void block_players(i_user_cmd* cmd);
};

extern movement g_movement;