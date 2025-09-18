#pragma once

#include <string>

struct config {
	void load_config(const std::wstring& name);
	void save_config(const std::wstring& name);
};

extern config g_cfg;