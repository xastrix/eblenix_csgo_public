#pragma once

#include <string>
#include <vector>
#include <filesystem>

enum F_STAT {
	FS_CONTENT_EMPTY = -2,
	FS_FAIL = -1,
	FS_OK,
};

namespace Files {
	F_STAT read(const std::string& filename, const std::string& flag, std::string& content);

	F_STAT write(const std::string& path, const std::string& flag, const std::string& data);

	F_STAT make_dirs(const std::string& path);

	F_STAT get_directory_files(const std::string& dirname, const std::string& ext, std::vector<std::wstring>& files);

	int64_t get_file_diff(const std::string& filename);

	F_STAT exist_object(const std::string& path);
}