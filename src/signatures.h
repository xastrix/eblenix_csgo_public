#pragma once

#include <stdint.h>
#include <string>

using sig_t = uint8_t*;

struct sig {
	void init();

	sig_t s_device;
	sig_t s_glow_manager;
	sig_t s_weapon_system;
	sig_t s_view_matrix;
	sig_t s_line_goes_through_smoke;
	sig_t s_has_c4;
	sig_t s_input;
	sig_t s_cam_think;
	sig_t s_is_loadoutallowed;
	sig_t s_list_leaves;
private:
	sig_t scan_sig(const std::string& module_name, const std::string& signature);
};

inline sig g_sig;