#include "signatures.h"

#include "helpers.h"
#include "globals.h"

void sig::init()
{
	s_device                  = scan_sig(GLOBAL(module_list[shaderapidx9DLL]), "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1;
	s_glow_manager            = scan_sig(GLOBAL(module_list[clientDLL]), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3;
	s_weapon_system           = scan_sig(GLOBAL(module_list[clientDLL]), "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2;
	s_view_matrix             = scan_sig(GLOBAL(module_list[clientDLL]), "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9");
	s_line_goes_through_smoke = scan_sig(GLOBAL(module_list[clientDLL]), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	s_has_c4                  = scan_sig(GLOBAL(module_list[clientDLL]), "56 8B F1 85 F6 74 31");
	s_input                   = scan_sig(GLOBAL(module_list[clientDLL]), "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1;
	s_cam_think               = scan_sig(GLOBAL(module_list[clientDLL]), "85 C0 75 30 38 86");
	s_is_loadoutallowed       = scan_sig(GLOBAL(module_list[clientDLL]), "84 C0 75 04 B0 01 5F");
	s_list_leaves             = scan_sig(GLOBAL(module_list[clientDLL]), "56 52 FF 50 18") + 5;
}

sig_t sig::scan_sig(const std::string& module_name, const std::string& signature)
{
	const auto handle = GetModuleHandleA(module_name.c_str());

	if (!handle)
		return {};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(handle) + dos_header->e_lfanew);

	const auto pattern_bytes = [signature]() -> std::vector<int> {
		std::vector<int> bytes{};

		auto start = const_cast<char*>(signature.c_str());
		auto end = const_cast<char*>(signature.c_str()) + signature.size();

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	}();

	const auto scan_bytes = reinterpret_cast<uint8_t*>(handle);

	const auto sz = pattern_bytes.size();
	const auto bl = pattern_bytes.data();

	const auto sz_img = nt_headers->OptionalHeader.SizeOfImage;

	for (int i = 0; i < sz_img - sz; i++) {
		auto found = true;

		for (auto j = 0; j < sz; ++j) {
			if (scan_bytes[i + j] != bl[j] && bl[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &scan_bytes[i];
	}

#ifdef _DEBUG
	printf("Wrong signature: %s\n", signature.c_str());
#endif

	return {};
}