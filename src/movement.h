#pragma once

#include "user_cmd.h"

// ::shared_ptr
#include <memory>

class c_move {
public:
	void run(user_cmd_t* cmd);

	static std::shared_ptr<c_move> make_shared() {
		return std::shared_ptr<c_move>(new c_move());
	}

private:
	void bunny_hop(user_cmd_t* cmd);
	void infinite_duck(user_cmd_t* cmd);
	void auto_strafe(user_cmd_t* cmd);
	void block_players(user_cmd_t* cmd);
};

inline std::shared_ptr<c_move> g_move = c_move::make_shared();