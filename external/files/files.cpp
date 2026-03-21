#include "files.h"

#include <chrono>
#include <fstream>

F_STAT Files::read(const std::string& path, const char* flag, std::string& out)
{
	FILE* f = nullptr;

#if defined(_MSC_VER) && _MSC_VER < 1920
	errno_t err = fopen_s(&f, path.c_str(), flag);
	if (err != 0 || f == nullptr) {
		out = "?";
		return FS_FAIL;
	}
#else
	f = fopen(path.c_str(), flag);
	if (f == nullptr) {
		out = "?";
		return FS_FAIL;
	}
#endif

	if (fseek(f, 0, SEEK_END) != 0) {
		fclose(f);
		out = "?";
		return FS_FAIL;
	}

	long sz = ftell(f);
	if (sz == -1L) {
		fclose(f);
		out = "?";
		return FS_FAIL;
	}

	if (fseek(f, 0, SEEK_SET) != 0) {
		fclose(f);
		out = "?";
		return FS_FAIL;
	}

	if (sz <= 0) {
		fclose(f);
		out = "?";
		return FS_FAIL;
	}

	out.resize(sz);
	auto data_r = fread(&out[0], 1, sz, f);
	fclose(f);

	if (data_r != static_cast<size_t>(sz)) {
		out = "?";
		return FS_FAIL;
	}

	return FS_OK;
}

F_STAT Files::make_dirs(const std::string& path)
{
	if (std::filesystem::exists(path))
		return FS_FAIL;

	if (!std::filesystem::create_directories(path))
		return FS_FAIL;

	return FS_OK;
}

F_STAT Files::write(const std::string& path, const char* flag, const std::string& data)
{
	FILE* f = nullptr;

#if defined(_MSC_VER) && _MSC_VER < 1920
	errno_t err = fopen_s(&f, path.c_str(), flag);
	if (err != 0 || f == nullptr) {
		return FS_FAIL;
	}
#else
	f = fopen(path.c_str(), flag);
	if (f == nullptr)
		return FS_FAIL;
#endif

	auto written = fwrite(data.data(), 1, data.size(), f);
	fclose(f);

	if (written != data.size()) {
		return FS_FAIL;
	}

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

	std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

	auto now = std::chrono::system_clock::now();
	int64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	int64_t file_ms = std::chrono::duration_cast<std::chrono::milliseconds>(sctp.time_since_epoch()).count();

	return now_ms - file_ms;
}

std::string Files::get_updated_datetime(const std::string& filename)
{
	auto ftime = std::filesystem::last_write_time(filename);

	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
		ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

	std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

	auto now = std::chrono::system_clock::now();
	int64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	int64_t file_ms = std::chrono::duration_cast<std::chrono::milliseconds>(sctp.time_since_epoch()).count();

	int64_t diff = now_ms - file_ms;

	int64_t seconds = diff / 1000;
	int64_t days = seconds / 86400;
	int64_t hours = (seconds % 86400) / 3600;
	int64_t minutes = (seconds % 3600) / 60;

	std::string result;

	if (days > 0) {
		result += std::to_string(days) + "d ";
	}
	if (hours > 0) {
		result += std::to_string(hours) + "h ";
	}
	if (minutes > 0 || (!days && !hours)) {
		result += std::to_string(minutes) + "min ago";
	}

	if (result.empty()) {
		result = "just now";
	}

	return result;
}

F_STAT Files::get_file_content(const std::string& filename, std::string& content)
{
	std::ifstream f(filename, std::ios::binary);

	if (!f.is_open())
		return FS_FAIL;

	content = { std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>() };

	return FS_OK;
}

F_STAT Files::file_exists(const std::string& path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR) {
			return FS_OK;
		}
		else if (s.st_mode & S_IFREG) {
			return FS_OK;
		}
	}

	return FS_FAIL;
}