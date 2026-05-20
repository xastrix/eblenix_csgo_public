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
	maxSignatures,
};

using sig_t = uint8_t*;

class c_sig {
public:
	void init();

	sig_t scan_sig(const std::string& module_name, const std::string& signature);

	sig_t operator[](_signature_list index) const {
		return m_signatures[index];
	}

	static std::shared_ptr<c_sig> make_shared() {
		return std::shared_ptr<c_sig>(new c_sig());
	}

private:
	DWORD get_module_size(const HMODULE mod);
	std::vector<int> pattern_to_bytes(const std::string& signature);

private:
	sig_t m_signatures[maxSignatures]{};
};

inline std::shared_ptr<c_sig> g_sig = c_sig::make_shared();

#define SIG(index) g_sig->operator[](index)