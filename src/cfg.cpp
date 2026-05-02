#include "cfg.h"

#include "vars.h"
#include "globals.h"
#include "helpers.h"
#include "interfaces.h"

#include <files.h>

void c_cfg_mgr::init()
{
	load_startup_config();
}

void c_cfg_mgr::load_startup_config()
{
	std::string data;

	if (!(Files::file_exists(CFG_STARTUP_FILE_PATH) == FS_OK)) {
		Files::write(CFG_STARTUP_FILE_PATH, "a", "");
		return;
	}

	if (!(Files::get_file_content(CFG_STARTUP_FILE_PATH, data) == FS_OK))
		return;
	
	load(std::wstring(data.begin(), data.end()));
}

void c_cfg_mgr::load(const std::wstring& name)
{
	std::string cfg_path = CFG_DIRECTORY_PATHS + std::string(name.begin(), name.end());
	std::string data;

	if (!(Files::read(cfg_path, "rb", data) == FS_OK))
		return;

	std::string decrypted = Helpers::xor_encrypt_decrypt(data, cfg_path);
	std::string line;

	size_t pos{};
	size_t next_pos{};

	while (pos < decrypted.size())
	{
		next_pos = decrypted.find('\n', pos);

		if (next_pos == std::string::npos)
			next_pos = decrypted.size();

		line = decrypted.substr(pos, next_pos - pos);

		const auto equal_pos = line.find('=');
		if (equal_pos == std::string::npos) {
			pos = next_pos + 1;
			continue;
		}

		const auto key = line.substr(0, equal_pos);
		const auto value = line.substr(equal_pos + 1);

		if (value == CFG_BOOL_TRUE_KEY) {
			g_var->set(key, true);
		}
		else if (value == CFG_BOOL_FALSE_KEY) {
			g_var->set(key, false);
		}
		else
		{
			if (value.find(CFG_INT_KEY) != std::string::npos) {
				g_var->set(key, std::stoi(Helpers::remove_chars_from_string(value, CFG_INT_KEY)));
			}
			else if (value.find(CFG_FLOAT_KEY) != std::string::npos) {
				g_var->set(key, std::stof(Helpers::remove_chars_from_string(value, CFG_FLOAT_KEY)));
			}
		}

		pos = next_pos + 1;
	}
}

void c_cfg_mgr::save(const std::wstring& name)
{
	std::string cfg_path = CFG_DIRECTORY_PATHS + std::string(name.begin(), name.end());
	std::string data;

	Files::make_dirs(CFG_DIRECTORY_PATHS);
	for (const auto&[k, v] : g_var->get_vars())
	{
		if (k.find(UNSAVE_MARK) != std::string::npos)
			continue;

		data += k + "=";
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, bool>) {
				data += (arg ? CFG_BOOL_TRUE_KEY : CFG_BOOL_FALSE_KEY);
			}
			else if constexpr (std::is_same_v<T, int>) {
				data += CFG_INT_KEY + std::to_string(arg);
			}
			else if constexpr (std::is_same_v<T, float>) {
				data += CFG_FLOAT_KEY + std::to_string(arg);
			}

			data += "\n";
		}, v);
	}

	Files::write(cfg_path, "wb", Helpers::xor_encrypt_decrypt(data, cfg_path));
}