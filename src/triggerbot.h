#pragma once

#include "sdk.h"

struct triggerbot {
	void run(i_user_cmd* cmd);
};

inline triggerbot g_triggerbot;