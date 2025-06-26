#pragma once

#include "sdk.h"

struct triggerbot {
	void run(user_cmd_t* cmd);
};

inline triggerbot g_triggerbot;