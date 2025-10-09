#pragma once

#include <string>

namespace config_manager
{
	void load_config(const std::wstring& name);
	void save_config(const std::wstring& name);
}