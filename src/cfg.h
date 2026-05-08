#pragma once

#include <string>

// ::shared_ptr
#include <memory>

class c_cfg_mgr {
public:
	void init();

	void load_startup_config();

	void load(const std::wstring& name);
	void save(const std::wstring& name);

	static std::shared_ptr<c_cfg_mgr> make_shared() {
		return std::shared_ptr<c_cfg_mgr>(new c_cfg_mgr());
	}

	void undo() {}
};

inline std::shared_ptr<c_cfg_mgr> g_cfg = c_cfg_mgr::make_shared();