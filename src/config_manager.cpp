#include "config_manager.h"

#include "vars.h"
#include "helpers.h"
#include "interfaces.h"

#include <fstream>
#include <sstream>
#include <filesystem>

void config_manager::load_config(const std::wstring& name)
{
	std::string cfg_path{ CFG_DIRECTORY_PATHS + std::string{ name.begin(), name.end() } };

	std::ifstream ifs{ cfg_path, std::ios::binary };

	if (!ifs.is_open())
		return;

	std::string encrypted{ (std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>() };
	std::string decrypted{ Helpers::xor_encrypt_decrypt(encrypted, cfg_path) };

	std::istringstream iss{ decrypted };
	std::string line{};

	while (std::getline(iss, line)) {
		const auto equal_pos = line.find('=');

		if (equal_pos == std::string::npos)
			continue;

		const auto key = line.substr(0, equal_pos);
		const auto value = line.substr(equal_pos + 1);

		if (value == CFG_BOOL_TRUE_KEY) {
			g_vars.set(key, true);
		}
		else if (value == CFG_BOOL_FALSE_KEY) {
			g_vars.set(key, false);
		}
		else
		{
			if (value.find(CFG_INT_KEY) != std::string::npos) {
				g_vars.set(key, std::stoi(Helpers::remove_chars_from_string(value, CFG_INT_KEY)));
			}
			else if (value.find(CFG_FLOAT_KEY) != std::string::npos) {
				g_vars.set(key, std::stof(Helpers::remove_chars_from_string(value, CFG_FLOAT_KEY)));
			}
		}
	}

	ifs.close();
}

void config_manager::save_config(const std::wstring& name)
{
	if (!std::filesystem::exists(CFG_DIRECTORY_PATHS))
		std::filesystem::create_directories(CFG_DIRECTORY_PATHS);

	std::string cfg_path{ CFG_DIRECTORY_PATHS + std::string{ name.begin(), name.end() } };

	std::ofstream ofs{ cfg_path, std::ios::binary };

	if (!ofs.is_open())
		return;

	std::ostringstream oss{};

	for (const auto&[k, v] : g_vars.get_vars()) {
		oss << k << "=";
		std::visit([&](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, bool>) {
				oss << (arg ? CFG_BOOL_TRUE_KEY : CFG_BOOL_FALSE_KEY);
			}
			else if constexpr (std::is_same_v<T, int>) {
				oss << CFG_INT_KEY << arg;
			}
			else if constexpr (std::is_same_v<T, float>) {
				oss << CFG_FLOAT_KEY << arg;
			}

			oss << std::endl;
		}, v);
	}

	std::string encrypted{ Helpers::xor_encrypt_decrypt(oss.str(), cfg_path) };

	ofs.write(encrypted.c_str(), encrypted.size());
	ofs.close();
}