#pragma once

#include <wtypes.h>
#include <string>
#include <vector>

enum _signature_list {
	S_DEVICE,
	S_GLOW_MANAGER,
	S_WEAPON_SYSTEM,
	S_VIEW_MATRIX,
	S_LINE_GOES_THROUGH_SMOKE,
	S_HAS_C4,
	S_INPUT,
	S_LIST_LEAVES,
	S_HUD_ELEMENT,
	S_FIND_HUD_ELEMENT,
	S_IS_LOADOUTALLOWED,
	S_CROSSHAIR_COLOR,
	S_CAM_THINK,
#ifndef CSGO_2020_BUILD
	S_STEAM_VALIDATION_REJECTED,
#endif
	maxSignatures,
};

using sig_t = uint8_t*;

struct sig {
	void init();

	sig_t operator[](_signature_list index) const {
		return m_signatures[index];
	}

private:
	sig_t scan_sig(const std::string& module_name, const std::string& signature);
	sig_t scan_sigs(const std::string& module_name, const std::vector<std::string>& signatures);
	DWORD get_module_size(const HMODULE mod);
	std::vector<int> pattern_to_bytes(const std::string& signature);

private:
	sig_t m_signatures[maxSignatures];
};

inline sig g_sig;