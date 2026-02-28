#include "signatures.h"

#include "globals.h"

void c_sig::init()
{
	m_signatures[S_DEVICE] = scan_sig(GLOBAL(module_list[shaderapidx9DLL]), "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1;
	m_signatures[S_GLOW_MANAGER] = scan_sig(GLOBAL(module_list[clientDLL]), "0F 11 05 ? ? ? ? 83 C8 01") + 3;
	m_signatures[S_WEAPON_SYSTEM] = scan_sig(GLOBAL(module_list[clientDLL]), "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2;
	m_signatures[S_VIEW_MATRIX] = scan_sig(GLOBAL(module_list[clientDLL]), "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9") + 3;
	m_signatures[S_LINE_GOES_THROUGH_SMOKE] = scan_sig(GLOBAL(module_list[clientDLL]), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	m_signatures[S_HAS_C4] = scan_sig(GLOBAL(module_list[clientDLL]), "56 8B F1 85 F6 74 31");
	m_signatures[S_INPUT] = scan_sig(GLOBAL(module_list[clientDLL]), "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1;
	m_signatures[S_LIST_LEAVES] = scan_sig(GLOBAL(module_list[clientDLL]), "56 52 FF 50 18") + 5;
	m_signatures[S_HUD_ELEMENT] = scan_sig(GLOBAL(module_list[clientDLL]), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1;
	m_signatures[S_FIND_HUD_ELEMENT] = scan_sig(GLOBAL(module_list[clientDLL]), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28");
	m_signatures[S_IS_LOADOUTALLOWED] = scan_sig(GLOBAL(module_list[clientDLL]), "84 C0 75 05 B0 01");
	m_signatures[S_CROSSHAIR_COLOR] = scan_sig(GLOBAL(module_list[clientDLL]), "FF 50 3C 80 7D 2C 00") + 3;
	m_signatures[S_CAM_THINK] = scan_sig(GLOBAL(module_list[clientDLL]), "85 C0 75 30 38 87");
}

sig_t c_sig::scan_sig(const std::string& module_name, const std::string& signature)
{
	const auto mod = GetModuleHandleA(module_name.c_str());

	if (!mod)
		return {};

	const auto base = reinterpret_cast<uint8_t*>(mod);
	const auto bytes = pattern_to_bytes(signature);

	const auto module_size = get_module_size(mod);

	const auto pattern_data = bytes.data();
	const auto pattern_size = bytes.size();

	for (int i = 0; i < module_size - pattern_size; i++) {
		auto found = true;

		for (int j = 0; j < pattern_size; j++) {
			if (base[i + j] != pattern_data[j] && pattern_data[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &base[i];
	}

	return {};
}

DWORD c_sig::get_module_size(const HMODULE mod)
{
	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(mod);

	if (!dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE)
		return 0L;

	const auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS>(mod + dos_header->e_lfanew);

	if (!nt_header || nt_header->Signature != IMAGE_NT_SIGNATURE)
		return 0L;

	return nt_header->OptionalHeader.SizeOfImage;
}

std::vector<int> c_sig::pattern_to_bytes(const std::string& signature)
{
	std::vector<int> ret{};

	auto start = const_cast<char*>(signature.data());
	for (auto c = start; c < start + signature.length(); c++)
	{
		if (*c == '?') {
			c++;

			if (*c == '?')
				c++;

			ret.push_back(-1);
		}
		else {
			ret.push_back(std::strtoul(c, &c, 16));
		}
	}

	return ret;
}