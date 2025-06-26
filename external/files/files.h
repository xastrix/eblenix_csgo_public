#pragma once

#include <string>

enum F_STAT {
	FS_OK,
	FS_FAIL = -1,
};

namespace Files {
	F_STAT read(const std::string& path, const char* flag, std::string& out);
	F_STAT make_dirs(const std::string& path);
	F_STAT write(const std::string& path, const char* flag, const std::string& data);
}