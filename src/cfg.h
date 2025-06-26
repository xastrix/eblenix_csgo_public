#pragma once

#include <string>

#define CFG_DIRECTORY_PATHS "EBX/cfg/"

#define CFG_INT_KEY         "{i32}"
#define CFG_FLOAT_KEY       "{f32}"
#define CFG_BOOL_TRUE_KEY   "{on}"
#define CFG_BOOL_FALSE_KEY  "{off}"

namespace cfg
{
	void load(const std::wstring& name);
	void save(const std::wstring& name);
}