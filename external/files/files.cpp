#include "files.h"

#include <chrono>
#include <fstream>

F_STAT Files::read(const std::string& filename, const std::string& flag, std::string& content)
{
	std::fstream f(filename, [flag]() {
		std::ios_base::openmode mode = std::ios::in;

		if (flag.find("b") != std::string::npos)
			mode |= std::ios::binary;

		return mode;
	}());

	if (!f.is_open())
		return FS_FAIL;

	if (f.peek() == std::ifstream::traits_type::eof()) {
		content.clear();
		return FS_CONTENT_EMPTY;
	}

	content.assign((std::istreambuf_iterator<char>(f)),
                    std::istreambuf_iterator<char>());

	if (!f && !f.eof())
		return FS_FAIL;

	return FS_OK;
}

F_STAT Files::write(const std::string& path, const std::string& flag, const std::string& data)
{
	std::ofstream f(path, [flag]() {
		std::ios_base::openmode mode = std::ios::out;

		if (flag.find("a") != std::string::npos)      mode |= std::ios::app;
		else if (flag.find("w") != std::string::npos) mode |= std::ios::trunc;

		if (flag.find("b") != std::string::npos)
			mode |= std::ios::binary;

		return mode;
	}());

	if (!f.is_open())
		return FS_FAIL;

	f.write(data.data(), data.size());

	if (!f)
		return FS_FAIL;

	return FS_OK;
}

F_STAT Files::make_dirs(const std::string& path)
{
	if (exist_object(path) == FS_FAIL)
		return FS_FAIL;

	if (!std::filesystem::create_directories(path))
		return FS_FAIL;

	return FS_OK;
}

F_STAT Files::get_directory_files(const std::string& dirname, const std::string& ext, std::vector<std::wstring>& files)
{
	files.clear();

	if (!std::filesystem::exists(dirname) || !std::filesystem::is_directory(dirname))
		return FS_FAIL;

	for (const auto& entry : std::filesystem::directory_iterator(dirname))
	{
		if (entry.is_regular_file())
		{
			auto path = entry.path();

			if (ext.empty() || path.extension() == ext)
			{
				files.push_back(path.filename().wstring());
			}
		}
	}

	return FS_OK;
}

int64_t Files::get_file_diff(const std::string& filename)
{
	auto ftime = std::filesystem::last_write_time(filename);
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
		ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

	auto now = std::chrono::system_clock::now();
	int64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	int64_t file_ms = std::chrono::duration_cast<std::chrono::milliseconds>(sctp.time_since_epoch()).count();

	return now_ms - file_ms;
}

F_STAT Files::exist_object(const std::string& path)
{
	std::filesystem::file_status status = std::filesystem::status(path);

	if (!std::filesystem::is_regular_file(status) &&
		!std::filesystem::is_directory(status))
		return FS_FAIL;

	return FS_OK;
}