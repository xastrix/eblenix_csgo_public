#pragma once

#include <stdint.h>
#include <string>

struct netvar_manager {
	uintptr_t get_netvar(const std::string& netvar);
};

extern netvar_manager g_netvars;