#pragma once

#include "sdk.h"

struct knifebot {
	void instance(i_user_cmd* cmd);
};

extern knifebot g_knifebot;