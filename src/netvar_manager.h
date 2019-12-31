#pragma once

#include <stdint.h>
#include <string>

namespace netvar_manager
{
	uintptr_t get_address(const std::string& netvar);
}