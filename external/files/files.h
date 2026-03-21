#pragma once

#include <string>
#include <vector>
#include <filesystem>

enum F_STAT {
	FS_OK,
	FS_FAIL = -1,
};

namespace Files {
	F_STAT read(const std::string& path, const char* flag, std::string& out);

	F_STAT make_dirs(const std::string& path);

	F_STAT write(const std::string& path, const char* flag, const std::string& data);

	F_STAT get_directory_files(const std::string& dirname, const std::string& ext, std::vector<std::wstring>& files);

	int64_t get_file_diff(const std::string& filename);

	std::string get_updated_datetime(const std::string& filename);

	F_STAT get_file_content(const std::string& filename, std::string& content);

	F_STAT file_exists(const std::string& path);
}