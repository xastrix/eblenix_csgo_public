#pragma once

#include "sdk.h"

struct knifebot {
	void run(user_cmd_t* cmd);
};

inline knifebot g_knifebot;