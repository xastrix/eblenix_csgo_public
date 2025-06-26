#include "files.h"

#include <filesystem>

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