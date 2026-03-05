#pragma once

#include "i_user_cmd.h"

// ::shared_ptr
#include <memory>

struct bunnyhop_t {
	bunnyhop_t(const bunnyhop_t&) = delete;
	bunnyhop_t& operator=(const bunnyhop_t&) = delete;

	static bunnyhop_t& get_instance() {
		static bunnyhop_t i{};
		return i;
	}

	void think(user_cmd_t* cmd);

private:
	bunnyhop_t() = default;
};

struct autostrafe_t {
	autostrafe_t(const autostrafe_t&) = delete;
	autostrafe_t& operator=(const autostrafe_t&) = delete;

	static autostrafe_t& get_instance() {
		static autostrafe_t i{};
		return i;
	}

	void think(user_cmd_t* cmd);

private:
	autostrafe_t() = default;
};

struct blockbot_t {
	blockbot_t(const blockbot_t&) = delete;
	blockbot_t& operator=(const blockbot_t&) = delete;

	static blockbot_t& get_instance() {
		static blockbot_t i{};
		return i;
	}

	void think(user_cmd_t* cmd);

private:
	blockbot_t() = default;
};

class c_move {
public:
	void run(user_cmd_t* cmd);

	static std::shared_ptr<c_move> make_shared() {
		return std::shared_ptr<c_move>(new c_move());
	}
};

inline std::shared_ptr<c_move> g_move = c_move::make_shared();