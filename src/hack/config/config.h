#pragma once

#include <string>

struct config {
	void load_config(const std::wstring& name);
	void save_config(const std::wstring& name);
private:
	std::string intkey{ "{i32}" },
		        floatkey{ "{f32}" },
		        truekey{ "true" },
		        falsekey{ "false" };
};

extern config g_cfg;