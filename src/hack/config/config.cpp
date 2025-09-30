#include "config.h"

#include "../var_manager/var_manager.h"
#include "../helpers/helpers.h"

#include "../../csgo/interfaces/interfaces.h"

#include <common.h>
#include <fstream>
#include <sstream>
#include <filesystem>

config g_cfg;

enum _var_types {
	i32, f32, bl, nbl, maxVarTypes,
};

std::string v_types[maxVarTypes] = {
	"{i32}", "{f32}", "true", "false",
};

void config::load_config(const std::wstring& name)
{
	std::string cfg_path{ CONFIG_DIRECTORY_NAME + std::string{ name.begin(), name.end() } };

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

		if (value == v_types[bl]) {
			g_vars.set(key, true);
		}
		else if (value == v_types[nbl]) {
			g_vars.set(key, false);
		}
		else
		{
			if (value.find(v_types[i32]) != std::string::npos) {
				g_vars.set(key, std::stoi(Helpers::remove_chars_from_string(value, v_types[i32])));
			}
			else if (value.find(v_types[f32]) != std::string::npos) {
				g_vars.set(key, std::stof(Helpers::remove_chars_from_string(value, v_types[f32])));
			}
		}
	}

	ifs.close();
}

void config::save_config(const std::wstring& name)
{
	if (!std::filesystem::exists(CONFIG_DIRECTORY_NAME))
		std::filesystem::create_directories(CONFIG_DIRECTORY_NAME);

	std::string cfg_path{ CONFIG_DIRECTORY_NAME + std::string{ name.begin(), name.end() } };

	std::ofstream ofs{ cfg_path, std::ios::binary };

	if (!ofs.is_open())
		return;

	std::ostringstream oss{};

	for (const auto&[k, v] : g_vars.get_vars()) {
		oss << k << "=";
		std::visit([&oss](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, bool>) {
				oss << v_types[arg ? bl : nbl];
			}
			else if constexpr (std::is_same_v<T, int>) {
				oss << v_types[i32] << arg;
			}
			else if constexpr (std::is_same_v<T, float>) {
				oss << v_types[f32] << arg;
			}

			oss << std::endl;
		}, v);
	}

	std::string encrypted{ Helpers::xor_encrypt_decrypt(oss.str(), cfg_path) };

	ofs.write(encrypted.c_str(), encrypted.size());
	ofs.close();
}