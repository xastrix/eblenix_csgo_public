#include "mem.h"

#include <vector>
#include <sstream>

mem g_mem;

void mem::write_bytes(const uintptr_t addr, const std::string& str_bytes)
{
	std::vector<uint8_t> bytes{};
	std::istringstream iss{ str_bytes };
	std::string token{};

	while (iss >> token) {
		std::string _hex{ token.substr(2) };

		int value{};
		std::istringstream(_hex) >> std::hex >> value;

		bytes.push_back(static_cast<uint8_t>(value));
	}

	std::memcpy(reinterpret_cast<void*>(addr), bytes.data(), bytes.size());
}