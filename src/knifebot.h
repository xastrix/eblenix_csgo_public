#pragma once

#include "sdk.h"

struct knifebot {
	void run(i_user_cmd* cmd);
};

inline knifebot g_knifebot;