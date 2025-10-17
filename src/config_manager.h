#pragma once

#include <string>

#define CFG_INT_KEY        "{i32}"
#define CFG_FLOAT_KEY      "{f32}"

#define CFG_BOOL_TRUE_KEY  "{true}"
#define CFG_BOOL_FALSE_KEY "{false}"

namespace config_manager
{
	void load_config(const std::wstring& name);
	void save_config(const std::wstring& name);
}