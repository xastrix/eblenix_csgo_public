#pragma once

#include "sdk.h"

struct triggerbot {
	void instance(i_user_cmd* cmd);
};

extern triggerbot g_triggerbot;