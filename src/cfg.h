#pragma once

#include <string>

class c_cfg_mgr {
public:
	void init();

	void load_startup_config();

	void load(const std::wstring& name);
	void save(const std::wstring& name);

	void undo() {}
};

inline c_cfg_mgr g_cfg;